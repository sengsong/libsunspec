
SRC_DIR = ..
INC_DIR = ../include
LIB_DIR = ../lib
TST_DIR = .
OBJ_DIR = .

CFLAGS = -Wall
DEBUG_CFLAGS = -O0 -g

INCLUDES = \
	-I $(INC_DIR) \
        -I $(SRC_DIR)

SOURCES = \
	$(TST_DIR)/inverter.c \
	$(TST_DIR)/inverter_example.c \
	$(TST_DIR)/cea2045.c \
	$(TST_DIR)/test_sunspec.c \
	$(TST_DIR)/test_inverter.c \
	$(TST_DIR)/AllTests.c \
	$(TST_DIR)/CuTest.c
OBJS = \
	$(TST_DIR)/inverter.o \
	$(TST_DIR)/inverter_example.o \
	$(TST_DIR)/cea2045.o \
	$(TST_DIR)/test_sunspec.o \
	$(TST_DIR)/test_inverter.o \
	$(TST_DIR)/AllTests.o \
	$(TST_DIR)/CuTest.o \

LIBS = \
	-L $(LIB_DIR) -lsunspec

BIN = $(TST_DIR)/AllTests

CFLAGS += $(INCLUDES)

all: bin

bin: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LIBS) 
	$(RM) $(OBJS)

test: all
	./AllTests

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(LIB) *~
