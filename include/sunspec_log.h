
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

#ifndef _SUNSPEC_LOG_H_
#define _SUNSPEC_LOG_H_

#include <stdint.h>

/* log priorities map to standard syslog priorities */
#define SUNS_LOG_EMERG          0       /* system is unusable */
#define SUNS_LOG_ALERT          1       /* action must be taken immediately */
#define SUNS_LOG_CRIT           2       /* critical conditions */
#define SUNS_LOG_ERR            3       /* error conditions */
#define SUNS_LOG_WARN           4       /* warning conditions */
#define SUNS_LOG_NOTICE         5       /* normal, but significant, condition */
#define SUNS_LOG_INFO           6       /* informational message */
#define SUNS_LOG_DEBUG          7       /* debug-level message */

#ifdef __cplusplus
extern "C" {
#endif

void suns_log(int priority, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _SUNSPEC_LOG_H_ */
