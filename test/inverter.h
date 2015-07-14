
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

typedef struct {
    uint16_t win_tms_valid;             /* win_tms is valid (0/1) */
    uint16_t win_tms;                   /* randomization time window in seconds, 0 = immediately */
    uint16_t rvrt_tms_valid;            /* rvrt_tms is valid (0/1) */
    uint16_t rvrt_tms;                  /* time setting is in effect in seconds, 0 = no time limit */
    uint16_t rmp_tms_valid;             /* rmp_tms is valid (0/1) */
    uint16_t rmp_tms;                   /* ramp time for setting, 0 = no ramp */
} inv_timers_t;

typedef struct {
    uint16_t mod_ena;                   /* module enabled */
    uint16_t act_crv;                   /* active curve */
    uint16_t n_crv;                     /* number of curves */
    uint16_t n_pt;                      /* number of points */
    inv_timers_t timers;                /* timers */
} inv_mod_t;

#define INV_CRV_NAM_LEN                 16
#define INV_READWRITE                   0
#define INV_READONLY                    1

#define VOLT_VAR_MAX_POINTS             20

#define VOLT_VAR_DEPT_REF_W_MAX         1
#define VOLT_VAR_DEPT_REF_VAR_MAX       2
#define VOLT_VAR_DEPT_REF_VAR_AVAL      3

typedef struct {
    uint16_t points;                    /* active points */
    uint16_t dept_ref;                  /* w max, var max, or var aval */
    float v[VOLT_VAR_MAX_POINTS];       /* volt points */
    float var[VOLT_VAR_MAX_POINTS];     /* var points */
    uint16_t rmp_tms_valid;             /* rmp_tms is valid (0/1) */
    uint16_t rmp_dec_tmm_valid;         /* rmp_dec_tmm is valid (0/1) */
    uint16_t rmp_inc_tmm_valid;         /* rmp_inc_tmm is valid (0/1) */
    uint16_t rmp_tms;                   /* ramp time for setting, 0 = no ramp */
    float rmp_dec_tmm;                  /* maximum rate at which the VAR value may be reduced in response to changes in the voltage value */
    float rmp_inc_tmm;                  /* maximum rate at which the VAR value may be increased in response to changes in the voltage value */
    uint16_t read_only;                 /* curve is read only */
    uint8_t crv_nam[INV_CRV_NAM_LEN];   /* curve name */
} inv_volt_var_curve_t;

#define FREQ_WATT_MAX_POINTS            20

typedef struct {
    uint16_t points;                    /* active points */
    float hz[FREQ_WATT_MAX_POINTS];     /* frequency points */
    float w[FREQ_WATT_MAX_POINTS];      /* watt points */
    uint16_t rmp_pt1_tms_valid;         /* rmp_pt1 is valid (0/1) */
    uint16_t rmp_dec_tmm_valid;         /* rmp_dec_tmm is valid (0/1) */
    uint16_t rmp_inc_tmm_valid;         /* rmp_inc_tmm is valid (0/1) */
    uint16_t rmp_rs_up_valid;           /* rmp_pt1 is valid (0/1) */
    uint16_t w_ref_valid;               /* w_ref_valid is valid (0/1) */
    uint16_t w_ref_str_hz_valid;        /* w_ref_str_valid is valid (0/1) */
    uint16_t w_ref_stop_hz_valid;       /* w_ref_stop_hz_valid is valid (0/1) */
    uint16_t rmp_pt1_tms;               /* ramp time for setting, 0 = no ramp */
    float rmp_dec_tmm;                  /* maximum rate at which the VAR value may be reduced in response to changes in the voltage value */
    float rmp_inc_tmm;                  /* maximum rate at which the VAR value may be increased in response to changes in the voltage value */
    float rmp_rs_up;                    /* maximum rate at which the power may be increased after releasing the frozen value of snap shot function */
    float w_ref;                        /* reference active power (default = WMax) */
    float w_ref_str_hz;                 /* frequency deviation from nominal frequency at the time of the snapshot to start constraining power output */
    float w_ref_stop_hz;                /* frequency deviation from nominal frequency at which to release the power output */
    uint16_t read_only;                 /* curve is read only */
    uint8_t crv_nam[INV_CRV_NAM_LEN];   /* curve name */
} inv_freq_watt_curve_t;

#define VOLT_WATT_MAX_POINTS            20

#define VOLT_WATT_DEPT_REF_W_MAX        1
#define VOLT_WATT_DEPT_REF_W_AVAL       2

typedef struct {
    uint16_t points;                    /* active points */
    uint16_t dept_ref;                  /* w max, w aval */
    uint16_t rmp_pt1_tms_valid;         /* rmp_pt1 is valid (0/1) */
    uint16_t rmp_dec_tmm_valid;         /* rmp_dec_tmm is valid (0/1) */
    uint16_t rmp_inc_tmm_valid;         /* rmp_inc_tmm is valid (0/1) */
    uint16_t rmp_pt1_tms;               /* ramp time for setting, 0 = no ramp */
    float v[VOLT_WATT_MAX_POINTS];      /* volt points */
    float w[VOLT_WATT_MAX_POINTS];      /* watt points */
    float rmp_dec_tmm;                  /* maximum rate at which the watt value may be reduced in response to changes in the voltage value */
    float rmp_inc_tmm;                  /* maximum rate at which the watt value may be increased in response to changes in the voltage value */
    uint16_t read_only;                 /* curve is read only */
    uint8_t crv_nam[INV_CRV_NAM_LEN];   /* curve name */
} inv_volt_watt_curve_t;

typedef struct {
    uint16_t enabled;
    float pf;
    inv_timers_t timers;            /* timers */
} inv_fixed_pf_t;

typedef struct {
    uint16_t enabled;
    uint16_t power;
    inv_timers_t timers;            /* timers */
} inv_max_power_t;

typedef struct {
    uint16_t conn;
    inv_timers_t timers;            /* timers */
} inv_connect_t;

typedef struct {
    float power;		    /* instantaneous power */
    float pf;                       /* instantaneous power factor */
    float total_energy;             /* total cumulative energy */
    uint32_t alarm;                 /* alarm flags */
} inv_status_t;

/* inverter related model type names */
#define INV_MODEL_CONTROLS          "controls"
#define INV_MODEL_FREQ_WATT         "freq_watt"
#define INV_MODEL_INVERTER          "inverter"
#define INV_MODEL_SETTINGS          "settings"
#define INV_MODEL_VOLT_VAR          "volt_var"
#define INV_MODEL_VOLT_WATT         "volt_watt"

/* inverter related points */
#define INV_ACT_CRV                 "ActCrv"
#define INV_CONN                    "Conn"
#define INV_CONN_RVRT_TMS           "Conn_RvrtTms"
#define INV_CONN_WIN_TMS            "Conn_WinTms"
#define INV_CRV_NAM                 "CrvNam"
#define INV_DEPT_REF                "DeptRef"
#define INV_EVT1                    "Evt1"
#define INV_MOD_ENA                 "ModEna"
#define INV_OUT_PF_SET              "OutPFSet"
#define INV_OUT_PF_SET_ENA          "OutPFSet_Ena"
#define INV_OUT_PF_SET_RMP_TMS      "OutPFSet_RmpTms"
#define INV_OUT_PF_SET_RVRT_TMS     "OutPFSet_RvrtTms"
#define INV_OUT_PF_SET_WIN_TMS      "OutPFSet_WinTms"
#define INV_N_CRV                   "NCrv"
#define INV_N_PT                    "NPt"
#define INV_PF                      "PF"
#define INV_READ_ONLY               "ReadOnly"
#define INV_RMP_DEC_TMM             "RmpDecTmm"
#define INV_RMP_INC_TMM             "RmpIncTmm"
#define INV_RMP_PT1_TMS             "RmpPt1Tms"
#define INV_RMP_P_T1_TMS            "RmpPT1Tms"
#define INV_RMP_RS_UP               "RmpRsUp"
#define INV_RMP_TMS                 "RmpTms"
#define INV_RVRT_TMS                "RvrtTms"
#define INV_W_GRA                   "WGra"
#define INV_W                       "W"
#define INV_WH                      "WH"
#define INV_WIN_TMS                 "WinTms"
#define INV_W_Max_LIM_ENA           "WMaxLim_Ena"
#define INV_W_MAX_LIM_PCT           "WMaxLimPct"
#define INV_W_MAX_LIM_PCT_RMP_TMS   "WMaxLimPct_RmpTms"
#define INV_W_MAX_LIM_PCT_RVRT_TMS  "WMaxLimPct_RvrtTms"
#define INV_W_MAX_LIM_PCT_WIN_TMS   "WMaxLimPct_WinTms"
#define INV_W_REF                   "WRef"
#define INV_W_REF_STOP_HZ           "WRefStopHz"
#define INV_W_REF_STR_HZ            "WRefStrHz"


/* volt-var related functions */
suns_err_t inv_volt_var_disable(suns_device_t *device);
suns_err_t inv_volt_var_enable(suns_device_t *device, uint16_t act_crv, inv_timers_t *timers);
suns_err_t inv_volt_var_get_status(suns_device_t *device, inv_mod_t *mod);
suns_err_t inv_volt_var_get_curve(suns_device_t *device, uint16_t index, inv_volt_var_curve_t *curve);
suns_err_t inv_volt_var_set_curve(suns_device_t *device, uint16_t index, inv_volt_var_curve_t *curve);

/* volt-watt related functions */
suns_err_t inv_volt_watt_disable(suns_device_t *device);
suns_err_t inv_volt_watt_enable(suns_device_t *device, uint16_t act_crv, inv_timers_t *timers);
suns_err_t inv_volt_watt_get_status(suns_device_t *device, inv_mod_t *mod);
suns_err_t inv_volt_watt_get_curve(suns_device_t *device, uint16_t index, inv_volt_watt_curve_t *curve);
suns_err_t inv_volt_watt_set_curve(suns_device_t *device, uint16_t index, inv_volt_watt_curve_t *curve);

/* freq-watt related functions */
suns_err_t inv_freq_watt_disable(suns_device_t *device);
suns_err_t inv_freq_watt_enable(suns_device_t *device, uint16_t act_crv, inv_timers_t *timers);
suns_err_t inv_freq_watt_get_status(suns_device_t *device, inv_mod_t *mod);
suns_err_t inv_freq_watt_get_curve(suns_device_t *device, uint16_t index, inv_freq_watt_curve_t *curve);
suns_err_t inv_freq_watt_set_curve(suns_device_t *device, uint16_t index, inv_freq_watt_curve_t *curve);

suns_err_t inv_mod_disable(suns_device_t *device, char *name);
suns_err_t inv_mod_get_status(suns_device_t *device, char *name, inv_mod_t *mod);
suns_err_t inv_mod_set_timers(suns_model_t *model, inv_timers_t *timers);
suns_err_t inv_get_wgra(suns_device_t *device, uint16_t *wgra);
suns_err_t inv_set_wgra(suns_device_t *device, uint16_t wgra);

/*
suns_err_t inv_get_fixed_pf(suns_device_t *device, uint16_t *enabled, float *pf, inv_timers_t *timers);
suns_err_t inv_set_fixed_pf(suns_device_t *device, uint16_t enabled, float pf, inv_timers_t *timers);
suns_err_t inv_get_max_power(suns_device_t *device, uint16_t *enabled, float *power, inv_timers_t *timers);
suns_err_t inv_set_max_power(suns_device_t *device, uint16_t enabled, float power, inv_timers_t *timers);
suns_err_t inv_get_connect(suns_device_t *device, uint16_t *conn, inv_timers_t *timers);
suns_err_t inv_set_connect(suns_device_t *device, uint16_t conn, inv_timers_t *timers);
suns_err_t inv_get_status(suns_device_t *device, inv_status_t *status);
*/

suns_err_t inv_get_fixed_pf(suns_device_t *device, inv_fixed_pf_t *fixed_pf);
suns_err_t inv_set_fixed_pf(suns_device_t *device, inv_fixed_pf_t *fixed_pf);
suns_err_t inv_get_max_power(suns_device_t *device, inv_max_power_t *max_power);
suns_err_t inv_set_max_power(suns_device_t *device, inv_max_power_t *max_power);
suns_err_t inv_get_connect(suns_device_t *device, inv_connect_t *connect);
suns_err_t inv_set_connect(suns_device_t *device, inv_connect_t *connect);
suns_err_t inv_get_status(suns_device_t *device, inv_status_t *status);

#endif /* _INVERTER_H_ */
