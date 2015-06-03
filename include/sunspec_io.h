
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

#ifndef _SUNSPEC_IO_H_
#define _SUNSPEC_IO_H_

#include <stdbool.h>
#include <stdint.h>

#include "sunspec_error.h"

/* io interface functions */
typedef suns_err_t(*suns_io_close_func_t)(void *io);
typedef suns_err_t(*suns_io_connect_func_t)(void *prot, uint32_t timeout);
typedef suns_err_t(*suns_io_disconnect_func_t)(void *prot);
typedef suns_err_t(*suns_io_read_func_t)(void *prot, unsigned char *buf, uint16_t *len, uint32_t timeout);
typedef suns_err_t(*suns_io_write_func_t)(void *prot, unsigned char *buf, uint16_t len, uint32_t timeout);
typedef suns_err_t(*suns_io_flush_func_t)(void *prot, bool flush_tx, bool flush_rx);

typedef struct suns_io_t {
    suns_io_close_func_t close;
    suns_io_connect_func_t connect;
    suns_io_disconnect_func_t disconnect;
    suns_io_read_func_t read;
    suns_io_write_func_t write;
    suns_io_flush_func_t flush;
    void *prot;
} suns_io_t;

#endif /* _SUNSPEC_IO_H_ */
