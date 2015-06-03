
/*
 * Copyright (C) 2014 SunSpec Alliance
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>

#include "sunspec_cea2045.h"
#include "sunspec_error.h"
#include "sunspec_io.h"
#include "sunspec_modbus.h"

typedef struct _suns_modbus_rtu_t {
    char *ifc_name;
    uint32_t baudrate;
    uint8_t parity;
    uint16_t slave_id;
    suns_io_t io;
} suns_modbus_rtu_t;

uint16_t
suns_modbus_crc16(const unsigned char *data, int16_t len)
{
    static const unsigned short crc_table[] = {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
        0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
        0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
        0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
        0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
        0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
        0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
        0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
        0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
        0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
        0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
        0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
        0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
        0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
        0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
        0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
        0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
        0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
        0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};

    uint16_t temp;
    uint16_t crc = 0xFFFF;
    const unsigned char *buf = data;

    while (len--) {
      temp = (*buf++ ^ crc) & 0xff;
      crc >>= 8;
      crc ^= crc_table[temp];
    }

    return crc;
}

#define SUNS_MODBUS_HDR_LEN             3
#define SUNS_MODBUS_CRC_LEN             2
#define SUNS_MODBUS_BUF_SIZE            512
#define SUNS_MODBUS_HOLDING_READ        3
#define SUNS_MODBUS_WRITE               16
#define SUNS_MODBUS_REQ_TIMEOUT         1000    /* default timeout in ms */

#define SUNS_MODBUS_RSP_ID              0
#define SUNS_MODBUS_RSP_FUNC            1
#define SUNS_MODBUS_RSP_LEN             2
#define SUNS_MODBUS_RSP_EXCEPT          2
#define SUNS_MODBUS_RSP_DATA            3 
#define SUNS_MODBUS_RSP_EXCEPT_CODE     0x80
#define SUNS_MODBUS_RSP_WRITE_ADDR      2
#define SUNS_MODBUS_RSP_WRITE_COUNT     4
#define SUNS_MODBUS_RSP_WRITE_DATA_LEN  3
#define SUNS_MODBUS_REQ_COUNT_MAX       125

suns_err_t
suns_modbus_rtu_connect(void *prot, uint32_t timeout)
{
    if (prot == NULL) {
        return SUNS_ERR_INIT;
    }

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_rtu_disconnect(void *prot)
{
    if (prot == NULL) {
        return SUNS_ERR_INIT;
    }

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_rtu_read_block(suns_modbus_rtu_t *prot, uint16_t addr, unsigned char *buf, uint16_t count, uint32_t timeout)
{
    suns_err_t ret;
    uint16_t len;
    uint16_t data_len;
    unsigned char req_buf[SUNS_MODBUS_BUF_SIZE];
    unsigned char resp_buf[SUNS_MODBUS_BUF_SIZE];
    uint16_t min_len = (SUNS_MODBUS_HDR_LEN + SUNS_MODBUS_CRC_LEN);
    uint16_t resp_len = min_len;
    uint16_t index = 0;
    uint16_t crc;
    uint16_t resp_crc;
    uint16_t crc_offset;
    uint16_t i;
    char exception = 0;

    /* flush tx/rx */
    prot->io.flush(prot->io.prot, true, true);

    req_buf[index++] = prot->slave_id;
    req_buf[index++] = SUNS_MODBUS_HOLDING_READ;
    suns_modbus_from_16(addr, &req_buf[index]);
    index += 2;
    suns_modbus_from_16(count, &req_buf[index]);
    index += 2;
    crc = suns_modbus_crc16(req_buf, (int16_t) index);
    req_buf[index++] = crc & 0xff;
    req_buf[index++] = (crc >> 8) & 0xff;

    /* send request */
    if ((ret = prot->io.write(prot->io.prot, req_buf, index, timeout)) != SUNS_ERR_OK) {
        return ret;
    }

    /* read response */
    index = 0;
    while (index < resp_len) {
        len = SUNS_MODBUS_BUF_SIZE - index;
        if ((ret = prot->io.read(prot->io.prot, &resp_buf[index], &len, timeout)) != SUNS_ERR_OK) {
            return ret;
        }
        index += len;

        if (min_len && index >= min_len) {
            min_len = 0;
            /* validate length */
            if (resp_buf[SUNS_MODBUS_RSP_FUNC] & SUNS_MODBUS_RSP_EXCEPT_CODE) {
                exception = resp_buf[SUNS_MODBUS_RSP_EXCEPT];
                data_len = 0;
            } else {
                data_len = resp_buf[SUNS_MODBUS_RSP_LEN];
                resp_len += data_len;
            }
        }
    }

    /* validate cksum */
    crc_offset = resp_len - 2;
    crc = suns_modbus_crc16(resp_buf, crc_offset);
    resp_crc = ((unsigned char) resp_buf[crc_offset]) |
               (((unsigned char) resp_buf[crc_offset + 1]) << 8);

    if (crc == resp_crc) {
        if (exception) {
            return SUNS_ERR_MODBUS_EXCEPT;
        } else {
            if ((resp_buf[SUNS_MODBUS_RSP_ID] != prot->slave_id) || 
                (resp_buf[SUNS_MODBUS_RSP_FUNC] != SUNS_MODBUS_HOLDING_READ)) {
                return SUNS_ERR_MODBUS_RESP;
            }
        }
    } else {
        return SUNS_ERR_MODBUS_CRC;
    }

    /* move data to buffer */
    for (i = 0; i < data_len; i++) {
        buf[i] = resp_buf[i + SUNS_MODBUS_RSP_DATA];
    }

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_rtu_read(void *prot, uint16_t addr, uint16_t count, unsigned char *buf, uint32_t timeout)
{
    uint16_t count_remaining = count;
    uint16_t req_addr = addr;
    uint16_t req_count;
    int index = 0;
    int ret;
    uint32_t req_timeout = SUNS_MODBUS_REQ_TIMEOUT;

    if (timeout != 0) {
        req_timeout = timeout;
    }

    while (count_remaining) {
        if (count_remaining > SUNS_MODBUS_REQ_COUNT_MAX) {
            req_count = SUNS_MODBUS_REQ_COUNT_MAX;
        } else {
            req_count = count_remaining;
        }

        if ((ret = suns_modbus_rtu_read_block((suns_modbus_rtu_t *) prot, req_addr, &buf[index], req_count, req_timeout)) != SUNS_ERR_OK) {
            return ret;
        }

        index += req_count * 2;
        req_addr += req_count;
        count_remaining -= req_count;
    }

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_rtu_write(void *prot, uint16_t addr, uint16_t count, unsigned char *buf, uint32_t timeout)
{
    suns_err_t ret;
    uint16_t len;
    uint16_t data_len;
    unsigned char req_buf[SUNS_MODBUS_BUF_SIZE];
    unsigned char resp_buf[SUNS_MODBUS_BUF_SIZE];
    uint16_t min_len = (SUNS_MODBUS_HDR_LEN + SUNS_MODBUS_CRC_LEN);
    uint16_t resp_len = min_len;
    uint16_t index = 0;
    uint16_t crc;
    uint16_t resp_crc;
    uint16_t crc_offset;
    uint16_t byte_count;
    uint16_t i;
    char exception = 0;
    suns_modbus_rtu_t *rtu_prot = (suns_modbus_rtu_t *) prot;
    uint32_t req_timeout = SUNS_MODBUS_REQ_TIMEOUT;

    if (timeout != 0) {
        req_timeout = timeout;
    }

    if (count > SUNS_MODBUS_REQ_COUNT_MAX) {
        return SUNS_ERR_BUF_SIZE;
    }

    /* flush tx/rx */
    rtu_prot->io.flush(rtu_prot->io.prot, true, true);

    req_buf[index++] = rtu_prot->slave_id;
    req_buf[index++] = SUNS_MODBUS_WRITE;
    suns_modbus_from_16(addr, &req_buf[index]);
    index += 2;
    byte_count = count * 2;
    suns_modbus_from_16(count, &req_buf[index]);
    index += 2;
    req_buf[index++] = byte_count;
    /* add data to request */
    for (i = 0; i < byte_count; i++) {
        req_buf[index++] = buf[i];
    }
    crc = suns_modbus_crc16(req_buf, (int16_t) index);
    req_buf[index++] = crc & 0xff;
    req_buf[index++] = (crc >> 8) & 0xff;

    /* send request */
    if ((ret = rtu_prot->io.write(rtu_prot->io.prot, req_buf, index, req_timeout)) != SUNS_ERR_OK) {
        return ret;
    }

    /* read response */
    index = 0;
    while (index < resp_len) {
        len = SUNS_MODBUS_BUF_SIZE - index;
        if ((ret = rtu_prot->io.read(rtu_prot->io.prot, &resp_buf[index], &len, req_timeout)) != SUNS_ERR_OK) {
            return ret;
        }
        index += len;

        if (min_len && index >= min_len) {
            min_len = 0;
            /* validate length */
            if (resp_buf[SUNS_MODBUS_RSP_FUNC] & SUNS_MODBUS_RSP_EXCEPT_CODE) {
                exception = resp_buf[SUNS_MODBUS_RSP_EXCEPT];
                data_len = 0;
            } else {
                data_len = SUNS_MODBUS_RSP_WRITE_DATA_LEN;
                resp_len += data_len;
            }
        }
    }

    /* validate cksum */
    crc_offset = resp_len - 2;
    crc = suns_modbus_crc16(resp_buf, crc_offset);
    resp_crc = ((unsigned char) resp_buf[crc_offset]) |
               (((unsigned char) resp_buf[crc_offset + 1]) << 8);

    if (crc == resp_crc) {
        if (exception) {
            return SUNS_ERR_MODBUS_EXCEPT;
        } else {
            if ((resp_buf[SUNS_MODBUS_RSP_ID] != rtu_prot->slave_id) || 
                (resp_buf[SUNS_MODBUS_RSP_FUNC] != SUNS_MODBUS_WRITE) ||
                (suns_modbus_to_16(&resp_buf[SUNS_MODBUS_RSP_WRITE_ADDR]) != addr) ||
                (suns_modbus_to_16(&resp_buf[SUNS_MODBUS_RSP_WRITE_COUNT]) != count)) {
                return SUNS_ERR_MODBUS_RESP;
            }
        }
    } else {
        return SUNS_ERR_MODBUS_CRC;
    }

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_rtu_close(suns_modbus_io_t *io)
{
    if (io == NULL) {
        return SUNS_ERR_INIT;
    }

    if (io->prot != NULL) {
        free(io->prot);
    }

    io->prot = NULL;
    io->connect = NULL;
    io->disconnect = NULL;
    io->read = NULL;
    io->write = NULL;
    io->close = NULL;

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_rtu_cea2045_open(suns_modbus_io_t *io, uint16_t slave_id)
{
    suns_err_t err;

    if (io == NULL) {
        return SUNS_ERR_INIT;
    }

    if ((io->prot = calloc(1, sizeof(suns_modbus_rtu_t))) == NULL) {
        return SUNS_ERR_ALLOC;
    }
    ((suns_modbus_rtu_t *) io->prot)->slave_id = slave_id;

    /* initialize cea2045 layer */
    if ((err = suns_cea2045_open(&((suns_modbus_rtu_t *) io->prot)->io)) != SUNS_ERR_OK) {
        return err;
    }

    io->connect = suns_modbus_rtu_connect;
    io->disconnect = suns_modbus_rtu_disconnect;
    io->read = suns_modbus_rtu_read;
    io->write = suns_modbus_rtu_write;
    io->close = suns_modbus_rtu_close;

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_rtu_serial_open(suns_modbus_io_t *io, char *ifc_name,
                            uint16_t slave_id, uint32_t baudrate, uint8_t parity)
{
    if (io == NULL) {
        return SUNS_ERR_INIT;
    }

    if ((io->prot = malloc(sizeof(suns_modbus_rtu_t))) == NULL) {
        return SUNS_ERR_ALLOC;
    }
    ((suns_modbus_rtu_t *) io->prot)->slave_id = slave_id;
    ((suns_modbus_rtu_t *) io->prot)->ifc_name = ifc_name;
    ((suns_modbus_rtu_t *) io->prot)->baudrate = baudrate;
    ((suns_modbus_rtu_t *) io->prot)->parity = parity;
    ((suns_modbus_rtu_t *) io->prot)->slave_id = slave_id;

    /* ### init serial interface here */

    io->connect = suns_modbus_rtu_connect;
    io->disconnect = suns_modbus_rtu_disconnect;
    io->read = suns_modbus_rtu_read;
    io->write = suns_modbus_rtu_write;
    io->close = suns_modbus_rtu_close;

    return SUNS_ERR_OK;
}
