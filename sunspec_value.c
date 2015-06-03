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
#include <stdio.h>
#include <string.h>

#include "sunspec_error.h"
#include "sunspec_value.h"

int64_t suns_value_sf_table[] = {1, 10, 100, 1000, 10000, 100000, 1000000,
                                  10000000, 100000000, 100000000, 10000000000LL};
void
suns_value_signed_str(int64_t value, int16_t sf, char *str, uint16_t len)
{
    int16_t neg = 0;
    int64_t v;
    uint64_t r;
    char format[80];
    int64_t factor;
    int16_t index = sf;

    str[0] = '\0';

    if (index < 0) {
        index *= -1;
    }

    if (index > 10) {
        return;
    }
    factor = suns_value_sf_table[index];

    if (sf < 0) {
        if (value < 0) {
            value *= -1;
            neg = 1;
        }
        v = value/factor;
        r = value % factor;
        if (neg) {
            sprintf(format, "-%%lld.%%0%hullu", index);
        } else {
            sprintf(format, "%%lld.%%0%hullu", index);
        }
        snprintf(str, len, format, v, r);
    } else {
        snprintf(str, len, "%lld", ((uint64_t) value) * (uint64_t) factor);
    }
}

void
suns_value_unsigned_str(uint64_t value, int16_t sf, char *str, uint16_t len)
{
    uint64_t v;
    uint64_t r;
    char format[80];
    int64_t factor;
    int16_t index = sf;

    str[0] = '\0';

    if (index < 0) {
        index *= -1;
    }

    if (index > 10) {
        return;
    }
    factor = suns_value_sf_table[index];

    if (sf < 0) {
        v = value/factor;
        r = value % factor;
        sprintf(format, "%%llu.%%0%hullu", index);
        snprintf(str, len, format, v, r);
    } else {
        snprintf(str, len, "%llu", value * factor);
    }
}

void
suns_value_int16_str(suns_value_t value, int16_t sf, char *str, uint16_t len)
{
    suns_value_signed_str((int64_t) value.s16, sf, str, len);
}

void
suns_value_uint16_str(suns_value_t value, int16_t sf, char *str, uint16_t len)
{
    suns_value_signed_str((int64_t) value.u16, sf, str, len);
}

void
suns_value_int32_str(suns_value_t value, int16_t sf, char *str, uint16_t len)
{
    suns_value_signed_str((int64_t) value.s32, sf, str, len);
}

void
suns_value_uint32_str(suns_value_t value, int16_t sf, char *str, uint16_t len)
{
    suns_value_signed_str((int64_t) value.u32, sf, str, len);
}

void
suns_value_int64_str(suns_value_t value, int16_t sf, char *str, uint16_t len)
{
    suns_value_signed_str((int64_t) value.s64, sf, str, len);
}

void
suns_value_uint64_str(suns_value_t value, int16_t sf, char *str, uint16_t len)
{
    suns_value_signed_str((int64_t) value.u64, sf, str, len);
}

void
suns_value_float32_str(suns_value_t value, int16_t sf, char *str, uint16_t len)
{
    snprintf(str, len, "%f", value.f32);
}

void
suns_value_str_str(suns_value_t value, int16_t sf, char *str, uint16_t len)
{
    strncpy(str, value.str, len);
}

void
suns_signed_to_float(int64_t value, int16_t sf, float *f32)
{
    int64_t factor;
    int16_t index = sf;

    *f32 = 0;

    if (index < 0) {
        index *= -1;
    }

    if (index > 10) {
        return;
    }
    factor = suns_value_sf_table[index];

    if (sf < 0) {
        *f32 = ((float) value)/factor;
    } else {
        *f32 = ((float) value) * factor;
    }
}

void
suns_unsigned_to_float(uint64_t value, int16_t sf, float *f32)
{
    int64_t factor;
    int16_t index = sf;

    *f32 = 0;

    if (index < 0) {
        index *= -1;
    }

    if (index > 10) {
        return;
    }
    factor = suns_value_sf_table[index];

    if (sf < 0) {
        *f32 = ((float) value)/factor;
    } else {
        *f32 = ((float) value) * factor;
    }
}

int64_t
suns_float_to_signed(int16_t sf, float f32)
{
    int64_t factor;
    int16_t index = sf;

    if (index < 0) {
        index *= -1;
    }

    if (index > 10) {
        return 0;
    }
    factor = suns_value_sf_table[index];
    if (sf < 0) {
        return (int64_t)(f32 * factor);
    } else {
        return (int64_t)(f32/factor);
    }
}

uint64_t
suns_float_to_unsigned(int16_t sf, float f32)
{
    int64_t factor;
    int16_t index = sf;

    if (index < 0) {
        index *= -1;
    }

    if (index > 10) {
        return 0;
    }
    factor = suns_value_sf_table[index];
    if (sf < 0) {
        return (uint64_t)(f32 * factor);
    } else {
        return (uint64_t)(f32/factor);
    }
}

void
suns_value_float_to_int16(suns_value_t *value, int16_t sf, float f32)
{
    value->s16 = (int16_t) suns_float_to_signed(sf, f32);
}

void
suns_value_float_to_int32(suns_value_t *value, int16_t sf, float f32)
{
    value->s32 = (int32_t) suns_float_to_signed(sf, f32);
}

void
suns_value_float_to_int64(suns_value_t *value, int16_t sf, float f32)
{
    value->s64 = suns_float_to_signed(sf, f32);
}

void
suns_value_float_to_uint16(suns_value_t *value, int16_t sf, float f32)
{
    value->u16 = (uint16_t) suns_float_to_unsigned(sf, f32);
}

void
suns_value_float_to_uint32(suns_value_t *value, int16_t sf, float f32)
{
    value->u32 = (uint32_t) suns_float_to_unsigned(sf, f32);
}

void
suns_value_float_to_uint64(suns_value_t *value, int16_t sf, float f32)
{
    value->u64 = (uint64_t) suns_float_to_unsigned(sf, f32);
}

void
suns_value_int16_to_float(suns_value_t value, int16_t sf, float *f32)
{
    suns_signed_to_float((int64_t) value.s16, sf, f32);
}

void
suns_value_int32_to_float(suns_value_t value, int16_t sf, float *f32)
{
    suns_signed_to_float((int64_t) value.s32, sf, f32);
}

void
suns_value_int64_to_float(suns_value_t value, int16_t sf, float *f32)
{
    suns_signed_to_float(value.s64, sf, f32);
}

void
suns_value_uint16_to_float(suns_value_t value, int16_t sf, float *f32)
{
    suns_unsigned_to_float((uint64_t) value.u16, sf, f32);
}

void
suns_value_uint32_to_float(suns_value_t value, int16_t sf, float *f32)
{
    suns_unsigned_to_float((uint64_t) value.u32, sf, f32);
}

void
suns_value_uint64_to_float(suns_value_t value, int16_t sf, float *f32)
{
    suns_unsigned_to_float(value.u64, sf, f32);
}

void
suns_value_float_to_float(suns_value_t value, int16_t sf, float *f32)
{
    *f32 = value.f32;
}

uint16_t suns_value_int16_impl(suns_value_t value) {return value.s16 != -32768;}
uint16_t suns_value_uint16_impl(suns_value_t value) {return value.u16 != 0xFFFF;}
uint16_t suns_value_int32_impl(suns_value_t value) {return value.u32 != 0x80000000;}
uint16_t suns_value_uint32_impl(suns_value_t value) {return value.u32 != 0xFFFFFFFF;}
uint16_t suns_value_int64_impl(suns_value_t value) {return value.u64 != 0x8000000000000000LL;}
uint16_t suns_value_uint64_impl(suns_value_t value) {return value.u64 != 0xFFFFFFFFFFFFFFFFLL;}
uint16_t suns_value_float32_impl(suns_value_t value) {return value.u32 != 0x7fc00000;}
uint16_t suns_value_string_impl(suns_value_t value) {return value.u16 != 0;}
uint16_t suns_value_sunssf_impl(suns_value_t value) {return value.s16 != -32768;}
uint16_t suns_value_enum16_impl(suns_value_t value) {return value.u16 != 0xFFFF;}
uint16_t suns_value_enum32_impl(suns_value_t value) {return value.u32 != 0xFFFFFFFF;}
uint16_t suns_value_bitfield16_impl(suns_value_t value) {return value.u16 != 0xFFFF;}
uint16_t suns_value_bitfield32_impl(suns_value_t value) {return value.u32 != 0xFFFFFFFF;}
uint16_t suns_value_ipaddr_impl(suns_value_t value) {return value.u32 != 0;}
uint16_t suns_value_ipv6addr_impl(suns_value_t value) {return value.u16 != 0;}
uint16_t suns_value_impl(suns_value_t value) {return 1;}


