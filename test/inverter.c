
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
inv_mod_get_status(suns_device_t *device, char *name, uint16_t *mod_ena, uint16_t *act_crv)
{
    suns_err_t err = SUNS_ERR_NOT_FOUND;
    suns_model_t *model;
    int16_t sf;

    model = suns_device_get_model(device, 0, name, 1);
    if (model) {
        /* read current volt var model values */
        if ((err = suns_model_read(model)) == SUNS_ERR_OK) {
            /* get volt var module enabled */
            if ((err = suns_model_point_get_uint16(model, "ModEna", 0, mod_ena, &sf)) != SUNS_ERR_OK) {
                return err;
            }
            /* get volt var active curve */
            err = suns_model_point_get_uint16(model, "ActCrv", 0, act_crv, &sf);
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
inv_volt_var_enable(suns_device_t *device, uint16_t act_crv, uint16_t rvrt_tms)
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
            if ((rvrt_tms != 0) && (err = suns_model_point_set_uint16(model, "RvrtTms", 0, rvrt_tms, 0)) != SUNS_ERR_OK) {
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
inv_volt_var_get_status(suns_device_t *device, uint16_t *mod_ena, uint16_t *act_crv)
{
    return inv_mod_get_status(device, "volt_var", mod_ena, act_crv);
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
inv_volt_watt_enable(suns_device_t *device, uint16_t act_crv, uint16_t rvrt_tms)
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
            if ((rvrt_tms != 0) && (err = suns_model_point_set_uint16(model, "RvrtTms", 0, rvrt_tms, 0)) != SUNS_ERR_OK) {
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
inv_volt_watt_get_status(suns_device_t *device, uint16_t *mod_ena, uint16_t *act_crv)
{
    return inv_mod_get_status(device, "volt_watt", mod_ena, act_crv);
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
inv_freq_watt_enable(suns_device_t *device, uint16_t act_crv, uint16_t rvrt_tms)
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
            if ((rvrt_tms != 0) && (err = suns_model_point_set_uint16(model, "RvrtTms", 0, rvrt_tms, 0)) != SUNS_ERR_OK) {
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
inv_freq_watt_get_status(suns_device_t *device, uint16_t *mod_ena, uint16_t *act_crv)
{
    return inv_mod_get_status(device, "freq_watt", mod_ena, act_crv);
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
            fixed_pf->win_tms_valid = suns_model_point_is_implemented(model, "OutPFSet_WinTms", 0);
            fixed_pf->rvrt_tms_valid = suns_model_point_is_implemented(model, "OutPFSet_RvrtTms", 0);
            fixed_pf->rmp_tms_valid = suns_model_point_is_implemented(model, "OutPFSet_RmpTms", 0);
            if (fixed_pf->win_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "OutPFSet_WinTms", 0, &fixed_pf->win_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (fixed_pf->rvrt_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "OutPFSet_RvrtTms", 0, &fixed_pf->rvrt_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (fixed_pf->rmp_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "OutPFSet_RmpTms", 0, &fixed_pf->rmp_tms, NULL)) != SUNS_ERR_OK) {
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

            if (fixed_pf->win_tms_valid && suns_model_point_is_implemented(model, "OutPFSet_WinTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "OutPFSet_WinTms", 0, fixed_pf->win_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (fixed_pf->rvrt_tms_valid && suns_model_point_is_implemented(model, "OutPFSet_RvrtTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "OutPFSet_RvrtTms", 0, fixed_pf->rvrt_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (fixed_pf->rmp_tms_valid && suns_model_point_is_implemented(model, "OutPFSet_RmpTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "OutPFSet_RmpTms", 0, fixed_pf->rmp_tms, 0)) != SUNS_ERR_OK) {
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
            max_power->win_tms_valid = suns_model_point_is_implemented(model, "WMaxLimPct_WinTms", 0);
            max_power->rvrt_tms_valid = suns_model_point_is_implemented(model, "WMaxLimPct_RvrtTms", 0);
            max_power->rmp_tms_valid = suns_model_point_is_implemented(model, "WMaxLimPct_RmpTms", 0);
            if (max_power->win_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "WMaxLimPct_WinTms", 0, &max_power->win_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (max_power->rvrt_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "WMaxLimPct_RvrtTms", 0, &max_power->rvrt_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (max_power->rmp_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "WMaxLimPct_RmpTms", 0, &max_power->rmp_tms, NULL)) != SUNS_ERR_OK) {
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

            if (max_power->win_tms_valid && suns_model_point_is_implemented(model, "WMaxLimPct_WinTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "WMaxLimPct_WinTms", 0, max_power->win_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (max_power->rvrt_tms_valid && suns_model_point_is_implemented(model, "WMaxLimPct_RvrtTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "WMaxLimPct_RvrtTms", 0, max_power->rvrt_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (max_power->rmp_tms_valid && suns_model_point_is_implemented(model, "WMaxLimPct_RmpTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "WMaxLimPct_RmpTms", 0, max_power->rmp_tms, 0)) != SUNS_ERR_OK) {
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
            connect->win_tms_valid = suns_model_point_is_implemented(model, "Conn_WinTms", 0);
            connect->rvrt_tms_valid = suns_model_point_is_implemented(model, "Conn_RvrtTms", 0);
            if (connect->win_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "Conn_WinTms", 0, &connect->win_tms, NULL)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (connect->rvrt_tms_valid) {
                if ((err = suns_model_point_get_uint16(model, "Conn_RvrtTms", 0, &connect->rvrt_tms, NULL)) != SUNS_ERR_OK) {
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
            if (connect->win_tms_valid && suns_model_point_is_implemented(model, "Conn_WinTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "Conn_WinTms", 0, connect->win_tms, 0)) != SUNS_ERR_OK) {
                    return err;
                }
            }
            if (connect->rvrt_tms_valid && suns_model_point_is_implemented(model, "Conn_RvrtTms", 0)) {
                if ((err = suns_model_point_set_uint16(model, "Conn_RvrtTms", 0, connect->rvrt_tms, 0)) != SUNS_ERR_OK) {
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
