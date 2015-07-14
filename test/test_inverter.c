/*
 * Copyright (C) 2014-2015 SunSpec Alliance
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

#include "CuTest.h"

#include "inverter.h"
#include "sunspec.h"

extern uint16_t test_device[];
extern uint16_t test_device_len;

void
test_inv_volt_watt(CuTest* tc)
{
    suns_device_t *device;
    suns_err_t err;
    inv_volt_watt_curve_t curve;
    inv_volt_watt_curve_t curve_1;
    uint16_t i;
    inv_mod_t mod;
    inv_timers_t timers;

    /* allocate device */
    device = suns_device_alloc();
    CuAssertTrue(tc, device != NULL);

    /* set device interface */
    err = suns_device_sim(device, 40000, test_device, test_device_len, 1);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* scan device */
    err = suns_device_scan(device);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* volt watt get status */
    printf("\nvolt watt get status:\n");

    err = inv_volt_watt_get_status(device, &mod);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, mod.timers.win_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rvrt_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rmp_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.win_tms == 402);
    CuAssertTrue(tc, mod.timers.rvrt_tms == 502);
    CuAssertTrue(tc, mod.timers.rmp_tms == 602);
    printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);

    /* volt watt enable curve 2 */
    printf("\nvolt watt enable curve 2\n");

    timers.win_tms_valid = 1;
    timers.rvrt_tms_valid = 1;
    timers.rmp_tms_valid = 1;
    timers.win_tms = 403;
    timers.rvrt_tms = 503;
    timers.rmp_tms = 603;
    err = inv_volt_watt_enable(device, 2, &timers);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* volt watt get status */
    printf("\nvolt watt get status:\n");

    err = inv_volt_watt_get_status(device, &mod);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, mod.mod_ena == 1);
    CuAssertTrue(tc, mod.act_crv == 2);
    CuAssertTrue(tc, mod.timers.win_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rvrt_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rmp_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.win_tms == 403);
    CuAssertTrue(tc, mod.timers.rvrt_tms == 503);
    CuAssertTrue(tc, mod.timers.rmp_tms == 603);
    printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);

    /* volt watt get curve 1 */
    printf("\nvolt watt get curve 1:\n");

    err = inv_volt_watt_get_curve(device, 1, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  w = %f\n", i, curve.v[i], curve.w[i]);
    }

    /* volt watt get curve 2 */
    printf("\nvolt watt get curve 2:\n");

    err = inv_volt_watt_get_curve(device, 2, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  w = %f\n", i, curve.v[i], curve.w[i]);
    }

    /* volt watt get curve 3 */
    printf("\nvolt watt get curve 3:\n");

    err = inv_volt_watt_get_curve(device, 3, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  w = %f\n", i, curve.v[i], curve.w[i]);
    }

    /* volt watt get curve 4 */
    printf("\nvolt watt get curve 4:\n");

    err = inv_volt_watt_get_curve(device, 4, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  w = %f\n", i, curve.v[i], curve.w[i]);
    }

    /* volt watt set curve 1 */
    curve.dept_ref = 1;
    curve.points = 3;
    curve.v[0] = 10;
    curve.v[1] = 20;
    curve.v[2] = 30;
    curve.w[0] = 100;
    curve.w[1] = 200;
    curve.w[2] = 300;
    curve.rmp_pt1_tms_valid = 1;
    curve.rmp_dec_tmm_valid = 1;
    curve.rmp_inc_tmm_valid = 1;
    curve.rmp_pt1_tms = 112;
    curve.rmp_dec_tmm = 222;
    curve.rmp_inc_tmm = 332;

    printf("\nvolt watt set curve 1:\n");
    printf("dept_ref = %d\n", curve.dept_ref);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  w = %f\n", i, curve.v[i], curve.w[i]);
    }

    err = inv_volt_watt_set_curve(device, 1, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* volt watt get curve 1 */
    printf("\nvolt watt get curve 1:\n");

    memset(&curve_1, 0, sizeof(curve_1));
    err = inv_volt_watt_get_curve(device, 1, &curve_1);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, curve_1.v[0] = 10);
    CuAssertTrue(tc, curve_1.v[1] = 20);
    CuAssertTrue(tc, curve_1.v[2] = 30);
    CuAssertTrue(tc, curve_1.w[0] = 100);
    CuAssertTrue(tc, curve_1.w[1] = 200);
    CuAssertTrue(tc, curve_1.w[2] = 300);
    CuAssertTrue(tc, curve.rmp_pt1_tms_valid == 1);
    CuAssertTrue(tc, curve.rmp_dec_tmm_valid == 1);
    CuAssertTrue(tc, curve.rmp_inc_tmm_valid == 1);
    CuAssertTrue(tc, curve.rmp_pt1_tms == 112);
    CuAssertTrue(tc, curve.rmp_dec_tmm == 222);
    CuAssertTrue(tc, curve.rmp_inc_tmm == 332);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  w = %f\n", i, curve.v[i], curve.w[i]);
    }

    /* volt var disable */
    printf("\nvolt watt disable\n");

    err = inv_volt_watt_disable(device);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* volt watt get status */
    printf("\nvolt watt get status:\n");

    err = inv_volt_watt_get_status(device, &mod);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, mod.mod_ena == 0);
    printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);

    suns_device_free(device);
}

void
test_inv_freq_watt(CuTest* tc)
{
    suns_device_t *device;
    suns_err_t err;
    inv_freq_watt_curve_t curve;
    inv_freq_watt_curve_t curve_1;
    uint16_t i;
    inv_mod_t mod;
    inv_timers_t timers;

    /* allocate device */
    device = suns_device_alloc();
    CuAssertTrue(tc, device != NULL);

    /* set device interface */
    err = suns_device_sim(device, 40000, test_device, test_device_len, 1);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* scan device */
    err = suns_device_scan(device);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* freq watt get status */
    printf("\nfreq watt get status:\n");

    err = inv_freq_watt_get_status(device, &mod);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, mod.timers.win_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rvrt_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rmp_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.win_tms == 404);
    CuAssertTrue(tc, mod.timers.rvrt_tms == 504);
    CuAssertTrue(tc, mod.timers.rmp_tms == 604);
    printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);

    /* freq watt enable curve 2 */
    printf("\nfreq watt enable curve 2\n");

    timers.win_tms_valid = 1;
    timers.rvrt_tms_valid = 1;
    timers.rmp_tms_valid = 1;
    timers.win_tms = 405;
    timers.rvrt_tms = 505;
    timers.rmp_tms = 605;
    err = inv_freq_watt_enable(device, 2, &timers);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* freq watt get status */
    printf("\nfreq watt get status:\n");

    err = inv_freq_watt_get_status(device, &mod);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, mod.mod_ena == 1);
    CuAssertTrue(tc, mod.act_crv == 2);
    CuAssertTrue(tc, mod.timers.win_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rvrt_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rmp_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.win_tms == 405);
    CuAssertTrue(tc, mod.timers.rvrt_tms == 505);
    CuAssertTrue(tc, mod.timers.rmp_tms == 605);
    printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);

    /* freq watt get curve 1 */
    printf("\nfreq watt get curve 1:\n");

    err = inv_freq_watt_get_curve(device, 1, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, curve.rmp_pt1_tms_valid == 1);
    CuAssertTrue(tc, curve.rmp_dec_tmm_valid == 1);
    CuAssertTrue(tc, curve.rmp_inc_tmm_valid == 1);
    CuAssertTrue(tc, curve.rmp_rs_up_valid == 1);
    CuAssertTrue(tc, curve.w_ref_valid == 1);
    CuAssertTrue(tc, curve.w_ref_str_hz_valid == 1);
    CuAssertTrue(tc, curve.w_ref_stop_hz_valid == 1);
    CuAssertTrue(tc, curve.rmp_pt1_tms == 104);
    CuAssertTrue(tc, curve.rmp_dec_tmm == 204);
    CuAssertTrue(tc, curve.rmp_inc_tmm == 304);
    CuAssertTrue(tc, curve.rmp_rs_up == 404);
    CuAssertTrue(tc, curve.w_ref == 504);
    CuAssertTrue(tc, curve.w_ref_str_hz == 604);
    CuAssertTrue(tc, curve.w_ref_stop_hz == 704);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: hz = %f  w = %f\n", i, curve.hz[i], curve.w[i]);
    }

    /* freq watt get curve 2 */
    printf("\nfreq watt get curve 2:\n");

    err = inv_freq_watt_get_curve(device, 2, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: hz = %f  w = %f\n", i, curve.hz[i], curve.w[i]);
    }

    /* freq watt get curve 3 */
    printf("\nfreq watt get curve 3:\n");

    err = inv_freq_watt_get_curve(device, 3, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: hz = %f  w = %f\n", i, curve.hz[i], curve.w[i]);
    }

    /* freq watt get curve 4 */
    printf("\nfreq watt get curve 4:\n");

    err = inv_freq_watt_get_curve(device, 4, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: hz = %f  w = %f\n", i, curve.hz[i], curve.w[i]);
    }

    /* freq watt set curve 1 */
    curve.points = 4;
    curve.hz[0] = 76;
    curve.hz[1] = 77;
    curve.hz[2] = 78;
    curve.hz[3] = 79;
    curve.w[0] = 86;
    curve.w[1] = 87;
    curve.w[2] = 88;
    curve.w[3] = 89;
    curve.rmp_pt1_tms_valid = 1;
    curve.rmp_dec_tmm_valid = 1;
    curve.rmp_inc_tmm_valid = 1;
    curve.rmp_rs_up_valid = 1;
    curve.w_ref_valid = 1;
    curve.w_ref_str_hz_valid = 1;
    curve.w_ref_stop_hz_valid = 1;
    curve.rmp_pt1_tms = 105;
    curve.rmp_dec_tmm = 205;
    curve.rmp_inc_tmm = 305;
    curve.rmp_rs_up = 405;
    curve.w_ref = 505;
    curve.w_ref_str_hz = 605;
    curve.w_ref_stop_hz = 705;

    printf("\nfreq watt set curve 1:\n");
    for (i = 0; i < curve.points; i++) {
        printf("%02d: hz = %f  w = %f\n", i, curve.hz[i], curve.w[i]);
    }

    err = inv_freq_watt_set_curve(device, 1, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* freq watt get curve 1 */
    printf("\nfreq watt get curve 1:\n");

    memset(&curve_1, 0, sizeof(curve_1));
    err = inv_freq_watt_get_curve(device, 1, &curve_1);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, curve_1.hz[0] == 76);
    CuAssertTrue(tc, curve_1.hz[1] == 77);
    CuAssertTrue(tc, curve_1.hz[2] == 78);
    CuAssertTrue(tc, curve_1.hz[3] == 79);
    CuAssertTrue(tc, curve_1.w[0] == 86);
    CuAssertTrue(tc, curve_1.w[1] == 87);
    CuAssertTrue(tc, curve_1.w[2] == 88);
    CuAssertTrue(tc, curve_1.w[3] == 89);
    CuAssertTrue(tc, curve.rmp_pt1_tms_valid == 1);
    CuAssertTrue(tc, curve.rmp_dec_tmm_valid == 1);
    CuAssertTrue(tc, curve.rmp_inc_tmm_valid == 1);
    CuAssertTrue(tc, curve.rmp_rs_up_valid == 1);
    CuAssertTrue(tc, curve.w_ref_valid == 1);
    CuAssertTrue(tc, curve.w_ref_str_hz_valid == 1);
    CuAssertTrue(tc, curve.w_ref_stop_hz_valid == 1);
    CuAssertTrue(tc, curve.rmp_pt1_tms == 105);
    CuAssertTrue(tc, curve.rmp_dec_tmm == 205);
    CuAssertTrue(tc, curve.rmp_inc_tmm == 305);
    CuAssertTrue(tc, curve.rmp_rs_up == 405);
    CuAssertTrue(tc, curve.w_ref == 505);
    CuAssertTrue(tc, curve.w_ref_str_hz == 605);
    CuAssertTrue(tc, curve.w_ref_stop_hz == 705);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: hz = %f  w = %f\n", i, curve.hz[i], curve.w[i]);
    }

    /* volt var disable */
    printf("\nfreq watt disable\n");

    err = inv_freq_watt_disable(device);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* freq watt get status */
    printf("\nfreq watt get status:\n");

    err = inv_freq_watt_get_status(device, &mod);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, mod.mod_ena == 0);
    printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);

    suns_device_free(device);
}

void
test_inv_volt_var(CuTest* tc)
{
    suns_device_t *device;
    suns_err_t err;
    inv_volt_var_curve_t curve;
    inv_volt_var_curve_t curve_1;
    uint16_t i;
    inv_mod_t mod;
    inv_timers_t timers;

    /* allocate device */
    device = suns_device_alloc();
    CuAssertTrue(tc, device != NULL);

    /* set device interface */
    err = suns_device_sim(device, 40000, test_device, test_device_len, 1);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* scan device */
    err = suns_device_scan(device);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* volt var get status */
    printf("\nvolt var get status:\n");

    err = inv_volt_var_get_status(device, &mod);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, mod.timers.win_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rvrt_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rmp_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.win_tms == 400);
    CuAssertTrue(tc, mod.timers.rvrt_tms == 500);
    CuAssertTrue(tc, mod.timers.rmp_tms == 600);
    printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);

    /* volt var enable curve 2 */
    printf("\nvolt var enable curve 2\n");

    timers.win_tms_valid = 1;
    timers.rvrt_tms_valid = 1;
    timers.rmp_tms_valid = 1;
    timers.win_tms = 401;
    timers.rvrt_tms = 501;
    timers.rmp_tms = 601;
    err = inv_volt_var_enable(device, 2, &timers);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* volt var get status */
    printf("\nvolt var get status:\n");

    err = inv_volt_var_get_status(device, &mod);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, mod.mod_ena == 1);
    CuAssertTrue(tc, mod.act_crv == 2);
    CuAssertTrue(tc, mod.timers.win_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rvrt_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.rmp_tms_valid == 1);
    CuAssertTrue(tc, mod.timers.win_tms == 401);
    CuAssertTrue(tc, mod.timers.rvrt_tms == 501);
    CuAssertTrue(tc, mod.timers.rmp_tms == 601);
    printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);

    /* volt var get curve 1 */
    printf("\nvolt var get curve 1:\n");

    err = inv_volt_var_get_curve(device, 1, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
    }

    /* volt var get curve 2 */
    printf("\nvolt var get curve 2:\n");

    err = inv_volt_var_get_curve(device, 2, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
    }
    CuAssertTrue(tc, curve.rmp_tms_valid == 1 && curve.rmp_tms == 330);
    CuAssertTrue(tc, curve.rmp_dec_tmm_valid == 1 && curve.rmp_dec_tmm == 440);
    CuAssertTrue(tc, curve.rmp_inc_tmm_valid == 1 && curve.rmp_inc_tmm == 550);
    CuAssertTrue(tc, curve.read_only == 1);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* volt var get curve 3 */
    printf("\nvolt var get curve 3:\n");

    err = inv_volt_var_get_curve(device, 3, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
    }

    /* volt var get curve 4 */
    printf("\nvolt var get curve 4:\n");

    err = inv_volt_var_get_curve(device, 4, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
    }

    /* volt var set curve 1 */
    curve.dept_ref = 1;
    curve.points = 4;
    curve.v[0] = 10;
    curve.v[1] = 20;
    curve.v[2] = 30;
    curve.v[3] = 40;
    curve.var[0] = 100;
    curve.var[1] = 200;
    curve.var[2] = 300;
    curve.var[3] = 400;
    curve.rmp_tms_valid = 1;
    curve.rmp_dec_tmm_valid = 1;
    curve.rmp_inc_tmm_valid = 1;
    curve.rmp_tms = 110;
    curve.rmp_dec_tmm = 220;
    curve.rmp_inc_tmm = 330;

    printf("\nvolt var set curve 1:\n");
    printf("dept_ref = %d\n", curve.dept_ref);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
    }

    err = inv_volt_var_set_curve(device, 1, &curve);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* volt var get curve 1 */
    printf("\nvolt var get curve 1:\n");

    memset(&curve_1, 0, sizeof(curve_1));
    err = inv_volt_var_get_curve(device, 1, &curve_1);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, curve_1.v[0] == 10);
    CuAssertTrue(tc, curve_1.v[1] == 20);
    CuAssertTrue(tc, curve_1.v[2] == 30);
    CuAssertTrue(tc, curve_1.v[3] == 40);
    CuAssertTrue(tc, curve_1.var[0] == 100);
    CuAssertTrue(tc, curve_1.var[1] == 200);
    CuAssertTrue(tc, curve_1.var[2] == 300);
    CuAssertTrue(tc, curve_1.var[3] == 400);
    CuAssertTrue(tc, curve.rmp_tms_valid == 1);
    CuAssertTrue(tc, curve.rmp_dec_tmm_valid == 1);
    CuAssertTrue(tc, curve.rmp_inc_tmm_valid == 1);
    CuAssertTrue(tc, curve.rmp_tms == 110);
    CuAssertTrue(tc, curve.rmp_dec_tmm == 220);
    CuAssertTrue(tc, curve.rmp_inc_tmm == 330);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
    }

    /* volt var disable */
    printf("\nvolt var disable\n");

    err = inv_volt_var_disable(device);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* volt var get status */
    printf("\nvolt var get status:\n");

    err = inv_volt_var_get_status(device, &mod);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, mod.mod_ena == 0);
    printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);

    suns_device_free(device);
}

void
test_inv(CuTest* tc)
{
    suns_device_t *device;
    suns_err_t err;
    uint16_t wgra;
    inv_fixed_pf_t fixed_pf;
    inv_max_power_t max_power;
    inv_connect_t connect;
    inv_status_t status;

    /* allocate device */
    device = suns_device_alloc();
    CuAssertTrue(tc, device != NULL);

    /* set device interface */
    err = suns_device_sim(device, 40000, test_device, test_device_len, 1);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* scan device */
    err = suns_device_scan(device);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* get WGra */
    printf("\nget WGra:\n");
    err = inv_get_wgra(device, &wgra);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    printf("WGra = %d\n", wgra);

    /* set WGra */
    printf("\nset WGra to 80\n");
    err = inv_set_wgra(device, 80);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* get WGra */
    printf("\nget WGra:\n");
    err = inv_get_wgra(device, &wgra);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    CuAssertTrue(tc, wgra == 80);
    printf("WGra = %d\n", wgra);

    /* get fixed power factor */
    printf("\nget fixed power factor:\n");

    err = inv_get_fixed_pf(device, &fixed_pf);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    printf("enabled = %d\n", fixed_pf.enabled);
    printf("pf = %f\n", fixed_pf.pf);
    printf("win_tms_valid = %d\n", fixed_pf.timers.win_tms_valid);
    printf("win_tms = %d\n", fixed_pf.timers.win_tms);
    printf("rvrt_tms_valid = %d\n", fixed_pf.timers.rvrt_tms_valid);
    printf("rvrt_tms = %d\n", fixed_pf.timers.rvrt_tms);
    printf("rmp_tms_valid = %d\n", fixed_pf.timers.rmp_tms_valid);
    printf("rmp_tms = %d\n", fixed_pf.timers.rmp_tms);

    fixed_pf.enabled = 1;
    fixed_pf.pf = .888;
    fixed_pf.timers.win_tms_valid = 1;
    fixed_pf.timers.win_tms = 10;
    fixed_pf.timers.rvrt_tms_valid = 1;
    fixed_pf.timers.rvrt_tms = 20;
    fixed_pf.timers.rmp_tms_valid = 1;
    fixed_pf.timers.rmp_tms = 30;

    /* set fixed power factor */
    printf("\nset and enable fixed power factor:\n");
    printf("enabled = %d\n", fixed_pf.enabled);
    printf("pf = %f\n", fixed_pf.pf);
    printf("win_tms_valid = %d\n", fixed_pf.timers.win_tms_valid);
    printf("win_tms = %d\n", fixed_pf.timers.win_tms);
    printf("rvrt_tms_valid = %d\n", fixed_pf.timers.rvrt_tms_valid);
    printf("rvrt_tms = %d\n", fixed_pf.timers.rvrt_tms);
    printf("rmp_tms_valid = %d\n", fixed_pf.timers.rmp_tms_valid);
    printf("rmp_tms = %d\n", fixed_pf.timers.rmp_tms);

    err = inv_set_fixed_pf(device, &fixed_pf);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* get fixed power factor */
    printf("\nget fixed power factor:\n");

    /* get fixed power factor */
    err = inv_get_fixed_pf(device, &fixed_pf);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    printf("enabled = %d\n", fixed_pf.enabled);
    printf("pf = %f\n", fixed_pf.pf);
    printf("win_tms_valid = %d\n", fixed_pf.timers.win_tms_valid);
    printf("win_tms = %d\n", fixed_pf.timers.win_tms);
    printf("rvrt_tms_valid = %d\n", fixed_pf.timers.rvrt_tms_valid);
    printf("rvrt_tms = %d\n", fixed_pf.timers.rvrt_tms);
    printf("rmp_tms_valid = %d\n", fixed_pf.timers.rmp_tms_valid);
    printf("rmp_tms = %d\n", fixed_pf.timers.rmp_tms);
    CuAssertTrue(tc, fixed_pf.enabled == 1);
    CuAssertTrue(tc, (fixed_pf.pf > .887) && (fixed_pf.pf < .889));
    CuAssertTrue(tc, fixed_pf.timers.win_tms_valid == 1);
    CuAssertTrue(tc, fixed_pf.timers.win_tms == 10);
    CuAssertTrue(tc, fixed_pf.timers.rvrt_tms_valid == 1);
    CuAssertTrue(tc, fixed_pf.timers.rvrt_tms == 20);
    CuAssertTrue(tc, fixed_pf.timers.rmp_tms_valid == 1);
    CuAssertTrue(tc, fixed_pf.timers.rmp_tms == 30);

    /* get max power */
    printf("\nget max power:\n");

    /* get max power */
    err = inv_get_max_power(device, &max_power);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    printf("enabled = %d\n", max_power.enabled);
    printf("power = %d\n", max_power.power);
    printf("win_tms_valid = %d\n", max_power.timers.win_tms_valid);
    printf("win_tms = %d\n", max_power.timers.win_tms);
    printf("rvrt_tms_valid = %d\n", max_power.timers.rvrt_tms_valid);
    printf("rvrt_tms = %d\n", max_power.timers.rvrt_tms);
    printf("rmp_tms_valid = %d\n", max_power.timers.rmp_tms_valid);
    printf("rmp_tms = %d\n", max_power.timers.rmp_tms);

    max_power.enabled = 1;
    max_power.power = 75;
    max_power.timers.win_tms_valid = 1;
    max_power.timers.win_tms = 40;
    max_power.timers.rvrt_tms_valid = 1;
    max_power.timers.rvrt_tms = 50;
    max_power.timers.rmp_tms_valid = 1;
    max_power.timers.rmp_tms = 60;

    /* set max power */
    printf("\nset and enable max power:\n");
    printf("enabled = %d\n", max_power.enabled);
    printf("power = %d\n", max_power.power);
    printf("win_tms_valid = %d\n", max_power.timers.win_tms_valid);
    printf("win_tms = %d\n", max_power.timers.win_tms);
    printf("rvrt_tms_valid = %d\n", max_power.timers.rvrt_tms_valid);
    printf("rvrt_tms = %d\n", max_power.timers.rvrt_tms);
    printf("rmp_tms_valid = %d\n", max_power.timers.rmp_tms_valid);
    printf("rmp_tms = %d\n", max_power.timers.rmp_tms);

    err = inv_set_max_power(device, &max_power);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* get max power */
    printf("\nget max power:\n");

    /* get max power */
    err = inv_get_max_power(device, &max_power);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    printf("enabled = %d\n", max_power.enabled);
    printf("power = %d\n", max_power.power);
    printf("win_tms_valid = %d\n", max_power.timers.win_tms_valid);
    printf("win_tms = %d\n", max_power.timers.win_tms);
    printf("rvrt_tms_valid = %d\n", max_power.timers.rvrt_tms_valid);
    printf("rvrt_tms = %d\n", max_power.timers.rvrt_tms);
    printf("rmp_tms_valid = %d\n", max_power.timers.rmp_tms_valid);
    printf("rmp_tms = %d\n", max_power.timers.rmp_tms);
    CuAssertTrue(tc, max_power.enabled == 1);
    CuAssertTrue(tc, max_power.power == 75);
    CuAssertTrue(tc, max_power.timers.win_tms_valid == 1);
    CuAssertTrue(tc, max_power.timers.win_tms == 40);
    CuAssertTrue(tc, max_power.timers.rvrt_tms_valid == 1);
    CuAssertTrue(tc, max_power.timers.rvrt_tms == 50);
    CuAssertTrue(tc, max_power.timers.rmp_tms_valid == 1);
    CuAssertTrue(tc, max_power.timers.rmp_tms == 60);

    /* get connect */
    printf("\nget connect:\n");

    err = inv_get_connect(device, &connect);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    printf("conn = %d\n", connect.conn);
    printf("win_tms_valid = %d\n", max_power.timers.win_tms_valid);
    printf("win_tms = %d\n", max_power.timers.win_tms);
    printf("rvrt_tms_valid = %d\n", max_power.timers.rvrt_tms_valid);
    printf("rvrt_tms = %d\n", max_power.timers.rvrt_tms);

    printf("\ndisconnect (set conn to 0)\n");

    connect.conn = 0;
    connect.timers.win_tms_valid = 1;
    connect.timers.win_tms = 40;
    connect.timers.rvrt_tms_valid = 1;
    connect.timers.rvrt_tms = 50;

    err = inv_set_connect(device, &connect);
    CuAssertTrue(tc, err == SUNS_ERR_OK);

    /* get connect */
    printf("\nget connect:\n");

    err = inv_get_connect(device, &connect);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    printf("conn = %d\n", connect.conn);
    printf("win_tms_valid = %d\n", max_power.timers.win_tms_valid);
    printf("win_tms = %d\n", max_power.timers.win_tms);
    printf("rvrt_tms_valid = %d\n", max_power.timers.rvrt_tms_valid);
    printf("rvrt_tms = %d\n", max_power.timers.rvrt_tms);
    CuAssertTrue(tc, connect.conn == 0);
    CuAssertTrue(tc, connect.timers.win_tms_valid == 1);
    CuAssertTrue(tc, connect.timers.win_tms == 40);
    CuAssertTrue(tc, connect.timers.rvrt_tms_valid == 1);
    CuAssertTrue(tc, connect.timers.rvrt_tms == 50);

    /* get status */
    printf("\nget status:\n");

    err = inv_get_status(device, &status);
    CuAssertTrue(tc, err == SUNS_ERR_OK);
    printf("power = %f\n", status.power);
    printf("pf = %f\n", status.pf);
    printf("total_energy = %f\n", status.total_energy);
    printf("alarm = 0x%08x\n", status.alarm);

    suns_device_free(device);
}
