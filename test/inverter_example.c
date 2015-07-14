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
#include "sunspec.h"

uint16_t inv_test_device[] = {
    0x5375, 0x6e53,                 /* SunS */
    /* model 1 */
    1, 66,
    0x5375, 0x6e53, 0x7065, 0x6354, 0x6573, 0x7400, 0x0000, 0x0000,    /* Mn - SunSpecTest */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x5465, 0x7374, 0x496e, 0x7665, 0x7274, 0x6572, 0x0000, 0x0000,    /* Md - TestInverter */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x6f70, 0x745f, 0x615f, 0x625f, 0x6300, 0x0000, 0x0000, 0x0000,    /* Opt - opt_a_b_c */
    0x312e, 0x322e, 0x3300, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,    /* Vr - 1.2.3 */
    0x736e, 0x2d31, 0x3233, 0x3435, 0x3637, 0x3839, 0x0000, 0x0000,    /* SN - sn-123456789 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0001,
    -32768,                         /* Pad */
    /* model 101 */
    101, 50,
    549,                            /* A */
    549,                            /* AphA */
    65535,                          /* AphB */
    65535,                          /* AphC */
    -2,                             /* A_SF */
    65535,                          /* PPVphAB */
    65535,                          /* PPVphBC */
    65535,                          /* PPVphCA */
    243,                            /* PhVphA */
    65535,                          /* PhVphB */
    65535,                          /* PhVphC */
    0,                              /* V_SF */
    1307,                           /* W */
    0,                              /* W_SF */
    6001,                           /* Hz */
    -2,                             /* Hz_SF */
    1334,                           /* VA */
    0,                              /* VA_SF */
    267,                            /* VAr */
    0,                              /* VAr_SF */
    979,                            /* PF */
    -3,                             /* PF_SF */
    0x001a, 0x4287,                 /* WH  (1720967) */
    0,                              /* WH_SF */
    380,                            /* DCA */
    -2,                             /* DCA_SF */
    350,                            /* DCV */
    0,                              /* DCV_SF */
    13300,                          /* DCW */
    -1,                             /* DCW_SF */
    -32768,                         /* TmpCab */
    -32768,                         /* TmpSnk */
    -32768,                         /* TmpTrns */
    -32768,                         /* TmpOt */
    -32768,                         /* Tmp_SF */
    4,                              /* St */
    4,                              /* StVnd */
    0x0000, 0x0000,                 /* Evt1  (0) */
    0x0000, 0x0000,                 /* Evt2  (0) */
    0x0000, 0x0000,                 /* EvtVnd1  (0) */
    0x0000, 0x0000,                 /* EvtVnd2  (0) */
    0x0000, 0x0000,                 /* EvtVnd3  (0) */
    0x0000, 0x0000,                 /* EvtVnd4  (0) */
    /* model 120 */
    120, 26,
    4,                              /* DERTyp */
    7500,                           /* WRtg */
    0,                              /* WRtg_SF */
    7500,                           /* VARtg */
    0,                              /* VARtg_SF */
    400,                            /* VArRtgQ1 */
    -32768,                         /* VArRtgQ2 */
    -32768,                         /* VArRtgQ3 */
    -400,                           /* VArRtgQ4 */
    1,                              /* VArRtg_SF */
    3130,                           /* ARtg */
    -2,                             /* ARtg_SF */
    -850,                           /* PFRtgQ1 */
    -32768,                         /* PFRtgQ2 */
    -32768,                         /* PFRtgQ3 */
    850,                            /* PFRtgQ4 */
    -3,                             /* PFRtg_SF */
    65535,                          /* WHRtg */
    -32768,                         /* WHRtg_SF */
    65535,                          /* AhrRtg */
    -32768,                         /* AhrRtg_SF */
    65535,                          /* MaxChaRte */
    -32768,                         /* MaxChaRte_SF */
    65535,                          /* MaxDisChaRte */
    -32768,                         /* MaxDisChaRte_SF */
    -32768,                         /* Pad */
    /* model 121 */
    121, 30,
    7500,                           /* WMax */
    240,                            /* VRef */
    0,                              /* VRefOfs */
    269,                            /* VMax */
    206,                            /* VMin */
    7500,                           /* VAMax */
    400,                            /* VArMaxQ1 */
    -32768,                         /* VArMaxQ2 */
    -32768,                         /* VArMaxQ3 */
    -400,                           /* VArMaxQ4 */
    1000,                            /* WGra */
    -850,                           /* PFMinQ1 */
    -32768,                         /* PFMinQ2 */
    -32768,                         /* PFMinQ3 */
    850,                            /* PFMinQ4 */
    65535,                          /* VArAct */
    65535,                          /* ClcTotVA */
    65535,                          /* MaxRmpRte */
    65535,                          /* ECPNomHz */
    65535,                          /* ConnPh */
    0,                              /* WMax_SF */
    0,                              /* VRef_SF */
    0,                              /* VRefOfs_SF */
    0,                              /* VMinMax_SF */
    0,                              /* VAMax_SF */
    1,                              /* VArMax_SF */
    -1,                             /* WGra_SF (-32768) */
    -3,                             /* PFMin_SF */
    -32768,                         /* MaxRmpRte_SF */
    -32768,                         /* ECPNomHz_SF */
    /* model 122 */
    122, 44,
    7,                              /* PVConn */
    0,                              /* StorConn */
    1,                              /* ECPConn */
    0x0000, 0x0000, 0x001a, 0x4287, /* ActWh  (1720967) */
    0x0000, 0x0000, 0x0000, 0x0000, /* ActVAh  (0) */
    0x0000, 0x0000, 0x0000, 0x0000, /* ActVArhQ1  (0) */
    0x0000, 0x0000, 0x0000, 0x0000, /* ActVArhQ2  (0) */
    0x0000, 0x0000, 0x0000, 0x0000, /* ActVArhQ3  (0) */
    0x0000, 0x0000, 0x0000, 0x0000, /* ActVArhQ4  (0) */
    -32768,                         /* VArAval */
    -32768,                         /* VArAval_SF */
    65535,                          /* WAval */
    -32768,                         /* WAval_SF */
    0xffff, 0xffff,                 /* StSetLimMsk  (4294967295) */
    0x0000, 0x0000,                 /* StActCtl  (0) */
    0x5254, 0x4300, 0x0000, 0x0000,     /* TmSrc - RTC */
    0x0000, 0x0000,                 /* Tms  (0) */
    65535,                          /* RtSt */
    0,                              /* Ris */
    0,                              /* Ris_SF */
    /* model 123 */
    123, 24,
    0,                              /* Conn_WinTms */
    0,                              /* Conn_RvrtTms */
    1,                              /* Conn */
    100,                            /* WMaxLimPct */
    0,                              /* WMaxLimPct_WinTms */
    0,                              /* WMaxLimPct_RvrtTms */
    0,                              /* WMaxLimPct_RmpTms */
    0,                              /* WMaxLim_Ena */
    900,                              /* OutPFSet */
    0,                              /* OutPFSet_WinTms */
    0,                              /* OutPFSet_RvrtTms */
    0,                              /* OutPFSet_RmpTms */
    0,                              /* OutPFSet_Ena */
    -32768,                         /* VArWMaxPct */
    0,                              /* VArMaxPct */
    -32768,                         /* VArAvalPct */
    0,                              /* VArPct_WinTms */
    0,                              /* VArPct_RvrtTms */
    0,                              /* VArPct_RmpTms */
    2,                              /* VArPct_Mod */
    0,                              /* VArPct_Ena */
    0,                              /* WMaxLimPct_SF */
    -3,                             /* OutPFSet_SF */
    0,                              /* VArPct_SF */
    /* model 126 */
    126, 226,
    1,                              /* ActCrv */
    0,                              /* ModEna */
    0,                              /* WinTms */
    600,                            /* RvrtTms */
    65535,                          /* RmpTms */
    4,                              /* NCrv */
    4,                              /* NPt */
    0,                              /* V_SF */
    0,                              /* DeptRef_SF */
    0,                              /* RmpIncDec_SF */
    4,                              /* ActPt */
    2,                              /* DeptRef */
    95,                             /* V1 */
    100,                            /* VAr1 */
    98,                             /* V2 */
    0,                              /* VAr2 */
    102,                            /* V3 */
    0,                              /* VAr3 */
    105,                            /* V4 */
    -100,                           /* VAr4 */
    65535,                          /* V5 */
    -32768,                         /* VAr5 */
    65535,                          /* V6 */
    -32768,                         /* VAr6 */
    65535,                          /* V7 */
    -32768,                         /* VAr7 */
    65535,                          /* V8 */
    -32768,                         /* VAr8 */
    65535,                          /* V9 */
    -32768,                         /* VAr9 */
    65535,                          /* V10 */
    -32768,                         /* VAr10 */
    65535,                          /* V11 */
    -32768,                         /* VAr11 */
    65535,                          /* V12 */
    -32768,                         /* VAr12 */
    65535,                          /* V13 */
    -32768,                         /* VAr13 */
    65535,                          /* V14 */
    -32768,                         /* VAr14 */
    65535,                          /* V15 */
    -32768,                         /* VAr15 */
    65535,                          /* V16 */
    -32768,                         /* VAr16 */
    65535,                          /* V17 */
    -32768,                         /* VAr17 */
    65535,                          /* V18 */
    -32768,                         /* VAr18 */
    65535,                          /* V19 */
    -32768,                         /* VAr19 */
    65535,                          /* V20 */
    -32768,                         /* VAr20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpTms */
    65535,                          /* RmpDecTmm */
    65535,                          /* RmpIncTmm */
    0,                              /* ReadOnly */
    4,                              /* ActPt */
    2,                              /* DeptRef */
    95,                             /* V1 */
    100,                            /* VAr1 */
    98,                             /* V2 */
    0,                              /* VAr2 */
    102,                            /* V3 */
    0,                              /* VAr3 */
    105,                            /* V4 */
    -100,                           /* VAr4 */
    65535,                          /* V5 */
    -32768,                         /* VAr5 */
    65535,                          /* V6 */
    -32768,                         /* VAr6 */
    65535,                          /* V7 */
    -32768,                         /* VAr7 */
    65535,                          /* V8 */
    -32768,                         /* VAr8 */
    65535,                          /* V9 */
    -32768,                         /* VAr9 */
    65535,                          /* V10 */
    -32768,                         /* VAr10 */
    65535,                          /* V11 */
    -32768,                         /* VAr11 */
    65535,                          /* V12 */
    -32768,                         /* VAr12 */
    65535,                          /* V13 */
    -32768,                         /* VAr13 */
    65535,                          /* V14 */
    -32768,                         /* VAr14 */
    65535,                          /* V15 */
    -32768,                         /* VAr15 */
    65535,                          /* V16 */
    -32768,                         /* VAr16 */
    65535,                          /* V17 */
    -32768,                         /* VAr17 */
    65535,                          /* V18 */
    -32768,                         /* VAr18 */
    65535,                          /* V19 */
    -32768,                         /* VAr19 */
    65535,                          /* V20 */
    -32768,                         /* VAr20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    10,                             /* RmpTms */
    22,                             /* RmpDecTmm */
    33,                             /* RmpIncTmm */
    1,                              /* ReadOnly */
    4,                              /* ActPt */
    2,                              /* DeptRef */
    93,                             /* V1 */
    100,                            /* VAr1 */
    98,                             /* V2 */
    0,                              /* VAr2 */
    102,                            /* V3 */
    0,                              /* VAr3 */
    108,                            /* V4 */
    -100,                           /* VAr4 */
    65535,                          /* V5 */
    -32768,                         /* VAr5 */
    65535,                          /* V6 */
    -32768,                         /* VAr6 */
    65535,                          /* V7 */
    -32768,                         /* VAr7 */
    65535,                          /* V8 */
    -32768,                         /* VAr8 */
    65535,                          /* V9 */
    -32768,                         /* VAr9 */
    65535,                          /* V10 */
    -32768,                         /* VAr10 */
    65535,                          /* V11 */
    -32768,                         /* VAr11 */
    65535,                          /* V12 */
    -32768,                         /* VAr12 */
    65535,                          /* V13 */
    -32768,                         /* VAr13 */
    65535,                          /* V14 */
    -32768,                         /* VAr14 */
    65535,                          /* V15 */
    -32768,                         /* VAr15 */
    65535,                          /* V16 */
    -32768,                         /* VAr16 */
    65535,                          /* V17 */
    -32768,                         /* VAr17 */
    65535,                          /* V18 */
    -32768,                         /* VAr18 */
    65535,                          /* V19 */
    -32768,                         /* VAr19 */
    65535,                          /* V20 */
    -32768,                         /* VAr20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpTms */
    0,                              /* RmpDecTmm */
    0,                              /* RmpIncTmm */
    0,                              /* ReadOnly */
    4,                              /* ActPt */
    2,                              /* DeptRef */
    90,                             /* V1 */
    100,                            /* VAr1 */
    100,                            /* V2 */
    0,                              /* VAr2 */
    103,                            /* V3 */
    -50,                            /* VAr3 */
    110,                            /* V4 */
    -100,                           /* VAr4 */
    65535,                          /* V5 */
    -32768,                         /* VAr5 */
    65535,                          /* V6 */
    -32768,                         /* VAr6 */
    65535,                          /* V7 */
    -32768,                         /* VAr7 */
    65535,                          /* V8 */
    -32768,                         /* VAr8 */
    65535,                          /* V9 */
    -32768,                         /* VAr9 */
    65535,                          /* V10 */
    -32768,                         /* VAr10 */
    65535,                          /* V11 */
    -32768,                         /* VAr11 */
    65535,                          /* V12 */
    -32768,                         /* VAr12 */
    65535,                          /* V13 */
    -32768,                         /* VAr13 */
    65535,                          /* V14 */
    -32768,                         /* VAr14 */
    65535,                          /* V15 */
    -32768,                         /* VAr15 */
    65535,                          /* V16 */
    -32768,                         /* VAr16 */
    65535,                          /* V17 */
    -32768,                         /* VAr17 */
    65535,                          /* V18 */
    -32768,                         /* VAr18 */
    65535,                          /* V19 */
    -32768,                         /* VAr19 */
    65535,                          /* V20 */
    -32768,                         /* VAr20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpTms */
    0,                              /* RmpDecTmm */
    0,                              /* RmpIncTmm */
    0,                              /* ReadOnly */
    /* model 131 */
    131, 226,
    0,                              /* ActCrv */
    0,                              /* ModEna */
    0,                              /* WinTms */
    0,                              /* RvrtTms */
    65535,                          /* RmpTms */
    4,                              /* NCrv */
    4,                              /* NPt */
    0,                              /* W_SF */
    -3,                             /* PF_SF */
    0,                              /* RmpIncDec_SF */
    4,                              /* ActPt */
    0,                              /* W1 */
    930,                            /* PF1 */
    10,                             /* W2 */
    940,                            /* PF2 */
    90,                             /* W3 */
    950,                            /* PF3 */
    100,                            /* W4 */
    960,                            /* PF4 */
    -32768,                         /* W5 */
    -32768,                         /* PF5 */
    -32768,                         /* W6 */
    -32768,                         /* PF6 */
    -32768,                         /* W7 */
    -32768,                         /* PF7 */
    -32768,                         /* W8 */
    -32768,                         /* PF8 */
    -32768,                         /* W9 */
    -32768,                         /* PF9 */
    -32768,                         /* W10 */
    -32768,                         /* PF10 */
    -32768,                         /* W11 */
    -32768,                         /* PF11 */
    -32768,                         /* W12 */
    -32768,                         /* PF12 */
    -32768,                         /* W13 */
    -32768,                         /* PF13 */
    -32768,                         /* W14 */
    -32768,                         /* PF14 */
    -32768,                         /* W15 */
    -32768,                         /* PF15 */
    -32768,                         /* W16 */
    -32768,                         /* PF16 */
    -32768,                         /* W17 */
    -32768,                         /* PF17 */
    -32768,                         /* W18 */
    -32768,                         /* PF18 */
    -32768,                         /* W19 */
    -32768,                         /* PF19 */
    -32768,                         /* W20 */
    -32768,                         /* PF20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpPT1Tms */
    1400,                           /* RmpDecTmm */
    1400,                           /* RmpIncTmm */
    0,                              /* ReadOnly */
    -32768,                         /* Pad */
    4,                              /* ActPt */
    0,                              /* W1 */
    930,                            /* PF1 */
    10,                             /* W2 */
    940,                            /* PF2 */
    90,                             /* W3 */
    950,                            /* PF3 */
    100,                            /* W4 */
    960,                            /* PF4 */
    -32768,                         /* W5 */
    -32768,                         /* PF5 */
    -32768,                         /* W6 */
    -32768,                         /* PF6 */
    -32768,                         /* W7 */
    -32768,                         /* PF7 */
    -32768,                         /* W8 */
    -32768,                         /* PF8 */
    -32768,                         /* W9 */
    -32768,                         /* PF9 */
    -32768,                         /* W10 */
    -32768,                         /* PF10 */
    -32768,                         /* W11 */
    -32768,                         /* PF11 */
    -32768,                         /* W12 */
    -32768,                         /* PF12 */
    -32768,                         /* W13 */
    -32768,                         /* PF13 */
    -32768,                         /* W14 */
    -32768,                         /* PF14 */
    -32768,                         /* W15 */
    -32768,                         /* PF15 */
    -32768,                         /* W16 */
    -32768,                         /* PF16 */
    -32768,                         /* W17 */
    -32768,                         /* PF17 */
    -32768,                         /* W18 */
    -32768,                         /* PF18 */
    -32768,                         /* W19 */
    -32768,                         /* PF19 */
    -32768,                         /* W20 */
    -32768,                         /* PF20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpPT1Tms */
    1400,                           /* RmpDecTmm */
    1400,                           /* RmpIncTmm */
    0,                              /* ReadOnly */
    -32768,                         /* Pad */
    4,                              /* ActPt */
    0,                              /* W1 */
    930,                            /* PF1 */
    10,                             /* W2 */
    940,                            /* PF2 */
    90,                             /* W3 */
    950,                            /* PF3 */
    100,                            /* W4 */
    960,                            /* PF4 */
    -32768,                         /* W5 */
    -32768,                         /* PF5 */
    -32768,                         /* W6 */
    -32768,                         /* PF6 */
    -32768,                         /* W7 */
    -32768,                         /* PF7 */
    -32768,                         /* W8 */
    -32768,                         /* PF8 */
    -32768,                         /* W9 */
    -32768,                         /* PF9 */
    -32768,                         /* W10 */
    -32768,                         /* PF10 */
    -32768,                         /* W11 */
    -32768,                         /* PF11 */
    -32768,                         /* W12 */
    -32768,                         /* PF12 */
    -32768,                         /* W13 */
    -32768,                         /* PF13 */
    -32768,                         /* W14 */
    -32768,                         /* PF14 */
    -32768,                         /* W15 */
    -32768,                         /* PF15 */
    -32768,                         /* W16 */
    -32768,                         /* PF16 */
    -32768,                         /* W17 */
    -32768,                         /* PF17 */
    -32768,                         /* W18 */
    -32768,                         /* PF18 */
    -32768,                         /* W19 */
    -32768,                         /* PF19 */
    -32768,                         /* W20 */
    -32768,                         /* PF20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpPT1Tms */
    1400,                           /* RmpDecTmm */
    1400,                           /* RmpIncTmm */
    0,                              /* ReadOnly */
    -32768,                         /* Pad */
    4,                              /* ActPt */
    0,                              /* W1 */
    930,                            /* PF1 */
    10,                             /* W2 */
    940,                            /* PF2 */
    90,                             /* W3 */
    950,                            /* PF3 */
    100,                            /* W4 */
    960,                            /* PF4 */
    -32768,                         /* W5 */
    -32768,                         /* PF5 */
    -32768,                         /* W6 */
    -32768,                         /* PF6 */
    -32768,                         /* W7 */
    -32768,                         /* PF7 */
    -32768,                         /* W8 */
    -32768,                         /* PF8 */
    -32768,                         /* W9 */
    -32768,                         /* PF9 */
    -32768,                         /* W10 */
    -32768,                         /* PF10 */
    -32768,                         /* W11 */
    -32768,                         /* PF11 */
    -32768,                         /* W12 */
    -32768,                         /* PF12 */
    -32768,                         /* W13 */
    -32768,                         /* PF13 */
    -32768,                         /* W14 */
    -32768,                         /* PF14 */
    -32768,                         /* W15 */
    -32768,                         /* PF15 */
    -32768,                         /* W16 */
    -32768,                         /* PF16 */
    -32768,                         /* W17 */
    -32768,                         /* PF17 */
    -32768,                         /* W18 */
    -32768,                         /* PF18 */
    -32768,                         /* W19 */
    -32768,                         /* PF19 */
    -32768,                         /* W20 */
    -32768,                         /* PF20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpPT1Tms */
    1400,                           /* RmpDecTmm */
    1400,                           /* RmpIncTmm */
    0,                              /* ReadOnly */
    -32768,                         /* Pad */
    /* model 132 */
    132, 226,
    1,                              /* ActCrv */
    0,                              /* ModEna */
    65535,                          /* WinTms */
    600,                            /* RvrtTms */
    65535,                          /* RmpTms */
    4,                              /* NCrv */
    3,                              /* NPt */
    -1,                             /* V_SF */
    0,                              /* DeptRef_SF */
    0,                              /* RmpIncDec_SF */
    3,                              /* ActPt */
    2,                              /* DeptRef */
    950,                            /* V1 */
    100,                            /* W1 */
    1000,                           /* V2 */
    100,                            /* W2 */
    1100,                           /* V3 */
    0,                              /* W3 */
    65535,                          /* V4 */
    -32768,                         /* W4 */
    65535,                          /* V5 */
    -32768,                         /* W5 */
    65535,                          /* V6 */
    -32768,                         /* W6 */
    65535,                          /* V7 */
    -32768,                         /* W7 */
    65535,                          /* V8 */
    -32768,                         /* W8 */
    65535,                          /* V9 */
    -32768,                         /* W9 */
    65535,                          /* V10 */
    -32768,                         /* W10 */
    65535,                          /* V11 */
    -32768,                         /* W11 */
    65535,                          /* V12 */
    -32768,                         /* W12 */
    65535,                          /* V13 */
    -32768,                         /* W13 */
    65535,                          /* V14 */
    -32768,                         /* W14 */
    65535,                          /* V15 */
    -32768,                         /* W15 */
    65535,                          /* V16 */
    -32768,                         /* W16 */
    65535,                          /* V17 */
    -32768,                         /* W17 */
    65535,                          /* V18 */
    -32768,                         /* W18 */
    65535,                          /* V19 */
    -32768,                         /* W19 */
    65535,                          /* V20 */
    -32768,                         /* W20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpPt1Tms */
    0,                              /* RmpDecTmm */
    0,                              /* RmpIncTmm */
    0,                              /* ReadOnly */
    3,                              /* ActPt */
    2,                              /* DeptRef */
    1000,                           /* V1 */
    100,                            /* W1 */
    1100,                           /* V2 */
    100,                            /* W2 */
    1130,                           /* V3 */
    0,                              /* W3 */
    65535,                          /* V4 */
    -32768,                         /* W4 */
    65535,                          /* V5 */
    -32768,                         /* W5 */
    65535,                          /* V6 */
    -32768,                         /* W6 */
    65535,                          /* V7 */
    -32768,                         /* W7 */
    65535,                          /* V8 */
    -32768,                         /* W8 */
    65535,                          /* V9 */
    -32768,                         /* W9 */
    65535,                          /* V10 */
    -32768,                         /* W10 */
    65535,                          /* V11 */
    -32768,                         /* W11 */
    65535,                          /* V12 */
    -32768,                         /* W12 */
    65535,                          /* V13 */
    -32768,                         /* W13 */
    65535,                          /* V14 */
    -32768,                         /* W14 */
    65535,                          /* V15 */
    -32768,                         /* W15 */
    65535,                          /* V16 */
    -32768,                         /* W16 */
    65535,                          /* V17 */
    -32768,                         /* W17 */
    65535,                          /* V18 */
    -32768,                         /* W18 */
    65535,                          /* V19 */
    -32768,                         /* W19 */
    65535,                          /* V20 */
    -32768,                         /* W20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpPt1Tms */
    80,                             /* RmpDecTmm */
    80,                             /* RmpIncTmm */
    0,                              /* ReadOnly */
    3,                              /* ActPt */
    2,                              /* DeptRef */
    1000,                           /* V1 */
    100,                            /* W1 */
    1100,                           /* V2 */
    100,                            /* W2 */
    1130,                           /* V3 */
    0,                              /* W3 */
    65535,                          /* V4 */
    -32768,                         /* W4 */
    65535,                          /* V5 */
    -32768,                         /* W5 */
    65535,                          /* V6 */
    -32768,                         /* W6 */
    65535,                          /* V7 */
    -32768,                         /* W7 */
    65535,                          /* V8 */
    -32768,                         /* W8 */
    65535,                          /* V9 */
    -32768,                         /* W9 */
    65535,                          /* V10 */
    -32768,                         /* W10 */
    65535,                          /* V11 */
    -32768,                         /* W11 */
    65535,                          /* V12 */
    -32768,                         /* W12 */
    65535,                          /* V13 */
    -32768,                         /* W13 */
    65535,                          /* V14 */
    -32768,                         /* W14 */
    65535,                          /* V15 */
    -32768,                         /* W15 */
    65535,                          /* V16 */
    -32768,                         /* W16 */
    65535,                          /* V17 */
    -32768,                         /* W17 */
    65535,                          /* V18 */
    -32768,                         /* W18 */
    65535,                          /* V19 */
    -32768,                         /* W19 */
    65535,                          /* V20 */
    -32768,                         /* W20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpPt1Tms */
    80,                             /* RmpDecTmm */
    80,                             /* RmpIncTmm */
    0,                              /* ReadOnly */
    3,                              /* ActPt */
    2,                              /* DeptRef */
    1000,                           /* V1 */
    100,                            /* W1 */
    1100,                           /* V2 */
    100,                            /* W2 */
    1130,                           /* V3 */
    0,                              /* W3 */
    65535,                          /* V4 */
    -32768,                         /* W4 */
    65535,                          /* V5 */
    -32768,                         /* W5 */
    65535,                          /* V6 */
    -32768,                         /* W6 */
    65535,                          /* V7 */
    -32768,                         /* W7 */
    65535,                          /* V8 */
    -32768,                         /* W8 */
    65535,                          /* V9 */
    -32768,                         /* W9 */
    65535,                          /* V10 */
    -32768,                         /* W10 */
    65535,                          /* V11 */
    -32768,                         /* W11 */
    65535,                          /* V12 */
    -32768,                         /* W12 */
    65535,                          /* V13 */
    -32768,                         /* W13 */
    65535,                          /* V14 */
    -32768,                         /* W14 */
    65535,                          /* V15 */
    -32768,                         /* W15 */
    65535,                          /* V16 */
    -32768,                         /* W16 */
    65535,                          /* V17 */
    -32768,                         /* W17 */
    65535,                          /* V18 */
    -32768,                         /* W18 */
    65535,                          /* V19 */
    -32768,                         /* W19 */
    65535,                          /* V20 */
    -32768,                         /* W20 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     /* CrvNam -  */
    65535,                          /* RmpPt1Tms */
    80,                             /* RmpDecTmm */
    80,                             /* RmpIncTmm */
    0,                              /* ReadOnly */

    0xffff, 0                       /* end model id and length */
};

void
example()
{
    suns_device_t *device;
    suns_err_t err;
    inv_volt_var_curve_t curve;
    uint16_t i;
    uint16_t wgra;
    inv_fixed_pf_t fixed_pf;
    inv_max_power_t max_power;
    inv_connect_t connect;
    inv_status_t status;
    inv_mod_t mod;

    /* allocate device */
    if ((device = suns_device_alloc()) == NULL) {
        printf("Error allocating device\n");
        return;
    }

    /* set device interface */
    if ((err = suns_device_sim(device, 40000, inv_test_device, sizeof(inv_test_device), 1)) != SUNS_ERR_OK) {
        printf("Error %d setting device interface\n", err);
        return;
    }

    /* scan device */
    if ((err = suns_device_scan(device)) != SUNS_ERR_OK) {
        printf("Error %d scanning device\n", err);
        return;
    }

    /* volt var get status */
    printf("\nvolt var get status:\n");

    if ((err = inv_volt_var_get_status(device, &mod)) == SUNS_ERR_OK) {
        printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);
    } else {
        printf("Error %d getting volt var status\n", err);
    }

    /* volt var enable curve 2 */
    printf("\nvolt var enable curve 2\n");

    if ((err = inv_volt_var_enable(device, 2, 0)) != SUNS_ERR_OK) {
        printf("Error %d enabling volt var\n", err);
    }

    /* volt var get status */
    printf("\nvolt var get status:\n");

    if ((err = inv_volt_var_get_status(device, &mod)) == SUNS_ERR_OK) {
        printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);
    } else {
        printf("Error %d getting volt var status\n", err);
    }

    /* volt var get curve 1 */
    printf("\nvolt var get curve 1:\n");

    if ((err = inv_volt_var_get_curve(device, 1, &curve)) == SUNS_ERR_OK) {
        for (i = 0; i < curve.points; i++) {
            printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
        }
    } else {
        printf("Error %d getting volt var curve\n", err);
    }

    /* volt var get curve 2 */
    printf("\nvolt var get curve 2:\n");

    if ((err = inv_volt_var_get_curve(device, 2, &curve)) == SUNS_ERR_OK) {
        for (i = 0; i < curve.points; i++) {
            printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
        }
    } else {
        printf("Error %d getting volt var curve\n", err);
    }

    /* volt var get curve 3 */
    printf("\nvolt var get curve 3:\n");

    if ((err = inv_volt_var_get_curve(device, 3, &curve)) == SUNS_ERR_OK) {
        for (i = 0; i < curve.points; i++) {
            printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
        }
    } else {
        printf("Error %d getting volt var curve\n", err);
    }

    /* volt var get curve 4 */
    printf("\nvolt var get curve 4:\n");

    if ((err = inv_volt_var_get_curve(device, 4, &curve)) == SUNS_ERR_OK) {
        for (i = 0; i < curve.points; i++) {
            printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
        }
    } else {
        printf("Error %d getting volt var curve\n", err);
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

    printf("\nvolt var set curve 1:\n");
    printf("dept_ref = %d\n", curve.dept_ref);
    for (i = 0; i < curve.points; i++) {
        printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
    }

    if ((err = inv_volt_var_set_curve(device, 1, &curve)) != SUNS_ERR_OK) {
        printf("Error %d setting volt var curve\n", err);
    }

    /* volt var get curve 1 */
    printf("\nvolt var get curve 1:\n");

    if ((err = inv_volt_var_get_curve(device, 1, &curve)) == SUNS_ERR_OK) {
        printf("dept_ref = %d\n", curve.dept_ref);
        for (i = 0; i < curve.points; i++) {
            printf("%02d: v = %f  var = %f\n", i, curve.v[i], curve.var[i]);
        }
    } else {
        printf("Error %d getting volt var curve\n", err);
    }

    /* volt var disable */
    printf("\nvolt var disable\n");

    if ((err = inv_volt_var_disable(device)) != SUNS_ERR_OK) {
        printf("Error %d enabling volt var\n", err);
    }

    /* volt var get status */
    printf("\nvolt var get status:\n");

    if ((err = inv_volt_var_get_status(device, &mod)) == SUNS_ERR_OK) {
        printf("mod_ena = %d  act_crv = %d\n", mod.mod_ena, mod.act_crv);
    } else {
        printf("Error %d getting volt var status\n", err);
    }

    /* get WGra */
    printf("\nget WGra:\n");

    if ((err = inv_get_wgra(device, &wgra)) == SUNS_ERR_OK) {
        printf("WGra = %d\n", wgra);
    } else {
        printf("Error %d getting WGra\n", err);
    }

    /* set WGra */
    printf("\nset WGra to 80\n");

    if ((err = inv_set_wgra(device, 80)) != SUNS_ERR_OK) {
        printf("Error %d setting WGra\n", err);
    }

    /* get WGra */
    printf("\nget WGra:\n");

    if ((err = inv_get_wgra(device, &wgra)) == SUNS_ERR_OK) {
        printf("WGra = %d\n", wgra);
    } else {
        printf("Error %d getting WGra\n", err);
    }

    /* get fixed power factor */
    printf("\nget fixed power factor:\n");

    if ((err = inv_get_fixed_pf(device, &fixed_pf)) == SUNS_ERR_OK) {
        printf("enabled = %d\n", fixed_pf.enabled);
        printf("pf = %f\n", fixed_pf.pf);
        printf("win_tms_valid = %d\n", fixed_pf.timers.win_tms_valid);
        printf("win_tms = %d\n", fixed_pf.timers.win_tms);
        printf("rvrt_tms_valid = %d\n", fixed_pf.timers.rvrt_tms_valid);
        printf("rvrt_tms = %d\n", fixed_pf.timers.rvrt_tms);
        printf("rmp_tms_valid = %d\n", fixed_pf.timers.rmp_tms_valid);
        printf("rmp_tms = %d\n", fixed_pf.timers.rmp_tms);
    } else {
        printf("Error %d getting fixed pf\n", err);
    }

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

    if ((err = inv_set_fixed_pf(device, &fixed_pf)) != SUNS_ERR_OK) {
        printf("Error %d setting fixed pf\n", err);
    }

    /* get fixed power factor */
    printf("\nget fixed power factor:\n");

    /* get fixed power factor */
    if ((err = inv_get_fixed_pf(device, &fixed_pf)) == SUNS_ERR_OK) {
        printf("enabled = %d\n", fixed_pf.enabled);
        printf("pf = %f\n", fixed_pf.pf);
        printf("win_tms_valid = %d\n", fixed_pf.timers.win_tms_valid);
        printf("win_tms = %d\n", fixed_pf.timers.win_tms);
        printf("rvrt_tms_valid = %d\n", fixed_pf.timers.rvrt_tms_valid);
        printf("rvrt_tms = %d\n", fixed_pf.timers.rvrt_tms);
        printf("rmp_tms_valid = %d\n", fixed_pf.timers.rmp_tms_valid);
        printf("rmp_tms = %d\n", fixed_pf.timers.rmp_tms);
    } else {
        printf("Error %d getting fixed pf\n", err);
    }

    /* get max power */
    printf("\nget max power:\n");

    /* get max power */
    if ((err = inv_get_max_power(device, &max_power)) == SUNS_ERR_OK) {
        printf("enabled = %d\n", max_power.enabled);
        printf("power = %d\n", max_power.power);
        printf("win_tms_valid = %d\n", max_power.timers.win_tms_valid);
        printf("win_tms = %d\n", max_power.timers.win_tms);
        printf("rvrt_tms_valid = %d\n", max_power.timers.rvrt_tms_valid);
        printf("rvrt_tms = %d\n", max_power.timers.rvrt_tms);
        printf("rmp_tms_valid = %d\n", max_power.timers.rmp_tms_valid);
        printf("rmp_tms = %d\n", max_power.timers.rmp_tms);
    } else {
        printf("Error %d getting max power\n", err);
    }

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

    if ((err = inv_set_max_power(device, &max_power)) != SUNS_ERR_OK) {
        printf("Error %d setting max power\n", err);
    }

    /* get max power */
    printf("\nget max power:\n");

    /* get max power */
    if ((err = inv_get_max_power(device, &max_power)) == SUNS_ERR_OK) {
        printf("enabled = %d\n", max_power.enabled);
        printf("power = %d\n", max_power.power);
        printf("win_tms_valid = %d\n", max_power.timers.win_tms_valid);
        printf("win_tms = %d\n", max_power.timers.win_tms);
        printf("rvrt_tms_valid = %d\n", max_power.timers.rvrt_tms_valid);
        printf("rvrt_tms = %d\n", max_power.timers.rvrt_tms);
        printf("rmp_tms_valid = %d\n", max_power.timers.rmp_tms_valid);
        printf("rmp_tms = %d\n", max_power.timers.rmp_tms);
    } else {
        printf("Error %d getting max power\n", err);
    }

    /* get connect */
    printf("\nget connect:\n");

    if ((err = inv_get_connect(device, &connect)) == SUNS_ERR_OK) {
        printf("conn = %d\n", connect.conn);
        printf("win_tms_valid = %d\n", max_power.timers.win_tms_valid);
        printf("win_tms = %d\n", max_power.timers.win_tms);
        printf("rvrt_tms_valid = %d\n", max_power.timers.rvrt_tms_valid);
        printf("rvrt_tms = %d\n", max_power.timers.rvrt_tms);
    } else {
        printf("Error %d getting connect\n", err);
    }

    printf("\ndisconnect (set conn to 0)\n");

    connect.conn = 0;
    connect.timers.win_tms_valid = 1;
    connect.timers.win_tms = 40;
    connect.timers.rvrt_tms_valid = 1;
    connect.timers.rvrt_tms = 50;

    if ((err = inv_set_connect(device, &connect)) != SUNS_ERR_OK) {
        printf("Error %d setting connect\n", err);
    }

    /* get connect */
    printf("\nget connect:\n");

    if ((err = inv_get_connect(device, &connect)) == SUNS_ERR_OK) {
        printf("conn = %d\n", connect.conn);
        printf("win_tms_valid = %d\n", max_power.timers.win_tms_valid);
        printf("win_tms = %d\n", max_power.timers.win_tms);
        printf("rvrt_tms_valid = %d\n", max_power.timers.rvrt_tms_valid);
        printf("rvrt_tms = %d\n", max_power.timers.rvrt_tms);
    } else {
        printf("Error %d getting connect\n", err);
    }

    /* get status */
    printf("\nget status:\n");

    if ((err = inv_get_status(device, &status)) == SUNS_ERR_OK) {
        printf("power = %f\n", status.power);
        printf("pf = %f\n", status.pf);
        printf("total_energy = %f\n", status.total_energy);
        printf("alarm = 0x%08x\n", status.alarm);
    } else {
        printf("Error %d getting status\n", err);
    }
}
