
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

#ifndef _INVERTER_H_
#define _INVERTER_H_

#include <stdint.h>

#include "sunspec.h"

#define VOLT_VAR_MAX_POINTS             20

#define VOLT_VAR_DEPT_REF_W_MAX         1
#define VOLT_VAR_DEPT_REF_VAR_MAX       2
#define VOLT_VAR_DEPT_REF_VAR_AVAL      3

typedef struct {
    uint16_t points;                    /* active points */
    float v[VOLT_VAR_MAX_POINTS];       /* volt points */
    float var[VOLT_VAR_MAX_POINTS];     /* var points */
    uint16_t dept_ref;                  /* w max, var max, or var aval */
} inv_volt_var_curve_t;

#define FREQ_WATT_MAX_POINTS            20

typedef struct {
    uint16_t points;                    /* active points */
    float hz[FREQ_WATT_MAX_POINTS];     /* frequency points */
    float w[FREQ_WATT_MAX_POINTS];      /* watt points */
} inv_freq_watt_curve_t;

#define VOLT_WATT_MAX_POINTS            20

#define VOLT_WATT_DEPT_REF_W_MAX        1
#define VOLT_WATT_DEPT_REF_W_AVAL       2

typedef struct {
    uint16_t points;                    /* active points */
    float v[VOLT_WATT_MAX_POINTS];      /* volt points */
    float w[VOLT_WATT_MAX_POINTS];      /* watt points */
    uint16_t dept_ref;                  /* w max, w aval */
} inv_volt_watt_curve_t;

typedef struct {
    uint16_t enabled;
    float pf;
    uint16_t win_tms_valid;     /* win_tms is valid (0/1) */
    uint16_t win_tms;           /* randomization time window in seconds, 0 = immediately */
    uint16_t rvrt_tms_valid;    /* rvrt_tms is valid (0/1) */
    uint16_t rvrt_tms;          /* time setting is in effect in seconds, 0 = no time limit */
    uint16_t rmp_tms_valid;     /* rmp_tms is valid (0/1) */
    uint16_t rmp_tms;           /* ramp time for setting, 0 = no ramp */
} inv_fixed_pf_t;

typedef struct {
    uint16_t enabled;
    uint16_t power;
    uint16_t win_tms_valid;     /* win_tms is valid (0/1) */
    uint16_t win_tms;           /* randomization time window in seconds, 0 = immediately */
    uint16_t rvrt_tms_valid;    /* rvrt_tms is valid (0/1) */
    uint16_t rvrt_tms;          /* time setting is in effect in seconds, 0 = no time limit */
    uint16_t rmp_tms_valid;     /* rmp_tms is valid (0/1) */
    uint16_t rmp_tms;           /* ramp time for setting, 0 = no ramp */
} inv_max_power_t;

typedef struct {
    uint16_t conn;
    uint16_t win_tms_valid;     /* win_tms is valid (0/1) */
    uint16_t win_tms;           /* randomization time window in seconds, 0 = immediately */
    uint16_t rvrt_tms_valid;    /* rvrt_tms is valid (0/1) */
    uint16_t rvrt_tms;          /* time setting is in effect in seconds, 0 = no time limit */
} inv_connect_t;

typedef struct {
    float power;		/* instantaneous power */
    float pf;                   /* instantaneous power factor */
    float total_energy;         /* total cumulative energy */
    uint32_t alarm;             /* alarm flags */
} inv_status_t;

/* volt-var related functions */
suns_err_t inv_volt_var_disable(suns_device_t *device);
suns_err_t inv_volt_var_enable(suns_device_t *device, uint16_t act_crv, uint16_t rvrt_tms);
suns_err_t inv_volt_var_get_status(suns_device_t *device, uint16_t *mod_ena, uint16_t *act_crv);
suns_err_t inv_volt_var_get_curve(suns_device_t *device, uint16_t index, inv_volt_var_curve_t *curve);
suns_err_t inv_volt_var_set_curve(suns_device_t *device, uint16_t index, inv_volt_var_curve_t *curve);

/* volt-watt related functions */
suns_err_t inv_volt_watt_disable(suns_device_t *device);
suns_err_t inv_volt_watt_enable(suns_device_t *device, uint16_t act_crv, uint16_t rvrt_tms);
suns_err_t inv_volt_watt_get_status(suns_device_t *device, uint16_t *mod_ena, uint16_t *act_crv);
suns_err_t inv_volt_watt_get_curve(suns_device_t *device, uint16_t index, inv_volt_watt_curve_t *curve);
suns_err_t inv_volt_watt_set_curve(suns_device_t *device, uint16_t index, inv_volt_watt_curve_t *curve);

/* freq-watt related functions */
suns_err_t inv_freq_watt_disable(suns_device_t *device);
suns_err_t inv_freq_watt_enable(suns_device_t *device, uint16_t act_crv, uint16_t rvrt_tms);
suns_err_t inv_freq_watt_get_status(suns_device_t *device, uint16_t *mod_ena, uint16_t *act_crv);
suns_err_t inv_freq_watt_get_curve(suns_device_t *device, uint16_t index, inv_freq_watt_curve_t *curve);
suns_err_t inv_freq_watt_set_curve(suns_device_t *device, uint16_t index, inv_freq_watt_curve_t *curve);

suns_err_t inv_mod_disable(suns_device_t *device, char *name);
suns_err_t inv_mod_get_status(suns_device_t *device, char *name, uint16_t *mod_ena, uint16_t *act_crv);
suns_err_t inv_get_wgra(suns_device_t *device, uint16_t *wgra);
suns_err_t inv_set_wgra(suns_device_t *device, uint16_t wgra);
suns_err_t inv_get_fixed_pf(suns_device_t *device, inv_fixed_pf_t *fixed_pf);
suns_err_t inv_set_fixed_pf(suns_device_t *device, inv_fixed_pf_t *fixed_pf);
suns_err_t inv_get_max_power(suns_device_t *device, inv_max_power_t *max_power);
suns_err_t inv_set_max_power(suns_device_t *device, inv_max_power_t *max_power);
suns_err_t inv_get_connect(suns_device_t *device, inv_connect_t *connect);
suns_err_t inv_set_connect(suns_device_t *device, inv_connect_t *connect);
suns_err_t inv_get_status(suns_device_t *device, inv_status_t *status);

#endif /* _INVERTER_H_ */
