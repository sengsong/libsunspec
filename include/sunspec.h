
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

#ifndef _SUNSPEC_H_
#define _SUNSPEC_H_

#include <stdint.h>

#include "sunspec_error.h"
#include "sunspec_device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SUNS_LIB_VERSION    "1.0.1c"

suns_device_t * suns_device_alloc();
void suns_device_free(suns_device_t *device);
suns_err_t suns_device_rtu_cea2045(suns_device_t *device, uint16_t slave_id);
suns_err_t suns_device_rtu_serial(suns_device_t *device, char *ifc_name, uint16_t slave_id,
                                  uint32_t baudrate, uint8_t parity);
suns_err_t suns_device_tcp(suns_device_t *device, uint8_t *ipaddr, uint16_t ipport, uint16_t slave_id);
suns_err_t suns_device_sim(suns_device_t *device, uint16_t base_addr,
                           uint16_t *sim_map, uint16_t sim_map_len, uint16_t slave_id);
suns_err_t suns_device_scan(suns_device_t *device);
suns_model_t * suns_device_get_model(suns_device_t *device, uint16_t id, char *id_str, uint16_t index);
suns_err_t suns_model_read(suns_model_t *model);
suns_err_t suns_model_write(suns_model_t *model);
uint16_t suns_point_is_implemented(suns_point_t *point);
uint16_t suns_model_point_is_implemented(suns_model_t *model, char *id, uint16_t index);
suns_point_t * suns_model_get_point(suns_model_t *model, char *id, uint16_t index);
suns_err_t suns_model_point_get_str(suns_model_t *model, char *id, uint16_t index, char **str);
suns_err_t suns_model_point_set_str(suns_model_t *model, char *id, uint16_t index, char *str);
suns_err_t suns_model_point_get_int16(suns_model_t *model, char *id, uint16_t index, int16_t *value, int16_t *sf);
suns_err_t suns_model_point_set_int16(suns_model_t *model, char *id, uint16_t index, int16_t value, int16_t sf);
suns_err_t suns_model_point_get_uint16(suns_model_t *model, char *id, uint16_t index, uint16_t *value, int16_t *sf);
suns_err_t suns_model_point_set_uint16(suns_model_t *model, char *id, uint16_t index, uint16_t value, int16_t sf);
suns_err_t suns_model_point_get_int32(suns_model_t *model, char *id, uint16_t index, int32_t *value, int16_t *sf);
suns_err_t suns_model_point_set_int32(suns_model_t *model, char *id, uint16_t index, int32_t value, int16_t sf);
suns_err_t suns_model_point_get_uint32(suns_model_t *model, char *id, uint16_t index, uint32_t *value, int16_t *sf);
suns_err_t suns_model_point_set_uint32(suns_model_t *model, char *id, uint16_t index, uint32_t value, int16_t sf);
suns_err_t suns_model_point_get_int64(suns_model_t *model, char *id, uint16_t index, int64_t *value, int16_t *sf);
suns_err_t suns_model_point_set_int64(suns_model_t *model, char *id, uint16_t index, int64_t value, int16_t sf);
suns_err_t suns_model_point_get_uint64(suns_model_t *model, char *id, uint16_t index, uint64_t *value, int16_t *sf);
suns_err_t suns_model_point_set_uint64(suns_model_t *model, char *id, uint16_t index, uint64_t value, int16_t sf);
suns_err_t suns_model_point_get_float32(suns_model_t *model, char *id, uint16_t index, float *value);
suns_err_t suns_model_point_set_float32(suns_model_t *model, char *id, uint16_t index, float value);
suns_err_t suns_point_get_str(suns_point_t *point, char **str);
suns_err_t suns_point_set_str(suns_point_t *point, char *str);
suns_err_t suns_point_get_int16(suns_point_t *point, int16_t *value, int16_t *sf);
suns_err_t suns_point_set_int16(suns_point_t *point, int16_t value, int16_t sf);
suns_err_t suns_point_get_uint16(suns_point_t *point, uint16_t *value, int16_t *sf);
suns_err_t suns_point_set_uint16(suns_point_t *point, uint16_t value, int16_t sf);
suns_err_t suns_point_get_int32(suns_point_t *point, int32_t *value, int16_t *sf);
suns_err_t suns_point_set_int32(suns_point_t *point, int32_t value, int16_t sf);
suns_err_t suns_point_get_uint32(suns_point_t *point, uint32_t *value, int16_t *sf);
suns_err_t suns_point_set_uint32(suns_point_t *point, uint32_t value, int16_t sf);
suns_err_t suns_point_get_int64(suns_point_t *point, int64_t *value, int16_t *sf);
suns_err_t suns_point_set_int64(suns_point_t *point, int64_t value, int16_t sf);
suns_err_t suns_point_get_uint64(suns_point_t *point, uint64_t *value, int16_t *sf);
suns_err_t suns_point_set_uint64(suns_point_t *point, uint64_t value, int16_t sf);
suns_err_t suns_point_get_float32(suns_point_t *point, float *value);
suns_err_t suns_point_set_float32(suns_point_t *point, float value);

#ifdef __cplusplus
}
#endif

#endif /* _SUNSPEC_H_ */
