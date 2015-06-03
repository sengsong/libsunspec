
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

#include <stdint.h>
#include <string.h>

#include "sunspec_error.h"
#include "sunspec_value.h"

typedef union {
    int16_t s;
    uint16_t u;
} suns_modbus_v16_t;

typedef union {
    int32_t s;
    uint32_t u;
    float f;
} suns_modbus_v32_t;

typedef union {
    int64_t s;
    uint64_t u;
    double d;
} suns_modbus_v64_t;

uint16_t
suns_modbus_to_16(const unsigned char *buf)
{
  return (((uint32_t)((unsigned char) buf[0])) << (1*8)) |
          ((uint32_t)((unsigned char) buf[1]));
}

uint32_t
suns_modbus_to_32(const unsigned char *buf)
{
  return (((uint32_t)((unsigned char) buf[0])) << (3*8)) |
         (((uint32_t)((unsigned char) buf[1])) << (2*8)) |
         (((uint32_t)((unsigned char) buf[2])) << (1*8)) |
          ((uint32_t)((unsigned char) buf[3]));
}

uint64_t
suns_modbus_to_64(const unsigned char *buf)
{
  return (((uint64_t)((unsigned char) buf[0])) << (7*8)) |
         (((uint64_t)((unsigned char) buf[1])) << (6*8)) |
         (((uint64_t)((unsigned char) buf[2])) << (5*8)) |
         (((uint64_t)((unsigned char) buf[3])) << (4*8)) |
         (((uint64_t)((unsigned char) buf[4])) << (3*8)) |
         (((uint64_t)((unsigned char) buf[5])) << (2*8)) |
         (((uint64_t)((unsigned char) buf[6])) << (1*8)) |
          ((uint64_t)((unsigned char) buf[7]));
}

void
suns_modbus_from_16(uint16_t val, unsigned char *buf)
{
    buf[0] = (unsigned char)((val >> (1*8)) & 0xff);
    buf[1] = (unsigned char)(val & 0xff);
}

void
suns_modbus_from_32(uint32_t val, unsigned char *buf)
{
    buf[0] = (unsigned char)((val >> (3*8)) & 0xff);
    buf[1] = (unsigned char)((val >> (2*8)) & 0xff);
    buf[2] = (unsigned char)((val >> (1*8)) & 0xff);
    buf[3] = (unsigned char)(val & 0xff);
}

void
suns_modbus_from_64(uint64_t val, unsigned char *buf)
{
    buf[0] = (unsigned char)((val >> (7*8)) & 0xff);
    buf[1] = (unsigned char)((val >> (6*8)) & 0xff);
    buf[2] = (unsigned char)((val >> (5*8)) & 0xff);
    buf[3] = (unsigned char)((val >> (4*8)) & 0xff);
    buf[4] = (unsigned char)((val >> (3*8)) & 0xff);
    buf[5] = (unsigned char)((val >> (2*8)) & 0xff);
    buf[6] = (unsigned char)((val >> (1*8)) & 0xff);
    buf[7] = (unsigned char)(val & 0xff);
}

void
suns_modbus_to_int16(unsigned char *buf, suns_value_t *value, uint16_t len)
{
    suns_modbus_v16_t v;

    v.u = suns_modbus_to_16(buf);
    (*value).s16 = v.s;
}

void
suns_modbus_to_uint16(unsigned char *buf, suns_value_t *value, uint16_t len)
{
    (*value).u16 =  suns_modbus_to_16(buf);
}

void
suns_modbus_to_int32(unsigned char *buf, suns_value_t *value, uint16_t len)
{
    suns_modbus_v32_t v;

    v.u = suns_modbus_to_32(buf);
    (*value).s32 = v.s;
}

void
suns_modbus_to_uint32(unsigned char *buf, suns_value_t *value, uint16_t len)
{
    (*value).u32 = suns_modbus_to_32(buf);
}

void
suns_modbus_to_int64(unsigned char *buf, suns_value_t *value, uint16_t len)
{
    suns_modbus_v64_t v;

    v.u = suns_modbus_to_64(buf);
    (*value).s64 = v.s;
}

void
suns_modbus_to_uint64(unsigned char *buf, suns_value_t *value, uint16_t len)
{
    (*value).u64 = suns_modbus_to_64(buf);
}

void
suns_modbus_to_float(unsigned char *buf, suns_value_t *value, uint16_t len)
{
    suns_modbus_v32_t v;

    v.u = suns_modbus_to_32(buf);
    (*value).f32 = v.f;
}

void
suns_modbus_to_double(unsigned char *buf, suns_value_t *value, uint16_t len)
{
    suns_modbus_v64_t v;

    v.u = suns_modbus_to_64(buf);
    (*value).f64 = v.d;
}

void
suns_modbus_to_str(unsigned char *buf, suns_value_t *value, uint16_t len)
{
    memcpy((*value).str, buf, len * 2);
}

void
suns_modbus_from_int16(unsigned char *buf, suns_value_t value, uint16_t len)
{
    suns_modbus_v16_t v;

    v.s = value.s16;
    suns_modbus_from_16(v.u, buf);
}

void
suns_modbus_from_uint16(unsigned char *buf, suns_value_t value, uint16_t len)
{
    suns_modbus_from_16(value.u16, buf);
}

void
suns_modbus_from_int32(unsigned char *buf, suns_value_t value, uint16_t len)
{
    suns_modbus_v32_t v;

    v.s = value.s32;
    suns_modbus_from_32(v.u, buf);
}

void
suns_modbus_from_uint32(unsigned char *buf, suns_value_t value, uint16_t len)
{
    suns_modbus_from_32(value.u32, buf);
}

void
suns_modbus_from_int64(unsigned char *buf, suns_value_t value, uint16_t len)
{
    suns_modbus_v64_t v;

    v.s = value.s64;
    suns_modbus_from_64(v.u, buf);
}

void
suns_modbus_from_uint64(unsigned char *buf, suns_value_t value, uint16_t len)
{
    suns_modbus_from_64(value.u64, buf);
}

void
suns_modbus_from_float(unsigned char *buf, suns_value_t value, uint16_t len)
{
    suns_modbus_v32_t v;

    v.f = value.f32;
    suns_modbus_from_32(v.u, buf);
}

void
suns_modbus_from_double(unsigned char *buf, suns_value_t value, uint16_t len)
{
    suns_modbus_v64_t v;

    v.d = value.f64;
    suns_modbus_from_64(v.u, buf);
}

void
suns_modbus_from_str(unsigned char *buf, suns_value_t value, uint16_t len)
{
    memcpy(buf, value.str, len * 2);
}
