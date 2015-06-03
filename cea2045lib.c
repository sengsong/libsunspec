#define CEA2045Port "/dev/ttyUSB0"
#define SHED 0x01
#define END_SHED 0x02
#define APP_ACK 0x03
#define APP_NAK 0x04
#define POWER_LEVEL 0x06
#define PRESENT_RELATIVE_PRICE 0x07
#define NEXT_RELATIVE_PRICE 0x08
#define TIME_REMAINING_PRICE 0x09
#define CPP 0x0a
#define GRID_EMERGENCY 0x0b
#define GRID_GUIDANCE 0x0c
#define COMM_STATUS 0x0e
#define CUST_OVERRIDE 0x11
#define OPER_STATE_REQ 0x12
#define OPER_STATE_RESP 0x13
#define SLEEP_STATE 0x14
#define WAKE_STATE 0x15
#define SIMPLE_TIME_SYNC 0x16
#define LOADUP 0x17
#define INFO_REQ 0x01
#define SET_UTC_TIME 0x02

#define TIMEOUT 500				// 500 millisecond from first to last byte of message
#define NB_ENABLE 0
#define NB_DISABLE 1

#include <stdio.h>
#include <string.h>
#include <unistd.h>	
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

/* Structure definitions */
struct cea2045BasicStruct {
	float	priceRatio;			// Present Relative Price ratio to send to SGD
	time_t	endTime;			// Date/Time in OADR format converted to UTC
	bool	overRide;			// Override status
	bool	sleepState;			// Sleep status
	bool	pendingOpState;		// Set to true if a OpState response is pending
	bool 	pendingAppAck;		// Flag to show a Application Ack is waiting to be sent to SGD
	bool 	pendingAppNak;		// Flag to show a Application Nak is waiting to be sent to SGD
	unsigned char opState;		// Latest operationa state
	unsigned char appAckSend;	// Application Ack/Nak Opcode 2 value to be sent to SGD in App Ack/Nak message
	unsigned char appAckRtn;	// Return opcode expected on SGD application ack response
	unsigned char opCode1;		// Last received OpCode1 from SGD
	unsigned char opCode2;		// Last received OpCode2 from SGD
	time_t timeoutTime;			// Time to wait for reply, 0 = not waiting for reply
};

struct cea2045InterStruct {
	unsigned char respOpCode1;	// Expected response opcode 1, 0 if none pending
	unsigned char respOpCode2;	// Expected response opcode 2
	bool respPending;			// A reaponse is expected
	time_t timeoutTime;			// Time to wait for reply, 0 = not waiting for reply
	unsigned char lastOpCode1;	// Last successfully completed intermediate command
	time_t completeTime;		// Time last command completed successfully
};
struct linkStruct {
	int		txCount;			// App layer writes a value to this after filling the txBuffer
								// Link layer clears this value after sending characters
	int		rxCount;			// Link layer writes a value to this after all characters are received
								// App layer clears this value after processing characters
	int		nakVal;				// Value returned in link nak from SGD or -1 if link ack returned
	bool	rxOverflow;			// Set by Link layer if rxCount exceeds rxBuffer size. Cleared on rxFlush
	bool	rxFlush;			// App layer sets to true to flush rxBuffer.
								// Link layer sets to false after setting rxCount to 0
	bool	txFlush;			// App layer sets to true to flush txBuffer.
								// Link layer sets to false after setting txCount to 0
	bool	txBlock;			// Transmit must wait 100ms before sending message after link ack/nak.
	bool	sunSpec;			// App layer sets to true to this is a SunSpec SGD
};

struct devInfoStruct {			// Device Information structure
	unsigned char respCode;		// Response code
	unsigned char ceaVer[2];	// ASCII CEA-2045 version
	uint16_t vendorID;			// 16 bit vendor ID
	uint16_t devType;			// 16 bit device type
	uint16_t devRev;			// 16 bit device revision
	uint32_t capBitmap;			// 32 bit capability bitmap
	unsigned char reserve;		// Reserved (not currently used)
	unsigned char modelNum[16];	// ASCII model number
	unsigned char serialNum[16];	// ASCII serial number
	unsigned char firmYear;		//Firmware year
	unsigned char firmMonth;	//Firmware month
	unsigned char firmDay;		//Firmware day
	unsigned char firmMajor;	//Firmware major
	unsigned char firmMinor;	//Firmware minor
};

struct	utcTimeStruct {
	time_t lastTime;	//Last time sent or received
	unsigned char tz;	//Time zone offset in 15 minute blocks
	unsigned char dst;	//DST offset in 15 minute blocks
};

// Function declorations
int CEA2045basic(unsigned char cmd, long cmdparam, float cmdparamf);
int CEA2045basicRx();
int CEA2045inter(unsigned char opcode1,unsigned char opcode2);
int CEA2045interRx();
int linkLayer();
long int calcTimeDiff(struct timeval *compareTime);
void addTimeOffset(struct timeval *timestruct, long int adder);
unsigned char calcRelativePriceByte(float relativePrice);
int calcChecksum(unsigned char* buffData, int msgSize, bool validate);
int initSerialPort();
void nonblock(int state);
int kbhit();

//Globals
volatile bool STOP = false;
unsigned char txBuffer[256];		// Buffer for characters to be sent to SGD
unsigned char rxBuffer[256];		// Buffer for characters received from SGD
unsigned char linkAckBuff[2];
struct cea2045BasicStruct basic;
struct cea2045InterStruct inter;
struct linkStruct linkData;
struct devInfoStruct devInfo;		// Device Information structure
struct utcTimeStruct utcTime;		// Set-Get utc time structure
struct termios tio;
struct timeval rxStartTime;
struct timeval txStartTime;
struct timeval currentTime;
struct timeval pendingTime;
struct timeval pendAckTime;
struct timeval txDelayTime;
struct timeval pendingTxTime;
time_t shedEndTime = 0;
time_t currentTimeSec = 0;
int expectedMsgSize = 0;
int rxCount = 0;
int cea2045_fd;
bool pendingTxLinkAck = false;
bool pendingLinkAck = false;

/**
 ****************************************************************************
 * Main routine                                                             *
 ****************************************************************************
 */
int main(int Parm_Count, char *Parms[])
{
	int i;
//	int count = 0;
	int teststep = 0;
	char Key;
	time_t cycleTime = 0;			//Used for example command sequence only
	int rtn;
	unsigned char tempstr[64];
	int tempInt;

	nonblock(NB_ENABLE);			// Set keyboard to non-blocking input
	cea2045_fd = initSerialPort();	//open the device(com port) to be non-blocking
	if (cea2045_fd < 0)
	{
		perror(CEA2045Port);
		return 3;			// Exit on failure
	}
	teststep = 12;
	cycleTime = time(&currentTimeSec) + 10;
	//***************************************************************
	// This is the main program loop
	//***************************************************************
	while (STOP == false)
	{
	
		// This loop will run roughly every millisecond
		rtn = linkLayer();

		//Look for keyboard input - This section is for debugging only
		i = 0;
		i=kbhit();
		if (i!= 0) {
			Key=fgetc(stdin);
			switch (Key)
			{ /* branch to appropiate key handler */
				case 0x1b: /* Esc */
					STOP = true;
					printf("\r\n");
					break;
				default:
					// do nothing for any other key
					break;
			}  //end of switch key
		}  //end if a key was hit
		
		currentTimeSec = time(&currentTimeSec);
		if (cycleTime <= currentTimeSec) {
			switch (teststep) {
				//================================================================================
				// This section provides examples of using the CEA-2045 basic commands 
				//================================================================================
				case 0:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue shed command\n");
						rtn = CEA2045basic(SHED, 60, 0);		//Issue shed command duration = 60 seconds
						if (rtn == 0) {
							basic.appAckRtn = SHED;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 1:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue cpp command\n");
						rtn = CEA2045basic(CPP, 60, 0);		//Issue shed command duration = 60 seconds
						if (rtn == 0) {
							basic.appAckRtn = CPP;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 2:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue grid emergency command\n");
						rtn = CEA2045basic(GRID_EMERGENCY, 60, 0);		//Issue grid emergency command duration = 60 seconds
						if (rtn == 0) {
							basic.appAckRtn = GRID_EMERGENCY;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 3:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue time remaining at price command\n");
						rtn = CEA2045basic(TIME_REMAINING_PRICE, 60, 0);		//Issue time remaining command duration = 60 seconds
						if (rtn == 0) {
							basic.appAckRtn = TIME_REMAINING_PRICE;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 4:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue End Shed command\n");
						rtn = CEA2045basic(END_SHED, 0, 0);	//Issue end shed command
						if (rtn == 0) {
							basic.appAckRtn = END_SHED;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 5:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue Grid Guidance command\n");
						rtn = CEA2045basic(GRID_GUIDANCE, 0, 0);	//Issue grid guidance command
						//Second parameters are: 
						// 0 = Bad time to use energy
						// 1 = Neutral
						// 2 = Good time to use energy
						if (rtn == 0) {
							basic.appAckRtn = GRID_GUIDANCE;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 6:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue Outside Comm Connection Status\n");
						rtn = CEA2045basic(COMM_STATUS, 1, 0);	//Issue comm status command
						//Second parameters are: 
						// 0 = No/Lost connection
						// 1 = Found/Good connection
						// 3 = Poor/unreliable connection
						if (rtn == 0) {
							basic.appAckRtn = COMM_STATUS;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 7:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue Operational State Request\n");
						rtn = CEA2045basic(OPER_STATE_REQ, 0, 0);	//Issue operational state request
						if (rtn == 0) {
							basic.pendingOpState = true;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 8:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue Simple Time Sync command\n");
						rtn = CEA2045basic(SIMPLE_TIME_SYNC, 1, 0);	//Issue simple time sync command (Sunday at 1AM)
						//Second parameter is: 
						// Bits 7 to 5 is weekday 0 = Sunday - 6 = Saturday
						// Bits 7 to 5 is hour 0 to 23
						// Command is only issued on exact hour transition
						if (rtn == 0) {
							basic.appAckRtn = SIMPLE_TIME_SYNC;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 9:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue Power Level command\n");
						rtn = CEA2045basic(POWER_LEVEL, 64, 0);	//Issue power level command
						//Second parameter is: 
						// Bit 7 (MSB) 0 = power absorbed, 1 = power produced
						// Bits 0 to 6 is percent power requested in 0 to 127 scale (127 = 100%)
						if (rtn == 0) {
							basic.appAckRtn = POWER_LEVEL;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 10:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue Present Relative Price command\n");
						rtn = CEA2045basic(PRESENT_RELATIVE_PRICE, 0, 1.9);	//Issue present relative price at 1.9 command
						//Second parameter is: 
						// relative price ratio = (Byte Value - 1) * (Byte Value + 63) / 8192
						if (rtn == 0) {
							basic.appAckRtn = PRESENT_RELATIVE_PRICE;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 11:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue Next Relative Price command\n");
						rtn = CEA2045basic(NEXT_RELATIVE_PRICE, 0, 2.3);	//Issue simple time sync command
						//Second parameter is: 
						// relative price ratio = (Byte Value - 1) * (Byte Value + 63) / 8192
						if (rtn == 0) {
							basic.appAckRtn = NEXT_RELATIVE_PRICE;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 12:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue Load Up command\n");
						rtn = CEA2045basic(LOADUP, 3600, 0);	//Issue Load Up command with a duration of 1 hour
						if (rtn == 0) {
							basic.appAckRtn = LOADUP;
							basic.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 13:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						printf("Issue intermediate info request\n");
						//=============================================================================
						// When this command completes, the inter.lastOpCode1 will contain the opcode 1 value
						// and inter.completeTime will contain the time it completed.
						// This is the same for all intermediate commands.
						//=============================================================================
						rtn = CEA2045inter(INFO_REQ, 1);	//Issue an intermediate information request
						if (rtn == 0) {
							inter.respOpCode1 = INFO_REQ;
							inter.respOpCode2 = 0x81;
							inter.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				case 14:
					if (linkData.txBlock == false) {
						cycleTime = currentTimeSec + 10;	//Do next step in 10 seconds
						teststep += 1;
						tempInt = -20;			//Time zone offset in 15 minute blocks for EST
						utcTime.tz = tempInt & 0xff;
						utcTime.dst = 4;		//DST offset in 15 minute blocks EDT

						printf("Issue intermediate Set UTC time command\n");
						rtn = CEA2045inter(SET_UTC_TIME, 0);	//Issue an intermediate information request
						if (rtn == 0) {
							inter.respOpCode1 = SET_UTC_TIME;
							inter.respOpCode2 = 0x80;
							inter.timeoutTime = time(&currentTimeSec) + 4;
						}
					}
					else {
						cycleTime += 1;	//Wait 1 second and try again
					}
					break;
				default:
					// Print results of device information command
					printf("devInfo.respCode = %i\n",devInfo.respCode);
					memcpy(tempstr, &devInfo.ceaVer[0], 2);
					tempstr[2] = 0;
					printf("devInfo.modelNum = %s\n",tempstr);
					printf("devInfo.vendorID = %x\n",devInfo.vendorID);
					printf("devInfo.devType = %x\n",devInfo.devType);
					printf("devInfo.devRev = %x\n",devInfo.devRev);
					printf("devInfo.capBitmap = %x\n",devInfo.capBitmap);
					printf("devInfo.reserve = %i\n",devInfo.reserve);
					memcpy(tempstr, &devInfo.modelNum[0], 16);
					tempstr[16] = 0;
					printf("devInfo.modelNum = %s\n",tempstr);
					memcpy(tempstr, &devInfo.serialNum[0], 16);
					tempstr[16] = 0;
					printf("devInfo.serialNum = %s\n",tempstr);
					printf("devInfo.firmYear = %i\n",devInfo.firmYear);
					printf("devInfo.firmMonth = %i\n",devInfo.firmMonth);
					printf("devInfo.firmDay = %i\n",devInfo.firmDay);
					printf("devInfo.firmMajor = %i\n",devInfo.firmMajor);
					printf("devInfo.firmMinor = %i\n",devInfo.firmMinor);
					//Test routine complete
					STOP = true;
					break;
			}
		}

		//Process returned messages
		if (linkData.rxCount > 0) {
			if (rxBuffer[0] == 8) {
				// This is a basic or intermediate message
				if (rxBuffer[1] == 1) {
					// This is a basic message
					printf("Processing basic message\n");
					rtn = CEA2045basicRx();
					//================================================================================
					// This is where the applications decides what to do with the 
					// message contents. The last received opcode 1 and 2 are place in the basic 
					// structure. Typical responses would be:
					// rtn = 0x03 - Application Ack - No need to do anything else
					// rtn = 0x04 - Application Nak - error code in basic.err - Posibly resend message
					// rtn = 0x13 - Operational State - State returned in basic.state
					// rtn = 0x11 - Customer Override - Override status in basic.override
					// rtn = 0x14 - Sleep - UCM can go into low power mode if desired
					// rtn = 0x15 - Wake - UCM needs to update grid information and send to SGD
					//================================================================================
				}
				
				else {
					// This is an intermediate message rxBuffer[1] == 2
					printf("Processing intermediate message\n");
					rtn = CEA2045interRx();
					//================================================================================
					// Each intermediate message is a special structure. The call returns an ID of the 
					// message type which defines the strucure to use to view the message.
					//================================================================================
				}
			}	
			else if (rxBuffer[0] == 9) {
					// This is a pass through message type - rxBuffer[1] defines protocol
					// Simply pass the contents on to the sending grid device
			}
		}				
				
		//Process pending basic message operations
		if (linkData.txBlock == false && (basic.pendingAppAck == true || basic.pendingAppNak == true)) {
			if (basic.pendingAppAck == true) {
				printf("Issue Application Ack command\n");
				rtn = CEA2045basic(APP_ACK, basic.appAckSend, 0);	//Issue Application Ack reply
			}
			else {
				printf("Issue Application Nak command\n");
				rtn = CEA2045basic(APP_NAK, basic.appAckSend, 0);	//Issue Application Nak reply
			}
			basic.pendingAppAck = false;
			basic.pendingAppNak = false;
		}
		else if (basic.timeoutTime > 0 && basic.timeoutTime < time(&currentTimeSec)) {
			//We have timed out waiting for a response
			printf("Timed out waiting for a response\n");
			basic.appAckRtn = 0;
			linkData.rxCount = 0;
			basic.pendingOpState = false;
			basic.timeoutTime = 0;
		}
		
		usleep(1000);		// Sleep for 1ms to allow other threads to operate
	}
	//Close UART on exit
	close(cea2045_fd);
	nonblock(NB_DISABLE);
	return rtn;
}  //end of main
				
				
/**
 *==========================================================================
 * This function is called to process basic messages received from the SGD
 * Syntax is:
 * int rtnval = CEA2045basicRx();
 * @return command ID on success
 * @return -1 on invalid message received
 *==========================================================================
 */
int CEA2045basicRx()
{
	int rtn = 0;
	
	basic.opCode1 = rxBuffer[4];
	basic.opCode2 = rxBuffer[5];
	//Check if Application Ack is pending
	if (basic.appAckRtn > 0) {
		//Verify Application Ack value
		if (rxBuffer[4] == 3) {
			if (rxBuffer[5] == basic.appAckRtn) {
				//Proper application ack was returned
				rtn = rxBuffer[4];
				printf("Application ACK %i received\n", basic.appAckRtn);
			}
			else {
				//Wrong application ack was returned
				printf("Wrong Application ACK received\n");
				rtn = -1;
			}
		}
	}
	else if (basic.pendingOpState == true) {
		//Get returned operational state
		if (rxBuffer[4] == 0x13) {
			//Operational state was returned
			basic.opState =  rxBuffer[5];
			rtn = rxBuffer[4];
			printf("Operational state %i received\n", rxBuffer[5]);
		}
		else {
			printf("Operational state not received\n");
			rtn = -1;
		}
	}
	else if (rxBuffer[4] == CUST_OVERRIDE) {
		//Operator Override state received - rxBuffer[5] = 0 is override disabled, 1 is override enabled
		printf("Customer Override received: %i\n", rxBuffer[5]);
		if (rxBuffer[5] == 0) basic.overRide = false;
		else basic.overRide = true;
		//Send application Ack
		rtn = rxBuffer[4];
		basic.pendingAppAck = true;
		basic.appAckSend = CUST_OVERRIDE;
	}
	else if (rxBuffer[4] == SLEEP_STATE) {
		//Sleep command received from SGD
		basic.sleepState = true;
		printf("Sleep command received\n");
		//Send application Ack
		rtn = rxBuffer[4];
		basic.pendingAppAck = true;
		basic.appAckSend = SLEEP_STATE;
	}
	else if (rxBuffer[4] == WAKE_STATE) {
		//Wake command received from SGD
		basic.sleepState = false;
		printf("Wake command received\n");
		//Send application Ack
		rtn = rxBuffer[4];
		basic.pendingAppAck = true;
		basic.appAckSend = WAKE_STATE;
	}
	basic.appAckRtn = 0;
	linkData.rxCount = 0;
	basic.pendingOpState = false;
	basic.timeoutTime = 0;
 
	return rtn;
} 			

/**
 *==========================================================================
 * This function is called to process intermediate messages received from
 * the SGD
 * Syntax is:
 * int rtnval = CEA2045interRx();
 * @return command ID on success
 * @return -1 on invalid message received
 *==========================================================================
 */
int CEA2045interRx()
{
	int rtn = 0;
	int messageSize;
	
	//Check if response is pending
	if (inter.respOpCode1 != 0) {
		//Verify Response opcodes
		if (rxBuffer[4] == inter.respOpCode1 && rxBuffer[5] == inter.respOpCode2) {
			//This is a valid response
			// Calculate message size
			messageSize = rxBuffer[2] *256 + rxBuffer[3];
			if (rxBuffer[4] == INFO_REQ && rxBuffer[5] == 0x81) {
				//This is a device information response
				devInfo.respCode = rxBuffer[6];		//Response code
				devInfo.ceaVer[0] = rxBuffer[7];	//CEA-2045 version
				devInfo.ceaVer[1] = rxBuffer[8];	//CEA-2045 version
				devInfo.vendorID = rxBuffer[9] * 256 + rxBuffer[10];	//Vendor ID integer
				devInfo.devType = rxBuffer[11] * 256 + rxBuffer[12];	//Device type
				devInfo.devRev = rxBuffer[13] * 256 + rxBuffer[14];	//Device revision
				devInfo.capBitmap = ((unsigned long)rxBuffer[15] << 24) + ((unsigned long)rxBuffer[16] << 16) + ((unsigned long)rxBuffer[17] << 8) + rxBuffer[18];	//Capability bitmap
				devInfo.reserve = rxBuffer[19];		//Reserved
				if (messageSize >= 32) {
					memcpy(&devInfo.modelNum[0],&rxBuffer[20],16);	//Model number
				}
				if (messageSize >= 48) {
					memcpy(&devInfo.serialNum[0],&rxBuffer[36],16);	//Serial number
				}
				if (messageSize >= 53) {
					devInfo.firmYear = rxBuffer[52];	//Firmware year
					devInfo.firmMonth = rxBuffer[53];	//Firmware month
					devInfo.firmDay = rxBuffer[54];		//Firmware day
					devInfo.firmMajor = rxBuffer[55];	//Firmware major
					devInfo.firmMinor = rxBuffer[56];	//Firmware minor
				}
				inter.lastOpCode1 = inter.respOpCode1;
				inter.respOpCode1 = 0;
				inter.timeoutTime = 0;
				inter.completeTime = time(&currentTimeSec);
			}
			else if (rxBuffer[4] == SET_UTC_TIME && rxBuffer[5] == 0x80) {
				//This is a Set UTC Time response
				if (rxBuffer[6] == 0) {
					//Success
					inter.lastOpCode1 = inter.respOpCode1;
					inter.respOpCode1 = 0;
					inter.timeoutTime = 0;
					inter.completeTime = time(&currentTimeSec);
					utcTime.lastTime = currentTimeSec;		// Save the last time the UTC time was set
				}
			}
			linkData.rxCount = 0;
		}
	}
 
	return rtn;
} 			

/**
 *==========================================================================
 * This function is called to send an Intermediate message to the SGD
 * Syntax is:
 * int rtnval = CEA2045basic(byte opcode1,byte opcode2);
 * @return 0 on success
 * @return 1 on failure to interpret a value
 * @return 2 on routine called when blocked
 *==========================================================================
 */
int CEA2045inter(unsigned char opcode1,unsigned char opcode2)
{
	bool valid;
	int rtnval;
	time_t tempTime;
	
	if (linkData.txBlock == true) {
		return 2;		//Routine called when blocked	
	}
	valid = true;
	rtnval = 0;
	switch(opcode1) {
		case INFO_REQ:
			txBuffer[3] = 0x02;
			linkData.txCount = 6;
			break;
		case SET_UTC_TIME:
			txBuffer[3] = 0x08;
			tempTime = time(&currentTimeSec) - 946684800;	// Convert unix time to seconds since 1/1/2000
			txBuffer[6] = (tempTime & 0xff000000) >> 24;	// UTC time
			txBuffer[7] = (tempTime & 0xff0000) >> 16;
			txBuffer[8] = (tempTime & 0xff00) >> 8;
			txBuffer[9] = tempTime & 0xff;
			txBuffer[10] = utcTime.tz;	//Time zone offset in 15 minute blocks
			txBuffer[11] = utcTime.dst;	//DST offset in 15 minute blocks
			linkData.txCount = 12;
			break;
		default:
			valid = false;
			break;
	}
	if (valid == true) {
		expectedMsgSize = 2;		//Set size of expected message
		txBuffer[0] = 0x08;			//Message type
		txBuffer[1] = 0x02;
		txBuffer[2] = 0x00;			//Payload length
		txBuffer[4] = opcode1;		//Message opcode
		txBuffer[5] = opcode2;		//Message operand
	}
	return rtnval;

}

/**
 *==========================================================================
 * This function is called to send a Basic message to the SGD
 * Syntax is:
 * int rtnval = CEA2045basic(byte COMMAND,int cmdparam);
 * @return 0 on success
 * @return 1 on failure to interpret a value
 * @return 2 on routine called when blocked
 *==========================================================================
 */
int CEA2045basic(unsigned char cmd,long cmdparam, float cmdparamf)
{
	unsigned char opcode1 = 0;		// opcode1
	unsigned char opcode2 = 0;		// opcode2
	long tempval;
	bool valid;
	int rtnval;

	if (linkData.txBlock == true) {
		return 2;		//Routine called when blocked	
	}
	valid = true;
	rtnval = 0;
	switch(cmd) {
		case SHED:
		case TIME_REMAINING_PRICE:
		case CPP:
		case GRID_EMERGENCY:
		case LOADUP:
			opcode1 = cmd;
			tempval = sqrt(cmdparam/2);
			if (tempval > 0xff) opcode2 = 0xff;
			else opcode2 = tempval;
			if (cmd != TIME_REMAINING_PRICE) {
				shedEndTime = time(&currentTimeSec) + cmdparam;
			} 
			break;
		case END_SHED:		
			opcode1 = cmd;
			opcode2 = 0;
			shedEndTime = 0;
			break;
		case COMM_STATUS:		
		case OPER_STATE_REQ:		
		case SIMPLE_TIME_SYNC:		
		case APP_ACK:
		case APP_NAK:
		case GRID_GUIDANCE:
			opcode1 = cmd;
			opcode2 = cmdparam;
			break;
		case PRESENT_RELATIVE_PRICE:
		case NEXT_RELATIVE_PRICE:
			opcode1 = cmd;
			opcode2 = calcRelativePriceByte(cmdparamf);
			break;
		case POWER_LEVEL:
			opcode1 = cmd;
			if (cmdparam > 0) {		// Power absorbed
				if (cmdparam > 127) {
					opcode2 = 127;
				}
			}
			else {					// Power produced
				cmdparam = -cmdparam;
				if (cmdparam > 127) {
					opcode2 = 127;
				}
				opcode2 = opcode2 || 0x80;
			} 			
			break;
		default:
			valid = false;
			break;
	}	
	if (valid == true) {
//		linkData.txBlock = false;	//Prevent any new messages until this completes	
		expectedMsgSize = 2;		//Set size of expected message
		txBuffer[0] = 0x08;			//Message type
		txBuffer[1] = 0x01;
		txBuffer[2] = 0x00;			//Payload length (always 2)
		txBuffer[3] = 0x02; 
		txBuffer[4] = opcode1;		//Message opcode
		txBuffer[5] = opcode2;		//Message operand
		linkData.txCount = 6;
	}
	return rtnval;
}

/**
 *==========================================================================
 * Link layer functionality. Trigger this routine every millisecond or so.
 * If desired this routine and its supporting subroutines can be made a 
 * separate process, thread or interrupt routine. Data could then be passed
 * via shared memory, pipe, etc.
 * @return 0 on success
 * @return 1 on link ack received
 * @return -1 on link nak - nak value in linkData.nakVal
 * @return -2 on link ack/nak timeout
 *==========================================================================
 */
int linkLayer()
{
	int newChars;

	
	if (linkData.sunSpec == false) {
		// This is a normal CEA2045 SGD device
		//Get any new characters from SGD
		if (linkData.rxCount == 0) {		// Only check for new received data if app layer is done with last message
//			printf("linkData.rxCount = %i\n", linkData.rxCount);
			// Check for received data 
			newChars = read(cea2045_fd, &rxBuffer[rxCount], sizeof(rxBuffer - rxCount));
			if (newChars > 0) {
				printf("%i bytes read\n", newChars);
				rxCount += newChars;
				if (pendingLinkAck == true) {
					//If waiting for link ack
					if (rxCount > 1) {
						//At least 2 characters have been received so check for link ack/nak
						addTimeOffset(&txDelayTime, 100000);
						linkData.txBlock = true;
						pendingLinkAck = false;
						expectedMsgSize = 0;
						rxCount = 0;
						txStartTime.tv_sec = 0;
						if (rxBuffer[0] == 6 && rxBuffer[1] == 0) {
							//Link ack received
							printf("Received Link Ack\n");
							linkData.nakVal = -1;
							return 1;
						}
						else if (rxBuffer[0] == 0x15) {
							//Link nak received
							linkData.nakVal = rxBuffer[1];
							return -1;
						}
					}
					
				}
				else {
					if (rxCount == newChars) {
						//Start of a received message
						gettimeofday(&rxStartTime, NULL);
					}
					if (expectedMsgSize == 0) {
						//Message size has not been determined
						if ((rxBuffer[0] == 0x08 || rxBuffer[0] == 0x09) && rxCount > 3) {
							// enough characters have been received to calculate message size
							expectedMsgSize = (rxBuffer[2] * 256) + rxBuffer[3] + 6;
							printf("%i Expected message size\n", expectedMsgSize);
						}
					}
					if (expectedMsgSize > 0 && rxCount >= expectedMsgSize) {
						// Complete message received - validate checksum
						printf("Complete message received\n");
						rxStartTime.tv_sec = 0;
						if (calcChecksum(rxBuffer, expectedMsgSize - 2, true) == 0) {
//							printf("Got here 1\n");
							linkAckBuff[0] = 0x06;
							linkAckBuff[1] = 0x0;
							linkData.rxCount = expectedMsgSize -2;
							rxCount = 0;
							expectedMsgSize = 0;
							addTimeOffset(&pendingTxTime, 40000);		//send ack in 40ms
							pendingTxLinkAck = true;
							linkData.txBlock = true;
						}
						else {
							linkAckBuff[0] = 0x15;
							linkAckBuff[1] = 0x03;			// Nak for bad checksum
							rxCount = 0;					// Flush receive buffer
							expectedMsgSize = 0;
							addTimeOffset(&pendingTxTime, 40000);		//send ack in 40ms
							pendingTxLinkAck = true;
							linkData.txBlock = true;
						}
						//===============================================
						// add additional code for other link errors
						//===============================================
					}
				}
			}
			else if (rxStartTime.tv_sec > 0){
				//Check for timeout on message
				if (calcTimeDiff(&rxStartTime) > 100) {
					//It has been over 100ms since start of message so nak message
					rxStartTime.tv_sec = 0;
					linkAckBuff[0] = 0x15;
					linkAckBuff[1] = 0x05;			// Nak for message timeout
					rxCount = 0;					// Flush receive buffer
					expectedMsgSize = 0;
					write(cea2045_fd, &linkAckBuff[0], 2);	// Write a link nak to the SGD
					addTimeOffset(&txDelayTime, 100000);		//Don't send messages for 100ms
					linkData.txBlock = true;
				}
			}
			else if (pendingLinkAck == true) {
				//Check for timeout waiting on link ack/nak
				if (calcTimeDiff(&txStartTime) > 120) {
					//It has been over 120ms since message was sent so timeout
					txStartTime.tv_sec = 0;
					pendingLinkAck = false;
					rxCount = 0;					// Flush receive buffer
					expectedMsgSize = 0;
					linkData.txBlock = false;
					return -2;
				}
			}
			if (pendingTxLinkAck == true) {
//				printf("Got here 2\n");
				//Check if it is time to send pending link ack/nak
				if (calcTimeDiff(&pendingTxTime) >= 0) {
					printf("Sending Link Ack/Nak\n");
					write(cea2045_fd, &linkAckBuff[0], 2);	// Write a link nak to the SGD
					pendingTxLinkAck = false;
					addTimeOffset(&txDelayTime, 100000);		//Don't send messages for 100ms
					linkData.txBlock = true;
				} 
			}
		}
		if (linkData.txBlock == true) {
			//Check if txDelayTime has expired
			if (calcTimeDiff(&txDelayTime) >= 0) {
				txDelayTime.tv_sec = 0;		//Clear the block on transmitting messages
				printf("Clearing txBlock\n");
				linkData.txBlock = false;
			}
		}
		if (linkData.txBlock == false && linkData.txCount > 0) {
			// Transmit data to SGD
			if (txStartTime.tv_sec == 0) {
				// This is a new message to be sent
				calcChecksum(txBuffer, linkData.txCount, false);
				gettimeofday(&txStartTime, NULL);
				write(cea2045_fd, &txBuffer[0], linkData.txCount + 2);	// Write data in txBuffer to the CEA 2045 port
				addTimeOffset(&txDelayTime, 150000);		//Don't send messages for 150ms or until Link ack received
				linkData.txBlock = true;
				linkData.txCount = 0;
				pendingLinkAck = true;
				expectedMsgSize = 2;		//Set size of expected message
			}
		}
	}
	else {
		// Put SunSpec code here for CSI project
	}
	return 0;
}

/**
 ***************************************************************************
 * This function calculates time in milliseconds from passed value to now. 
 * A negative number says the passed time is still in the future.
 * A positive number says the passed time is still in the past.
 *
 * @param compareTime  A pointer to a timeval structure.
 * @return long int number of milliseconds
 ***************************************************************************
 */
long int calcTimeDiff(struct timeval *compareTime) {
long int t1;

	gettimeofday(&currentTime, NULL);
	t1 = (currentTime.tv_sec - compareTime->tv_sec) * 1000 + ((currentTime.tv_usec - compareTime->tv_usec) / 1000);
	return t1;
}

/**
 ******************************************************************************
 * This function adds a passed number of microseconds to now in timeval struct 
 *
 * @param timestruct  A pointer to a timeval structure
 * @param adder  Number of micriseconds to add to current time.
 * @param validate True if this is a checksum test, false otherwise.
 ******************************************************************************
 */
void addTimeOffset(struct timeval *timestruct, long int adder) {

	gettimeofday(timestruct, NULL);
	timestruct->tv_usec += adder;
	while (timestruct->tv_usec > 999999) {
		timestruct->tv_sec += 1;
		timestruct->tv_usec -= 1000000;
	}
}

/**
 ******************************************************************************
 * This calculates the byte value for the relative price function formula:
 * Byte value = (-62 + SQRT(4096 + (32768*RP))) / 2 
 *
 * @param relativePrice float ratio of current price / normal price
 ******************************************************************************
 */

unsigned char calcRelativePriceByte(float relativePrice) {
	float f1;
	
	if (relativePrice > 9.825) {
		return 0xff;
	}
	else if (relativePrice < 0) {
		return 0x0;
	}
	f1 = (-62 + sqrt(4096 + (32768 * relativePrice))) / 2;
	return (int) (f1+.05);

}

/**
 ***************************************************************************
 * This function calculates the check sum bytes for the passed basic serial 
 * message and attaches it to the end of the message. 
 *
 * @param buffData  A pointer to the message string string.
 * @param msgSize  Size of the message without checksum.
 * @param validate True if this is a checksum test, false otherwise.
 * @return 0 on success
 * @return -1 if checksum fails validation
 ***************************************************************************
 */
int calcChecksum(unsigned char* buffData, int msgSize, bool validate) {
	// Message is in array buffData
	int i;
	int checksum1 = 0xaa;
	int checksum2 = 0;

	for (i = 0; i < msgSize; i++) {
		checksum1 += buffData[i];
		checksum1 = checksum1 % 0xff;
		checksum2 += checksum1;
		checksum2 = checksum2 % 0xff;
	}
	if (validate != true) {
		// Copy checksum to end of buffer
		buffData[msgSize] = 255 - ((checksum1 + checksum2) % 255);
		buffData[msgSize + 1] = 255 - ((checksum1 + buffData[msgSize]) % 255);
		return 0;
	}
	else {
		// Verify the last two characters of buffer match the checksum calculated
		if (buffData[msgSize] == 255 - ((checksum1 + checksum2) % 255)) {
			if (buffData[msgSize + 1] == 255 - ((checksum1 + buffData[msgSize]) % 255)) {
				return 0;
			}
		}
		return -1;
	}
}

/**
 ***************************************************************************
 * Initialize the serial port 
 *
 * @return file descriptor for com port
 ***************************************************************************
 */
int initSerialPort()
{
	int tty_fd = -1;
	
	tty_fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (tty_fd == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}
	
	tcgetattr(tty_fd, &tio);
	tio.c_cflag = B19200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	tio.c_iflag = IGNPAR;
	tio.c_oflag = 0;
	tio.c_lflag = 0;
	tcflush(tty_fd, TCIFLUSH);
	tcsetattr(tty_fd, TCSANOW, &tio);

	return tty_fd;
}

/**
 ***************************************************************************
 * This reads the keyboard input without blocking. Enter must be pressed 
 * to get input. 
 *
 * @return number of characters read
 ***************************************************************************
 */
int kbhit()
{
	struct timeval tv;
	fd_set fds;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
	select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &fds);
}

/**
 ***************************************************************************
 * This configures keyboard for non-blocking 
 *
 * @param state  A pointer to the message string string.
 ***************************************************************************
 */
void nonblock(int state)
{
	struct termios ttystate;
	
	//get the terminal state
	tcgetattr(STDIN_FILENO, &ttystate);
	if (state==NB_ENABLE) {
		//turn off canonical mode
		ttystate.c_lflag &= ~ICANON;
		//minimum of number input read.
		ttystate.c_cc[VMIN] = 1;
	}
	else if (state==NB_DISABLE) {
		//turn on canonical mode
		ttystate.c_lflag |= ICANON;
	}
	//set the terminal attributes.
	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

}
