
SRC_DIR = .
LIB_DIR = ./lib
OBJ_DIR = .
INCLUDE_DIR = ./include

CFLAGS = -Wall
DEBUG_CFLAGS = -O0 -g

INCLUDES = \
	-I $(INCLUDE_DIR)

SOURCES = \
	$(SRC_DIR)/ezxml.c \
	$(SRC_DIR)/sunspec.c \
	$(SRC_DIR)/sunspec_device.c \
	$(SRC_DIR)/sunspec_modbus.c \
	$(SRC_DIR)/sunspec_modbus_rtu.c \
	$(SRC_DIR)/sunspec_modbus_sim.c \
	$(SRC_DIR)/sunspec_value.c \
	$(SRC_DIR)/sunspec_log.c \
	$(SRC_DIR)/sunspec_cea2045.c

OBJS = \
	$(SRC_DIR)/ezxml.o \
	$(SRC_DIR)/sunspec.o \
	$(SRC_DIR)/sunspec_device.o \
	$(SRC_DIR)/sunspec_modbus.o \
	$(SRC_DIR)/sunspec_modbus_rtu.o \
	$(SRC_DIR)/sunspec_modbus_sim.o \
	$(SRC_DIR)/sunspec_value.o \
	$(OBJ_DIR)/sunspec_log.o \
	$(SRC_DIR)/sunspec_cea2045.o

LIB = $(LIB_DIR)/libsunspec.a

CFLAGS += $(INCLUDES) $(DEBUG_CFLAGS)

all: lib

lib: $(OBJS)
	$(AR) rcs $(LIB) $(OBJS)
	$(RM) $(OBJS)

test: all
	$(MAKE) -C test test

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

install: all

clean:
	$(RM) $(OBJS) $(LIB) *~


