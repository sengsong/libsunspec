
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

#include <stdio.h>
#include <string.h>

#include "inverter.h"

suns_err_t
inv_mod_disable(suns_device_t *device, char *name)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;

    model = suns_device_get_model(device, 0, name, 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* disable volt var in local model */
            if ((err = suns_model_point_set_uint16(model, "ModEna", 0, 0, 0)) == SUNS_ERR_OK) {
                /* write model changes to device */
                err = suns_model_write(model);
            }
        }
    }
       
    return err;
}

suns_err_t
inv_mod_get_status(suns_device_t *device, char *name, inv_mod_t *mod)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    int16_t sf;

    model = suns_device_get_model(device, 0, name, 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get volt var module enabled */
            if (((err = suns_model_point_get_uint16(model, "ModEna", 0, &mod->mod_ena, &sf)) != SUNS_ERR_OK) ||
                ((err = suns_model_point_get_uint16(model, "ActCrv", 0, &mod->act_crv, &sf))  != SUNS_ERR_OK) ||
                ((err = suns_model_point_get_uint16(model, "NCrv", 0, &mod->n_crv, &sf)) != SUNS_ERR_OK) ||
                ((err = suns_model_point_get_uint16(model, "NPt", 0, &mod->n_pt, &sf)) != SUNS_ERR_OK)) {
                return err;
            }
        }

        /* get timers */
        mod->timers.win_tms_valid = suns_model_point_is_implemented(model, "WinTms", 0);
        mod->timers.rvrt_tms_valid = suns_model_point_is_implemented(model, "RvrtTms", 0);
        mod->timers.rmp_tms_valid = suns_model_point_is_implemented(model, "RmpTms", 0);
        if (mod->timers.win_tms_valid) {
            if ((err = suns_model_point_get_uint16(model, "WinTms", 0, &mod->timers.win_tms, NULL)) != SUNS_ERR_OK) {
                return err;
            }
        }
        if (mod->timers.rvrt_tms_valid) {
            if ((err = suns_model_point_get_uint16(model, "RvrtTms", 0, &mod->timers.rvrt_tms, NULL)) != SUNS_ERR_OK) {
                return err;
            }
        }
        if (mod->timers.rmp_tms_valid) {
            if ((err = suns_model_point_get_uint16(model, "RmpTms", 0, &mod->timers.rmp_tms, NULL)) != SUNS_ERR_OK) {
                return err;
            }
        }
    }

    return err;
}

suns_err_t
inv_mod_set_timers(suns_model_t *model, inv_timers_t *timers)
{
    suns_err_t err = SUNS_ERR_OK;

    if (timers) {
        if ((timers->win_tms_valid != 0) && (err = suns_model_point_set_uint16(model, "WinTms", 0, timers->win_tms, 0)) != SUNS_ERR_OK) {
            return err;
        }
        if ((timers->rmp_tms_valid != 0) && (err = suns_model_point_set_uint16(model, "RmpTms", 0, timers->rmp_tms, 0)) != SUNS_ERR_OK) {
            return err;
        }
        if ((timers->rvrt_tms_valid != 0) && (err = suns_model_point_set_uint16(model, "RvrtTms", 0, timers->rvrt_tms, 0)) != SUNS_ERR_OK) {
            return err;
        }
    }

    return err;
}

suns_err_t
inv_volt_var_disable(suns_device_t *device)
{
    return inv_mod_disable(device, "volt_var");
}

suns_err_t
inv_volt_var_enable(suns_device_t *device, uint16_t act_crv, inv_timers_t *timers)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    uint16_t ncrv;
    uint16_t mod_ena = 0;
    int16_t sf;

    model = suns_device_get_model(device, 0, "volt_var", 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get number of curves supported */
            if ((err = suns_model_point_get_uint16(model, "NCrv", 0, &ncrv, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (act_crv > ncrv) {
                return SUNS_ERR_RANGE;
            }
            /* set active curve in local model */
            if ((err = suns_model_point_set_uint16(model, "ActCrv", 0, act_crv, 0)) != SUNS_ERR_OK) {
                return err;
            }
            /* set timers */
            if ((err = inv_mod_set_timers(model, timers)) != SUNS_ERR_OK) {
                return err;
            }
            /* write model changes to device */
            if ((err = suns_model_write(model)) != SUNS_ERR_OK) {
                return err;
            }
            if (act_crv != 0) {
                mod_ena = 1;
            }
            /* enable volt var in local model */
            if ((err = suns_model_point_set_uint16(model, "ModEna", 0, mod_ena, 0)) == SUNS_ERR_OK) {
                /* write model changes to device */
                err = suns_model_write(model);
            }
        }
    }

    return err;
}

suns_err_t
inv_volt_var_get_status(suns_device_t *device, inv_mod_t *mod)
{
    return inv_mod_get_status(device, "volt_var", mod);
}

suns_err_t
inv_volt_var_get_curve(suns_device_t *device, uint16_t index, inv_volt_var_curve_t *curve)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    uint16_t ncrv;
    uint16_t npt;
    int16_t sf;
    uint16_t i;
    char point_name[16];
    char *nam_str = NULL;

    model = suns_device_get_model(device, 0, "volt_var", 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get number of curves supported */
            if ((err = suns_model_point_get_uint16(model, "NCrv", 0, &ncrv, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (index > ncrv) {
                return SUNS_ERR_RANGE;
            }
            if ((err = suns_model_point_get_uint16(model, "NPt", 0, &npt, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            for (i = 0; i < npt; i++) {
                /* get v point */
                sprintf(point_name, "V%d", i + 1);
                if ((err = suns_model_point_get_float32(model, point_name, index, &curve->v[i])) != SUNS_ERR_OK) {
                    return err;
                }
                /* get var point */
                sprintf(point_name, "VAr%d", i + 1);
                if ((err = suns_model_point_get_float32(model, point_name, index, &curve->var[i])) != SUNS_ERR_OK) {
                    return err;
                }
            }
            curve->points = npt;
            /* get volt var active curve */
            err = suns_model_point_get_uint16(model, "DeptRef", index, &curve->dept_ref, &sf);

            /* get timers */
            curve->rmp_tms_valid = suns_model_point_is_implemented(model, "RmpTms", 0);
            curve->rmp_dec_tmm_valid = suns_model_point_is_implemented(model, "RmpDecTmm", 0);
            curve->rmp_inc_tmm_valid = suns_model_point_is_implemented(model, "RmpIncTmm", 0);
            if (curve->rmp_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "RmpTms", index, &curve->rmp_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (curve->rmp_dec_tmm_valid) {
                if ((err = suns_model_point_get_float32(model, "RmpDecTmm", index, &curve->rmp_dec_tmm)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (curve->rmp_inc_tmm_valid) {
                if ((err = suns_model_point_get_float32(model, "RmpIncTmm", index, &curve->rmp_inc_tmm)) != SUNS_ERR_OK) {
                    return err;
                }
            }

            if ((err = suns_model_point_get_str(model, "CrvNam", index, &nam_str)) != SUNS_ERR_OK) {
                return err;
            }
            memcpy(curve->crv_nam, nam_str, INV_CRV_NAM_LEN);

            if ((err = suns_model_point_get_uint16(model, "ReadOnly", index, &curve->read_only, NULL)) != SUNS_ERR_OK) {
               return err;
            }
        }
    }

    return err;
}

suns_err_t
inv_volt_var_set_curve(suns_device_t *device, uint16_t index, inv_volt_var_curve_t *curve)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    uint16_t ncrv;
    uint16_t npt;
    int16_t sf;
    uint16_t i;
    char point_name[16];

    model = suns_device_get_model(device, 0, "volt_var", 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get number of curves supported */
            if ((err = suns_model_point_get_uint16(model, "NCrv", 0, &ncrv, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (index > ncrv) {
                return SUNS_ERR_RANGE;
            }
            if ((err = suns_model_point_get_uint16(model, "NPt", 0, &npt, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (curve->points > npt) {
                return SUNS_ERR_RANGE;
            }
            for (i = 0; i < npt; i++) {
                /* get v point */
                sprintf(point_name, "V%d", i + 1);
                if ((err = suns_model_point_set_float32(model, point_name, index, curve->v[i])) != SUNS_ERR_OK) {
                    return err;
                }
                /* get var point */
                sprintf(point_name, "VAr%d", i + 1);
                if ((err = suns_model_point_set_float32(model, point_name, index, curve->var[i])) != SUNS_ERR_OK) {
                    return err;
                }
            }

            /* set timers if present */
            if ((curve->rmp_tms_valid != 0) && (err = suns_model_point_set_uint16(model, "RmpTms", index, curve->rmp_tms, 0)) != SUNS_ERR_OK) {
                return err;
            }
            if ((curve->rmp_dec_tmm_valid != 0) && (err = suns_model_point_set_float32(model, "RmpDecTmm", index, curve->rmp_dec_tmm)) != SUNS_ERR_OK) {
                return err;
            }
            if ((curve->rmp_inc_tmm_valid != 0) && (err = suns_model_point_set_float32(model, "RmpIncTmm", index, curve->rmp_inc_tmm)) != SUNS_ERR_OK) {
                return err;
            }

            /* set curve name if present */
            if (curve->crv_nam[0] != 0) {
                if ((err = suns_model_point_set_str(model, "CrvNam", index, (char *) curve->crv_nam)) != SUNS_ERR_OK) {
                    return err;
                }
            }

            curve->points = npt;
            /* get volt var active curve */
            if ((err = suns_model_point_set_uint16(model, "DeptRef", index, curve->dept_ref, 0))  != SUNS_ERR_OK) {
                return err;
            }
            /* write changes to device */
            err = suns_model_write(model);
        }
    }

    return err;
}

suns_err_t
inv_volt_watt_disable(suns_device_t *device)
{
    return inv_mod_disable(device, "volt_watt");
}

suns_err_t
inv_volt_watt_enable(suns_device_t *device, uint16_t act_crv, inv_timers_t *timers)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    uint16_t ncrv;
    uint16_t mod_ena = 0;
    int16_t sf;

    model = suns_device_get_model(device, 0, "volt_watt", 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get number of curves supported */
            if ((err = suns_model_point_get_uint16(model, "NCrv", 0, &ncrv, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (act_crv > ncrv) {
                return SUNS_ERR_RANGE;
            }
            /* set active curve in local model */
            if ((err = suns_model_point_set_uint16(model, "ActCrv", 0, act_crv, 0)) != SUNS_ERR_OK) {
                return err;
            }
            /* set timers */
            if ((err = inv_mod_set_timers(model, timers)) != SUNS_ERR_OK) {
                return err;
            }
            /* write model changes to device */
            if ((err = suns_model_write(model)) != SUNS_ERR_OK) {
                return err;
            }
            if (act_crv != 0) {
                mod_ena = 1;
            }
            /* enable volt var in local model */
            if ((err = suns_model_point_set_uint16(model, "ModEna", 0, mod_ena, 0)) == SUNS_ERR_OK) {
                /* write model changes to device */
                err = suns_model_write(model);
            }
        }
    }

    return err;
}

suns_err_t
inv_volt_watt_get_status(suns_device_t *device, inv_mod_t *mod)
{
    return inv_mod_get_status(device, "volt_watt", mod);
}

suns_err_t
inv_volt_watt_get_curve(suns_device_t *device, uint16_t index, inv_volt_watt_curve_t *curve)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    uint16_t ncrv;
    uint16_t npt;
    int16_t sf;
    uint16_t i;
    char point_name[16];
    char *nam_str = NULL;

    model = suns_device_get_model(device, 0, "volt_watt", 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get number of curves supported */
            if ((err = suns_model_point_get_uint16(model, "NCrv", 0, &ncrv, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (index > ncrv) {
                return SUNS_ERR_RANGE;
            }
            if ((err = suns_model_point_get_uint16(model, "NPt", 0, &npt, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            for (i = 0; i < npt; i++) {
                /* get v point */
                sprintf(point_name, "V%d", i + 1);
                if ((err = suns_model_point_get_float32(model, point_name, index, &curve->v[i])) != SUNS_ERR_OK) {
                    return err;
                }
                /* get var point */
                sprintf(point_name, "W%d", i + 1);
                if ((err = suns_model_point_get_float32(model, point_name, index, &curve->w[i])) != SUNS_ERR_OK) {
                    return err;
                }
            }
            curve->points = npt;
            /* get volt var active curve */
            err = suns_model_point_get_uint16(model, "DeptRef", index, &curve->dept_ref, &sf);

            /* get timers */
            curve->rmp_pt1_tms_valid = suns_model_point_is_implemented(model, "RmpPt1Tms", 0);
            curve->rmp_dec_tmm_valid = suns_model_point_is_implemented(model, "RmpDecTmm", 0);
            curve->rmp_inc_tmm_valid = suns_model_point_is_implemented(model, "RmpIncTmm", 0);
            if (curve->rmp_pt1_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "RmpPt1Tms", index, &curve->rmp_pt1_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (curve->rmp_dec_tmm_valid) {
                if ((err = suns_model_point_get_float32(model, "RmpDecTmm", index, &curve->rmp_dec_tmm)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (curve->rmp_inc_tmm_valid) {
                if ((err = suns_model_point_get_float32(model, "RmpIncTmm", index, &curve->rmp_inc_tmm)) != SUNS_ERR_OK) {
                    return err;
                }
            }

            if ((err = suns_model_point_get_str(model, "CrvNam", index, &nam_str)) != SUNS_ERR_OK) {
                return err;
            }
            memcpy(curve->crv_nam, nam_str, INV_CRV_NAM_LEN);

            if ((err = suns_model_point_get_uint16(model, "ReadOnly", index, &curve->read_only, NULL)) != SUNS_ERR_OK) {
               return err;
            }
        }
    }

    return err;
}

suns_err_t
inv_volt_watt_set_curve(suns_device_t *device, uint16_t index, inv_volt_watt_curve_t *curve)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    uint16_t ncrv;
    uint16_t npt;
    int16_t sf;
    uint16_t i;
    char point_name[16];

    model = suns_device_get_model(device, 0, "volt_watt", 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get number of curves supported */
            if ((err = suns_model_point_get_uint16(model, "NCrv", 0, &ncrv, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (index > ncrv) {
                return SUNS_ERR_RANGE;
            }
            if ((err = suns_model_point_get_uint16(model, "NPt", 0, &npt, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (curve->points > npt) {
                return SUNS_ERR_RANGE;
            }
            for (i = 0; i < npt; i++) {
                /* get v point */
                sprintf(point_name, "V%d", i + 1);
                if ((err = suns_model_point_set_float32(model, point_name, index, curve->v[i])) != SUNS_ERR_OK) {
                    return err;
                }
                /* get var point */
                sprintf(point_name, "W%d", i + 1);
                if ((err = suns_model_point_set_float32(model, point_name, index, curve->w[i])) != SUNS_ERR_OK) {
                    return err;
                }
            }

            /* set timers if present */
            if ((curve->rmp_pt1_tms_valid != 0) && (err = suns_model_point_set_uint16(model, "RmpPt1Tms", index, curve->rmp_pt1_tms, 0)) != SUNS_ERR_OK) {
                return err;
            }
            if ((curve->rmp_dec_tmm_valid != 0) && (err = suns_model_point_set_float32(model, "RmpDecTmm", index, curve->rmp_dec_tmm)) != SUNS_ERR_OK) {
                return err;
            }
            if ((curve->rmp_inc_tmm_valid != 0) && (err = suns_model_point_set_float32(model, "RmpIncTmm", index, curve->rmp_inc_tmm)) != SUNS_ERR_OK) {
                return err;
            }

            /* set curve name if present */
            if (curve->crv_nam[0] != '\0') {
                if ((err = suns_model_point_set_str(model, "CrvNam", index, (char *) curve->crv_nam)) != SUNS_ERR_OK) {
                    return err;
                }
            }

            curve->points = npt;
            /* get volt var active curve */
            if ((err = suns_model_point_set_uint16(model, "DeptRef", index, curve->dept_ref, 0))  != SUNS_ERR_OK) {
                return err;
            }
            /* write changes to device */
            err = suns_model_write(model);
        }
    }

    return err;
}

suns_err_t
inv_freq_watt_disable(suns_device_t *device)
{
    return inv_mod_disable(device, "freq_watt");
}

suns_err_t
inv_freq_watt_enable(suns_device_t *device, uint16_t act_crv, inv_timers_t *timers)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    uint16_t ncrv;
    uint16_t mod_ena = 0;
    int16_t sf;

    model = suns_device_get_model(device, 0, "freq_watt", 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get number of curves supported */
            if ((err = suns_model_point_get_uint16(model, "NCrv", 0, &ncrv, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (act_crv > ncrv) {
                return SUNS_ERR_RANGE;
            }
            /* set active curve in local model */
            if ((err = suns_model_point_set_uint16(model, "ActCrv", 0, act_crv, 0)) != SUNS_ERR_OK) {
                return err;
            }
            /* set timers */
            if ((err = inv_mod_set_timers(model, timers)) != SUNS_ERR_OK) {
                return err;
            }
            /* write model changes to device */
            if ((err = suns_model_write(model)) != SUNS_ERR_OK) {
                return err;
            }
            if (act_crv != 0) {
                mod_ena = 1;
            }
            /* enable volt var in local model */
            if ((err = suns_model_point_set_uint16(model, "ModEna", 0, mod_ena, 0)) == SUNS_ERR_OK) {
                /* write model changes to device */
                err = suns_model_write(model);
            }
        }
    }

    return err;
}

suns_err_t
inv_freq_watt_get_status(suns_device_t *device, inv_mod_t *mod)
{
    return inv_mod_get_status(device, "freq_watt", mod);
}

suns_err_t
inv_freq_watt_get_curve(suns_device_t *device, uint16_t index, inv_freq_watt_curve_t *curve)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    uint16_t ncrv;
    uint16_t npt;
    int16_t sf;
    uint16_t i;
    char point_name[16];
    char *nam_str = NULL;

    model = suns_device_get_model(device, 0, "freq_watt", 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get number of curves supported */
            if ((err = suns_model_point_get_uint16(model, "NCrv", 0, &ncrv, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (index > ncrv) {
                return SUNS_ERR_RANGE;
            }
            if ((err = suns_model_point_get_uint16(model, "NPt", 0, &npt, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            for (i = 0; i < npt; i++) {
                /* get v point */
                sprintf(point_name, "Hz%d", i + 1);
                if ((err = suns_model_point_get_float32(model, point_name, index, &curve->hz[i])) != SUNS_ERR_OK) {
                    return err;
                }
                /* get var point */
                sprintf(point_name, "W%d", i + 1);
                if ((err = suns_model_point_get_float32(model, point_name, index, &curve->w[i])) != SUNS_ERR_OK) {
                    return err;
                }
            }
            curve->points = npt;

            curve->rmp_pt1_tms_valid = suns_model_point_is_implemented(model, "RmpPt1Tms", 0);
            curve->rmp_dec_tmm_valid = suns_model_point_is_implemented(model, "RmpDecTmm", 0);
            curve->rmp_inc_tmm_valid = suns_model_point_is_implemented(model, "RmpIncTmm", 0);
            curve->rmp_rs_up_valid = suns_model_point_is_implemented(model, "RmpRsUp", 0);
            curve->w_ref_valid = suns_model_point_is_implemented(model, "WRef", 0);
            curve->w_ref_str_hz_valid = suns_model_point_is_implemented(model, "WRefStrHz", 0);
            curve->w_ref_stop_hz_valid = suns_model_point_is_implemented(model, "WRefStopHz", 0);
            if (curve->rmp_pt1_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "RmpPt1Tms", index, &curve->rmp_pt1_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (curve->rmp_dec_tmm_valid) {
                if ((err = suns_model_point_get_float32(model, "RmpDecTmm", index, &curve->rmp_dec_tmm)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (curve->rmp_inc_tmm_valid) {
                if ((err = suns_model_point_get_float32(model, "RmpIncTmm", index, &curve->rmp_inc_tmm)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (curve->rmp_rs_up_valid) {
                if ((err = suns_model_point_get_float32(model, "RmpRsUp", index, &curve->rmp_rs_up)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (curve->w_ref_valid) {
                if ((err = suns_model_point_get_float32(model, "WRef", index, &curve->w_ref)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (curve->w_ref_str_hz_valid) {
                if ((err = suns_model_point_get_float32(model, "WRefStrHz", index, &curve->w_ref_str_hz)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (curve->w_ref_stop_hz_valid) {
                if ((err = suns_model_point_get_float32(model, "WRefStopHz", index, &curve->w_ref_stop_hz)) != SUNS_ERR_OK) {
                    return err;
                }
            }

            if ((err = suns_model_point_get_str(model, "CrvNam", index, &nam_str)) != SUNS_ERR_OK) {
                return err;
            }
            memcpy(curve->crv_nam, nam_str, INV_CRV_NAM_LEN);

            if ((err = suns_model_point_get_uint16(model, "ReadOnly", index, &curve->read_only, NULL)) != SUNS_ERR_OK) {
               return err;
            }
        }
    }

    return err;
}

suns_err_t
inv_freq_watt_set_curve(suns_device_t *device, uint16_t index, inv_freq_watt_curve_t *curve)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    uint16_t ncrv;
    uint16_t npt;
    int16_t sf;
    uint16_t i;
    char point_name[16];

    model = suns_device_get_model(device, 0, "freq_watt", 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get number of curves supported */
            if ((err = suns_model_point_get_uint16(model, "NCrv", 0, &ncrv, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (index > ncrv) {
                return SUNS_ERR_RANGE;
            }
            if ((err = suns_model_point_get_uint16(model, "NPt", 0, &npt, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            if (curve->points > npt) {
                return SUNS_ERR_RANGE;
            }
            for (i = 0; i < npt; i++) {
                /* get v point */
                sprintf(point_name, "Hz%d", i + 1);
                if ((err = suns_model_point_set_float32(model, point_name, index, curve->hz[i])) != SUNS_ERR_OK) {
                    return err;
                }
                /* get var point */
                sprintf(point_name, "W%d", i + 1);
                if ((err = suns_model_point_set_float32(model, point_name, index, curve->w[i])) != SUNS_ERR_OK) {
                    return err;
                }
            }
            curve->points = npt;

            /* set timers if present */
            if ((curve->rmp_pt1_tms_valid != 0) && (err = suns_model_point_set_uint16(model, "RmpPt1Tms", index, curve->rmp_pt1_tms, 0)) != SUNS_ERR_OK) {
                return err;
            }
            if ((curve->rmp_dec_tmm_valid != 0) && (err = suns_model_point_set_float32(model, "RmpDecTmm", index, curve->rmp_dec_tmm)) != SUNS_ERR_OK) {
                return err;
            }
            if ((curve->rmp_inc_tmm_valid != 0) && (err = suns_model_point_set_float32(model, "RmpIncTmm", index, curve->rmp_inc_tmm)) != SUNS_ERR_OK) {
                return err;
            }
            if ((curve->rmp_rs_up_valid != 0) && (err = suns_model_point_set_float32(model, "RmpRsUp", index, curve->rmp_rs_up)) != SUNS_ERR_OK) {
                return err;
            }
            if ((curve->w_ref_valid != 0) && (err = suns_model_point_set_float32(model, "WRef", index, curve->w_ref)) != SUNS_ERR_OK) {
                return err;
            }
            if ((curve->w_ref_str_hz_valid != 0) && (err = suns_model_point_set_float32(model, "WRefStrHz", index, curve->w_ref_str_hz)) != SUNS_ERR_OK) {
                return err;
            }
            if ((curve->w_ref_stop_hz_valid != 0) && (err = suns_model_point_set_float32(model, "WRefStopHz", index, curve->w_ref_stop_hz)) != SUNS_ERR_OK) {
                return err;
            }

            /* set curve name if present */
            if (curve->crv_nam[0] != '\0') {
                if ((err = suns_model_point_set_str(model, "CrvNam", index, (char *) curve->crv_nam)) != SUNS_ERR_OK) {
                    return err;
                }
            }

            /* write changes to device */
            err = suns_model_write(model);
        }
    }

    return err;
}

suns_err_t
inv_get_wgra(suns_device_t *device, uint16_t *wgra)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    float wgra_float;

    model = suns_device_get_model(device, 0, "settings", 1);
    if (model) {
        /* read current WGra value */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            if (suns_model_point_is_implemented(model, "WGra", 0)) {
                /* get WGra as a float to incorperate the scale factor if present */
                if ((err = suns_model_point_get_float32(model, "WGra", 0, &wgra_float)) != SUNS_ERR_OK) {
                    return err;
                }
            } else {
                return SUNS_ERR_UNIMPLEMENTED;
            }
        }
        *wgra = (uint16_t) wgra_float;
    }

    return err;
}

suns_err_t
inv_set_wgra(suns_device_t *device, uint16_t wgra)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    float wgra_float;

    model = suns_device_get_model(device, 0, "settings", 1);
    if (model) {
        /* read current WGra value */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            if (suns_model_point_is_implemented(model, "WGra", 0)) {
                /* set WGra as a float to incorperate the scale factor if present */
                wgra_float = (float) wgra;
                if ((err = suns_model_point_set_float32(model, "WGra", 0, wgra_float)) != SUNS_ERR_OK) {
                    return err;
                }
                /* write updates to device */
                err = suns_model_write(model);
            } else {
                return SUNS_ERR_UNIMPLEMENTED;
            }
        }
    }

    return err;
}

suns_err_t
inv_get_fixed_pf(suns_device_t *device, inv_fixed_pf_t *fixed_pf)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;

    model = suns_device_get_model(device, 0, "controls", 1);
    if (model) {
        /* read fixed pf related values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* return error if fixed pf not supported */
            if ((suns_model_point_is_implemented(model, "OutPFSet", 0) == 0) ||
                (suns_model_point_is_implemented(model, "OutPFSet_Ena", 0) == 0)) {
                return SUNS_ERR_UNIMPLEMENTED;
            }
            if ((err = suns_model_point_get_float32(model, "OutPFSet", 0, &fixed_pf->pf)) != SUNS_ERR_OK) {
                return err;
            }
            if ((err = suns_model_point_get_uint16(model, "OutPFSet_Ena", 0, &fixed_pf->enabled, NULL)) != SUNS_ERR_OK) {
                return err;
            }
            fixed_pf->timers.win_tms_valid = suns_model_point_is_implemented(model, "OutPFSet_WinTms", 0);
            fixed_pf->timers.rvrt_tms_valid = suns_model_point_is_implemented(model, "OutPFSet_RvrtTms", 0);
            fixed_pf->timers.rmp_tms_valid = suns_model_point_is_implemented(model, "OutPFSet_RmpTms", 0);
            if (fixed_pf->timers.win_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "OutPFSet_WinTms", 0, &fixed_pf->timers.win_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (fixed_pf->timers.rvrt_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "OutPFSet_RvrtTms", 0, &fixed_pf->timers.rvrt_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (fixed_pf->timers.rmp_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "OutPFSet_RmpTms", 0, &fixed_pf->timers.rmp_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
        }
    }

    return err;
}

suns_err_t
inv_set_fixed_pf(suns_device_t *device, inv_fixed_pf_t *fixed_pf)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;

    model = suns_device_get_model(device, 0, "controls", 1);
    if (model) {
        /* read fixed pf related values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* return error if fixed pf not supported */
            if ((suns_model_point_is_implemented(model, "OutPFSet", 0) == 0) ||
                (suns_model_point_is_implemented(model, "OutPFSet_Ena", 0) == 0)) {
                return SUNS_ERR_UNIMPLEMENTED;
            }
            if ((err = suns_model_point_set_float32(model, "OutPFSet", 0, fixed_pf->pf)) != SUNS_ERR_OK) {
                return err;
            }

            if (fixed_pf->timers.win_tms_valid && suns_model_point_is_implemented(model, "OutPFSet_WinTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "OutPFSet_WinTms", 0, fixed_pf->timers.win_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (fixed_pf->timers.rvrt_tms_valid && suns_model_point_is_implemented(model, "OutPFSet_RvrtTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "OutPFSet_RvrtTms", 0, fixed_pf->timers.rvrt_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (fixed_pf->timers.rmp_tms_valid && suns_model_point_is_implemented(model, "OutPFSet_RmpTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "OutPFSet_RmpTms", 0, fixed_pf->timers.rmp_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            /* write updates to device */
            if ((err = suns_model_write(model)) != SUNS_ERR_OK) {
                return err;
            }
            /* enable fixed pf */
            if ((err = suns_model_point_set_uint16(model, "OutPFSet_Ena", 0, fixed_pf->enabled, 0)) != SUNS_ERR_OK) {
                return err;
            }
            /* write enable to device */
            err = suns_model_write(model);
        }
    }

    return err;
}

suns_err_t
inv_get_max_power(suns_device_t *device, inv_max_power_t *max_power)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    float value;

    model = suns_device_get_model(device, 0, "controls", 1);
    if (model) {
        /* read max power related values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* return error if max power not supported */
            if ((suns_model_point_is_implemented(model, "WMaxLimPct", 0) == 0) ||
                (suns_model_point_is_implemented(model, "WMaxLim_Ena", 0) == 0)) {
                return SUNS_ERR_UNIMPLEMENTED;
            }
            /* use floating point to capture scale factor information */
            if ((err = suns_model_point_get_float32(model, "WMaxLimPct", 0, &value)) != SUNS_ERR_OK) {
                return err;
            }
            max_power->power = (uint16_t) value;
            if ((err = suns_model_point_get_uint16(model, "WMaxLim_Ena", 0, &max_power->enabled, NULL)) != SUNS_ERR_OK) {
                return err;
            }
            max_power->timers.win_tms_valid = suns_model_point_is_implemented(model, "WMaxLimPct_WinTms", 0);
            max_power->timers.rvrt_tms_valid = suns_model_point_is_implemented(model, "WMaxLimPct_RvrtTms", 0);
            max_power->timers.rmp_tms_valid = suns_model_point_is_implemented(model, "WMaxLimPct_RmpTms", 0);
            if (max_power->timers.win_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "WMaxLimPct_WinTms", 0, &max_power->timers.win_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (max_power->timers.rvrt_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "WMaxLimPct_RvrtTms", 0, &max_power->timers.rvrt_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (max_power->timers.rmp_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "WMaxLimPct_RmpTms", 0, &max_power->timers.rmp_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
        }
    }

    return err;
}

suns_err_t
inv_set_max_power(suns_device_t *device, inv_max_power_t *max_power)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    float value;

    model = suns_device_get_model(device, 0, "controls", 1);
    if (model) {
        /* read fixed pf related values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* return error if max power not supported */
            if ((suns_model_point_is_implemented(model, "WMaxLimPct", 0) == 0) ||
                (suns_model_point_is_implemented(model, "WMaxLim_Ena", 0) == 0)) {
                return SUNS_ERR_UNIMPLEMENTED;
            }
            value = (float) max_power->power;
            if ((err = suns_model_point_set_float32(model, "WMaxLimPct", 0, value)) != SUNS_ERR_OK) {
                return err;
            }

            if (max_power->timers.win_tms_valid && suns_model_point_is_implemented(model, "WMaxLimPct_WinTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "WMaxLimPct_WinTms", 0, max_power->timers.win_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (max_power->timers.rvrt_tms_valid && suns_model_point_is_implemented(model, "WMaxLimPct_RvrtTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "WMaxLimPct_RvrtTms", 0, max_power->timers.rvrt_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (max_power->timers.rmp_tms_valid && suns_model_point_is_implemented(model, "WMaxLimPct_RmpTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "WMaxLimPct_RmpTms", 0, max_power->timers.rmp_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            /* write updates to device */
            if ((err = suns_model_write(model)) != SUNS_ERR_OK) {
                return err;
            }
            /* enable fixed pf */
            if ((err = suns_model_point_set_uint16(model, "WMaxLim_Ena", 0, max_power->enabled, 0)) != SUNS_ERR_OK) {
                return err;
            }
            /* write enable to device */
            err = suns_model_write(model);
        }
    }

    return err;
}

suns_err_t
inv_get_connect(suns_device_t *device, inv_connect_t *connect)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;

    model = suns_device_get_model(device, 0, "controls", 1);
    if (model) {
        /* read connect related values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* return error if connect/disconnect not supported */
            if ((suns_model_point_is_implemented(model, "Conn", 0) == 0)) {
                return SUNS_ERR_UNIMPLEMENTED;
            }
            if ((err = suns_model_point_get_uint16(model, "Conn", 0, &connect->conn, NULL)) != SUNS_ERR_OK) {
                return err;
            }
            connect->timers.win_tms_valid = suns_model_point_is_implemented(model, "Conn_WinTms", 0);
            connect->timers.rvrt_tms_valid = suns_model_point_is_implemented(model, "Conn_RvrtTms", 0);
            if (connect->timers.win_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "Conn_WinTms", 0, &connect->timers.win_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (connect->timers.rvrt_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "Conn_RvrtTms", 0, &connect->timers.rvrt_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
        }
    }

    return err;
}

suns_err_t
inv_set_connect(suns_device_t *device, inv_connect_t *connect)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;

    model = suns_device_get_model(device, 0, "controls", 1);
    if (model) {
        /* read connect related values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* return error if connect/disconnect not supported */
            if ((suns_model_point_is_implemented(model, "Conn", 0) == 0)) {
                return SUNS_ERR_UNIMPLEMENTED;
            }
            if (connect->timers.win_tms_valid && suns_model_point_is_implemented(model, "Conn_WinTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "Conn_WinTms", 0, connect->timers.win_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (connect->timers.rvrt_tms_valid && suns_model_point_is_implemented(model, "Conn_RvrtTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "Conn_RvrtTms", 0, connect->timers.rvrt_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            /* write updates to device */
            if ((err = suns_model_write(model)) != SUNS_ERR_OK) {
                return err;
            }
            /* update connected */
            if ((err = suns_model_point_set_uint16(model, "Conn", 0, connect->conn, 0)) != SUNS_ERR_OK) {
                return err;
            }
            /* write enable to device */
            err = suns_model_write(model);
        }
    }

    return err;
}

suns_err_t
inv_get_status(suns_device_t *device, inv_status_t *status)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;

    model = suns_device_get_model(device, 0, "inverter", 1);
    if (model) {
        /* read inverter measurement/status values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            if ((err = suns_model_point_get_float32(model, "W", 0, &status->power)) != SUNS_ERR_OK) {
                return err;
            }
            if ((err = suns_model_point_get_float32(model, "PF", 0, &status->pf)) != SUNS_ERR_OK) {
                return err;
            }
            if ((err = suns_model_point_get_float32(model, "WH", 0, &status->total_energy)) != SUNS_ERR_OK) {
                return err;
            }
            if ((err = suns_model_point_get_uint32(model, "Evt1", 0, &status->alarm, NULL)) != SUNS_ERR_OK) {
                return err;
            }
        }
    }

    return err;
}
