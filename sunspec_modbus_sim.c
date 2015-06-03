
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

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>

#include "sunspec_error.h"
#include "sunspec_modbus.h"
#include "sunspec_log.h"

typedef struct _suns_modbus_sim_t {
    uint16_t base_addr;
    uint16_t *sim_map;
    uint16_t sim_map_len;
    uint16_t slave_id;
} suns_modbus_sim_t;

suns_err_t
suns_modbus_sim_connect(void *prot, uint32_t timeout)
{
    if (prot == NULL) {
        return SUNS_ERR_INIT;
    }

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_sim_disconnect(void *prot)
{
    if (prot == NULL) {
        return SUNS_ERR_INIT;
    }

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_sim_read(void *prot, uint16_t addr, uint16_t len, unsigned char *buf, uint32_t timeout)
{
    uint16_t offset;
    suns_modbus_sim_t *sim = (suns_modbus_sim_t *) prot;
    uint16_t *dest = (uint16_t *) buf;
    uint16_t i;

    if ((addr < sim->base_addr) || ((addr + (len * 2)) > (sim->base_addr + sim->sim_map_len))) {
        return SUNS_ERR_RANGE;
    }

    offset = addr - sim->base_addr;
    for (i = 0; i < len; i++) {
        dest[i] = htons(sim->sim_map[offset + i]);
    }

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_sim_write(void *prot, uint16_t addr, uint16_t len, unsigned char *buf, uint32_t timeout)
{
    uint16_t offset;
    suns_modbus_sim_t *sim = (suns_modbus_sim_t *) prot;
    uint16_t *dest = (uint16_t *) buf;
    uint16_t i;

    if ((addr < sim->base_addr) || ((addr + (len * 2)) > (sim->base_addr + sim->sim_map_len))) {
        return SUNS_ERR_RANGE;
    }

    offset = addr - sim->base_addr;
    for (i = 0; i < len; i++) {
        sim->sim_map[offset + i] = ntohs(dest[i]);
    }

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_sim_close(suns_modbus_io_t *io)
{
    if (io == NULL) {
        return SUNS_ERR_INIT;
    }

    if (io->prot != NULL) {
        free(io->prot);
    }

    io->prot = NULL;
    io->connect = NULL;
    io->disconnect = NULL;
    io->read = NULL;
    io->write = NULL;
    io->close = NULL;

    return SUNS_ERR_OK;
}

suns_err_t
suns_modbus_sim_open(suns_modbus_io_t *io, uint16_t base_addr, uint16_t *sim_map, uint16_t sim_map_len, uint16_t slave_id)
{
    if (io == NULL) {
        return SUNS_ERR_INIT;
    }

    if ((io->prot = malloc(sizeof(suns_modbus_sim_t))) == NULL) {
        return SUNS_ERR_ALLOC;
    }
    ((suns_modbus_sim_t *) io->prot)->base_addr = base_addr;
    ((suns_modbus_sim_t *) io->prot)->sim_map = (uint16_t *) malloc(sim_map_len);
    memcpy((char *)((suns_modbus_sim_t *) io->prot)->sim_map, (char *) sim_map, sim_map_len);
    ((suns_modbus_sim_t *) io->prot)->sim_map_len = sim_map_len;
    ((suns_modbus_sim_t *) io->prot)->slave_id = slave_id;

    io->connect = suns_modbus_sim_connect;
    io->disconnect = suns_modbus_sim_disconnect;
    io->read = suns_modbus_sim_read;
    io->write = suns_modbus_sim_write;
    io->close = suns_modbus_sim_close;

    return SUNS_ERR_OK;
}
