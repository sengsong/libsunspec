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

#ifndef _SUNSPEC_VALUE_H_
#define _SUNSPEC_VALUE_H_

typedef union {
    int16_t s16;
    uint16_t u16;
    int32_t s32;
    uint32_t u32;
    int64_t s64;
    uint64_t u64;
    float f32;
    double f64;
    char *str;
} suns_value_t;

#ifdef __cplusplus
extern "C" {
#endif

uint16_t suns_value_int16_impl(suns_value_t value);
uint16_t suns_value_uint16_impl(suns_value_t value);
uint16_t suns_value_acc16_impl(suns_value_t value);
uint16_t suns_value_int32_impl(suns_value_t value);
uint16_t suns_value_uint32_impl(suns_value_t value);
uint16_t suns_value_acc32_impl(suns_value_t value);
uint16_t suns_value_int64_impl(suns_value_t value);
uint16_t suns_value_uint64_impl(suns_value_t value);
uint16_t suns_value_acc64_impl(suns_value_t value);
uint16_t suns_value_float32_impl(suns_value_t value);
uint16_t suns_value_string_impl(suns_value_t value);
uint16_t suns_value_sunssf_impl(suns_value_t value);
uint16_t suns_value_enum16_impl(suns_value_t value);
uint16_t suns_value_enum32_impl(suns_value_t value);
uint16_t suns_value_bitfield16_impl(suns_value_t value);
uint16_t suns_value_bitfield32_impl(suns_value_t value);
uint16_t suns_value_pad_impl(suns_value_t value);
uint16_t suns_value_ipaddr_impl(suns_value_t value);
uint16_t suns_value_ipv6addr_impl(suns_value_t value);
uint16_t suns_value_impl(suns_value_t value);

void suns_value_int16_str(suns_value_t value, int16_t sf, char *str, uint16_t len);
void suns_value_uint16_str(suns_value_t value, int16_t sf, char *str, uint16_t len);
void suns_value_int32_str(suns_value_t value, int16_t sf, char *str, uint16_t len);
void suns_value_uint32_str(suns_value_t value, int16_t sf, char *str, uint16_t len);
void suns_value_int64_str(suns_value_t value, int16_t sf, char *str, uint16_t len);
void suns_value_uint64_str(suns_value_t value, int16_t sf, char *str, uint16_t len);
void suns_value_float32_str(suns_value_t value, int16_t sf, char *str, uint16_t len);
void suns_value_str_str(suns_value_t value, int16_t sf, char *str, uint16_t len);

void suns_value_float_to_int16(suns_value_t *value, int16_t sf, float f32);
void suns_value_float_to_int32(suns_value_t *value, int16_t sf, float f32);
void suns_value_float_to_int64(suns_value_t *value, int16_t sf, float f32);
void suns_value_float_to_uint16(suns_value_t *value, int16_t sf, float f32);
void suns_value_float_to_uint32(suns_value_t *value, int16_t sf, float f32);
void suns_value_float_to_uint64(suns_value_t *value, int16_t sf, float f32);
void suns_value_int16_to_float(suns_value_t value, int16_t sf, float *f32);
void suns_value_int32_to_float(suns_value_t value, int16_t sf, float *f32);
void suns_value_int64_to_float(suns_value_t value, int16_t sf, float *f32);
void suns_value_uint16_to_float(suns_value_t value, int16_t sf, float *f32);
void suns_value_uint32_to_float(suns_value_t value, int16_t sf, float *f32);
void suns_value_uint64_to_float(suns_value_t value, int16_t sf, float *f32);
void suns_value_float_to_float(suns_value_t value, int16_t sf, float *f32);
/*
void suns_value_float_to_int16(int16_t *value, int16_t sf, float f32);
void suns_value_float_to_int32(int32_t *value, int16_t sf, float f32);
void suns_value_float_to_int64(int64_t *value, int16_t sf, float f32);
void suns_value_float_to_uint16(uint16_t *value, int16_t sf, float f32);
void suns_value_float_to_uint32(uint32_t *value, int16_t sf, float f32);
void suns_value_float_to_uint64(uint64_t *value, int16_t sf, float f32);
void suns_value_int16_to_float(int16_t value, int16_t sf, float *f32);
void suns_value_int32_to_float(int32_t value, int16_t sf, float *f32);
void suns_value_int64_to_float(int64_t value, int16_t sf, float *f32);
void suns_value_uint16_to_float(uint16_t value, int16_t sf, float *f32);
void suns_value_uint32_to_float(uint32_t value, int16_t sf, float *f32);
void suns_value_uint64_to_float(uint64_t value, int16_t sf, float *f32);
*/
#ifdef __cplusplus
}
#endif

#endif /* _SUNSPEC_VALUE_H_ */
