
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

#ifndef _SUNSPEC_ERROR_H_
#define _SUNSPEC_ERROR_H_

#include <stdint.h>

#define SUNS_ERR_OK                     0
#define SUNS_ERR_ERROR                  1
#define SUNS_ERR_ALLOC                  2
#define SUNS_ERR_UNIMPL                 3
#define SUNS_ERR_INIT                   4
#define SUNS_ERR_RANGE                  5
#define SUNS_ERR_NOT_FOUND              6
#define SUNS_ERR_NOT_SUNSPEC            7
#define SUNS_ERR_MODEL_DEF_NOT_FOUND    8
#define SUNS_ERR_MODEL_LEN              9
#define SUNS_ERR_SF_RESOLVE             10
#define SUNS_ERR_BUF_SIZE               11
#define SUNS_ERR_TYPE                   12
#define SUNS_ERR_SF                     13
#define SUNS_ERR_TIMEOUT                14
#define SUNS_ERR_MODBUS_EXCEPT          15
#define SUNS_ERR_MODBUS_RESP            16
#define SUNS_ERR_MODBUS_CRC             17
#define SUNS_ERR_BUSY                   18
#define SUNS_ERR_UNIMPLEMENTED          19
/* errno base + errno if errno is returned */
#define SUNS_ERR_ERRNO_BASE		1000

typedef uint32_t suns_err_t;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* _SUNSPEC_ERROR_H_ */
