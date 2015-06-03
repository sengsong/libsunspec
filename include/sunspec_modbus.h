
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

#ifndef _SUNSPEC_MODBUS_H_
#define _SUNSPEC_MODBUS_H_

#include <stdint.h>

#include "sunspec_error.h"
#include "sunspec_value.h"

struct _suns_modbus_io_t;

/* modbus interface functions */
typedef suns_err_t(*suns_modbus_connect_func_t)(void *prot, uint32_t timeout);
typedef suns_err_t(*suns_modbus_disconnect_func_t)(void *prot);
typedef suns_err_t(*suns_modbus_read_func_t)(void *prot, uint16_t addr, uint16_t count, unsigned char *buf, uint32_t timeout);
typedef suns_err_t(*suns_modbus_write_func_t)(void *prot, uint16_t addr, uint16_t count, unsigned char *buf, uint32_t timeout);
typedef suns_err_t(*suns_modbus_close_func_t)(struct _suns_modbus_io_t *io);

#define SUNS_MODBUS_IO_MAGIC    0x28945613

typedef struct _suns_modbus_io_t {
    uint32_t magic;
    suns_modbus_connect_func_t connect;
    suns_modbus_disconnect_func_t disconnect;
    suns_modbus_read_func_t read;
    suns_modbus_write_func_t write;
    suns_modbus_close_func_t close;
    void *prot;
} suns_modbus_io_t;

#ifdef __cplusplus
extern "C" {
#endif

uint16_t suns_modbus_to_16(const unsigned char *buf);
uint32_t suns_modbus_to_32(const unsigned char *buf);
uint64_t suns_modbus_to_64(const unsigned char *buf);
void suns_modbus_from_16(uint16_t val, unsigned char *buf);
void suns_modbus_from_32(uint32_t val, unsigned char *buf);
void suns_modbus_from_64(uint64_t val, unsigned char *buf);

void suns_modbus_to_int16(unsigned char *buf, suns_value_t *value, uint16_t len);
void suns_modbus_to_uint16(unsigned char *buf, suns_value_t *value, uint16_t len);
void suns_modbus_to_int32(unsigned char *buf, suns_value_t *value, uint16_t len);
void suns_modbus_to_uint32(unsigned char *buf, suns_value_t *value, uint16_t len);
void suns_modbus_to_int64(unsigned char *buf, suns_value_t *value, uint16_t len);
void suns_modbus_to_uint64(unsigned char *buf, suns_value_t *value, uint16_t len);
void suns_modbus_to_float(unsigned char *buf, suns_value_t *value, uint16_t len);
void suns_modbus_to_double(unsigned char *buf, suns_value_t *value, uint16_t len);
void suns_modbus_to_str(unsigned char *buf, suns_value_t *value, uint16_t len);
void suns_modbus_from_int16(unsigned char *buf, suns_value_t value, uint16_t len);
void suns_modbus_from_uint16(unsigned char *buf, suns_value_t value, uint16_t len);
void suns_modbus_from_int32(unsigned char *buf, suns_value_t value, uint16_t len);
void suns_modbus_from_uint32(unsigned char *buf, suns_value_t value, uint16_t len);
void suns_modbus_from_int64(unsigned char *buf, suns_value_t value, uint16_t len);
void suns_modbus_from_uint64(unsigned char *buf, suns_value_t value, uint16_t len);
void suns_modbus_from_float(unsigned char *buf, suns_value_t value, uint16_t len);
void suns_modbus_from_double(unsigned char *buf, suns_value_t value, uint16_t len);
void suns_modbus_from_str(unsigned char *buf, suns_value_t value, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* _SUNSPEC_MODBUS_H_ */
