/*
    Created on: 01.10.2019
    Author: Georgi Angelov
        http://www.wizio.eu/
        https://github.com/Wiz-IO/azure-sphere-micropython
*/  

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>

#include <applibs/i2c.h>
#include <applibs/log.h>
#define DEBUG_I2C Log_Debug

#include "py/objtuple.h"
#include "py/objstr.h"
#include "py/runtime.h"
#include "py/stream.h"
#include "py/builtin.h"
#include "py/mphal.h"

#include "py_buffers.h"

#define I2C_DEFAULT_ADDRESS 0x6A

#define MAX_I2C 4

typedef struct i2c_obj_s
{
    mp_obj_base_t base;
    int id;
    int *pFD;
} i2c_obj_t;

extern const mp_obj_type_t i2c_type;
static int i2c_fd[MAX_I2C] = {-1, -1, -1, -1};
const i2c_obj_t i2c_obj[MAX_I2C] = {
    {{&i2c_type}, 0, &i2c_fd[0]},
    {{&i2c_type}, 1, &i2c_fd[1]},
    {{&i2c_type}, 2, &i2c_fd[2]},
    {{&i2c_type}, 3, &i2c_fd[3]},
};

STATIC void i2c_print(const mp_print_t *print, mp_obj_t SELF, mp_print_kind_t kind)
{
    i2c_obj_t *self = MP_OBJ_TO_PTR(SELF);
    mp_printf(print, "I2C ( id = %d, fd = %d )", self->id, *self->pFD);
}

/// \classmethod \constructor(id, ...)
STATIC mp_obj_t i2c_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args)
{
    //DEBUG_I2C("[I2C] NEW\n");
    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);
    int id = mp_obj_get_int(args[0]);
    if (id < 0 || id > MAX_I2C - 1)
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "I2C ( %d ) doesn't exist", id));
    const i2c_obj_t *obj = &i2c_obj[id]; // get I2C object
    return MP_OBJ_FROM_PTR(obj);
}

STATIC mp_obj_t i2c_address(mp_obj_t SELF, mp_obj_t ADDRESS)
{
    int res = -1;
    i2c_obj_t *self = MP_OBJ_TO_PTR(SELF);
    if (*self->pFD > -1)
    {
        int address = mp_obj_get_int(ADDRESS);
        res = I2CMaster_SetDefaultTargetAddress(*self->pFD, (I2C_DeviceAddress)address);
        DEBUG_I2C("[I2C] i2c_address( 0x%X )\n", address);
    }
    if (res)
    {
        DEBUG_I2C("[ERROR] i2c_address()\n");
    }
    return mp_obj_new_bool(res == 0); // True = ok
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(obj_i2c_address, i2c_address);

STATIC mp_obj_t i2c_speed(mp_obj_t SELF, mp_obj_t SPEED)
{
    int res = -1;
    i2c_obj_t *self = MP_OBJ_TO_PTR(SELF);
    if (*self->pFD > -1)
    {
        int speed = mp_obj_get_int(SPEED);
        res = I2CMaster_SetBusSpeed(*self->pFD, speed);
        DEBUG_I2C("[I2C] i2c_speed( %d )\n", speed);
    }
    if (res)
    {
        DEBUG_I2C("[ERROR] i2c_speed()\n");
    }
    return mp_obj_new_bool(res == 0); // True = ok
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(obj_i2c_speed, i2c_speed);

STATIC mp_obj_t i2c_timeout(mp_obj_t SELF, mp_obj_t MS)
{
    int res = -1;
    i2c_obj_t *self = MP_OBJ_TO_PTR(SELF);
    if (*self->pFD > -1)
    {
        int timeout = mp_obj_get_int(MS);
        res = I2CMaster_SetTimeout(*self->pFD, (uint32_t)timeout);
        DEBUG_I2C("[I2C] i2c_timeout( %d )\n", timeout);
    }
    if (res)
    {
        DEBUG_I2C("[ERROR] i2c_timeout()\n");
    }
    return mp_obj_new_bool(res == 0); // True = ok
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(obj_i2c_timeout, i2c_timeout);

// i2c.open() OR open( speed )
STATIC mp_obj_t i2c_open(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    i2c_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if (*self->pFD > -1)
    {
        DEBUG_I2C("[ERROR] i2c_open() allready\n");
        return mp_obj_new_bool(0);
    }
    *self->pFD = I2CMaster_Open((I2C_InterfaceId)self->id);
    if (*self->pFD < 0)
    {
        DEBUG_I2C("[ERROR] i2c_open()\n");
        return mp_obj_new_bool(0);
    }

    /* parse args */
    static const mp_arg_t allowed[] = {
        {MP_QSTR_address, MP_ARG_INT, {.u_int = I2C_DEFAULT_ADDRESS}},
        {MP_QSTR_speed, MP_ARG_INT, {.u_int = I2C_BUS_SPEED_STANDARD}},
        {MP_QSTR_timeout, MP_ARG_INT, {.u_int = 100}},
    };
    mp_arg_val_t ARG[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args - 1, args + 1, kw_args, MP_ARRAY_SIZE(allowed), allowed, ARG);

    /* set values, todo errors */
    i2c_address(args[0], mp_obj_new_int(ARG[0].u_int));
    i2c_speed(args[0], mp_obj_new_int(ARG[1].u_int));
    i2c_timeout(args[0], mp_obj_new_int(ARG[2].u_int));

    return mp_obj_new_bool(1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(obj_i2c_open, 1 /*n_args_min*/, i2c_open);

STATIC mp_obj_t i2c_close(mp_obj_t SELF)
{
    i2c_obj_t *self = MP_OBJ_TO_PTR(SELF);
    if (*self->pFD > -1)
    {
        close(*self->pFD);
        *self->pFD = -1;
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(obj_i2c_close, i2c_close);

STATIC mp_obj_t i2c_write(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    DEBUG_I2C("[I2C] i2c_write()\n");
    i2c_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if (*self->pFD < 0)
    {
        DEBUG_I2C("[ERROR] i2c_write() %d, %s\n", errno, strerror(errno));
        return mp_obj_new_int(0);
    }
    /* parse args */
    static const mp_arg_t allowed[] = {
        {MP_QSTR_buffer, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL}}, // ARG[0]
        {MP_QSTR_address, MP_ARG_INT, {.u_int = I2C_DEFAULT_ADDRESS}},          // ARG[1]
    };
    mp_arg_val_t ARG[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args - 1, args + 1, kw_args, MP_ARRAY_SIZE(allowed), allowed, ARG);

    /* get wBuffer */
    mp_buffer_info_t buffer_info;
    uint8_t data[1];
    get_buf_for_send(ARG[0].u_obj, &buffer_info, data); // get the buffer

    ssize_t wr = I2CMaster_Write(*self->pFD,
                                 (I2C_DeviceAddress)ARG[1].u_int, // address
                                 (const uint8_t *)buffer_info.buf,
                                 (size_t)buffer_info.len);

    DEBUG_I2C("[I2C] i2c_write( address = 0x%X, len = %d ) wr = %d\n", ARG[1].u_int, buffer_info.len, wr);
    return mp_obj_new_int((mp_int_t)wr); // writed
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(obj_i2c_write, 1 /*n_args_min*/, i2c_write);

STATIC mp_obj_t i2c_read(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    DEBUG_I2C("[I2C] i2c_read()\n");
    i2c_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if (*self->pFD < 0)
    {
        DEBUG_I2C("[ERROR] i2c_read() %d, %s\n", errno, strerror(errno));
        return mp_obj_new_int(0);
    }
    /* parse args */
    static const mp_arg_t allowed[] = {
        {MP_QSTR_buffer, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL}}, // ARG[0]
        {MP_QSTR_address, MP_ARG_INT, {.u_int = I2C_DEFAULT_ADDRESS}},          // ARG[1]
    };
    mp_arg_val_t ARG[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args - 1, args + 1, kw_args, MP_ARRAY_SIZE(allowed), allowed, ARG);

    /* make rBuffer */
    vstr_t vstr;
    mp_obj_t o_ret = get_buf_for_recv(ARG[0].u_obj, &vstr); // get the buffer

    int rd = I2CMaster_Read(*self->pFD,
                            (I2C_DeviceAddress)ARG[1].u_int,
                            (uint8_t *)vstr.buf,
                            (size_t)vstr.len);
    DEBUG_I2C("[I2C] i2c_read( address = 0x%X ) rd = %d\n", ARG[1].u_int, rd);

    /* return the received data */
    if (o_ret != MP_OBJ_NULL)
    {
        return o_ret;
    }
    else
    {
        return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(obj_i2c_read, 1 /*n_args_min*/, i2c_read);

STATIC mp_obj_t i2c_writeRead(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args)
{
    DEBUG_I2C("[I2C] i2c_writeRead()\n");
    i2c_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if (*self->pFD < 0)
    {
        DEBUG_I2C("[ERROR] i2c_writeRead() %d, %s\n", errno, strerror(errno));
        return mp_obj_new_int(0);
    }
    /* parse args */
    static const mp_arg_t allowed[] = {
        {MP_QSTR_wBuf, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL}}, // ARG[0]
        {MP_QSTR_rBuf, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL}}, // ARG[1]
        {MP_QSTR_address, MP_ARG_INT, {.u_int = I2C_DEFAULT_ADDRESS}},        // ARG[2]
    };
    mp_arg_val_t ARG[MP_ARRAY_SIZE(allowed)];
    mp_arg_parse_all(n_args - 1, args + 1, kw_args, MP_ARRAY_SIZE(allowed), allowed, ARG);

    /* get wBuffer */
    mp_buffer_info_t buffer_info;
    uint8_t data[1];
    get_buf_for_send(ARG[0].u_obj, &buffer_info, data);

    /* make rBuffer */
    vstr_t vstr;
    mp_obj_t o_ret = get_buf_for_recv(ARG[1].u_obj, &vstr);

    ssize_t res = I2CMaster_WriteThenRead(*self->pFD,
                                          (I2C_DeviceAddress)ARG[2].u_int,  // address
                                          (const uint8_t *)buffer_info.buf, // writeData
                                          (size_t)buffer_info.len,          // lenWriteData
                                          (uint8_t *)vstr.buf,              // readData
                                          (size_t)vstr.len);                // lenReadData

    DEBUG_I2C("[I2C] i2c_writeRead() res = %d\n", res);

    /* return the received data */
    if (o_ret != MP_OBJ_NULL)
    {
        return o_ret;
    }
    else
    {
        return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(obj_i2c_writeRead, 1 /*n_args_min*/, i2c_writeRead);

/* I2C CLASS */

STATIC const mp_rom_map_elem_t i2c_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&obj_i2c_open)},
    {MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&obj_i2c_close)},
    {MP_ROM_QSTR(MP_QSTR_address), MP_ROM_PTR(&obj_i2c_address)},
    {MP_ROM_QSTR(MP_QSTR_speed), MP_ROM_PTR(&obj_i2c_speed)},
    {MP_ROM_QSTR(MP_QSTR_timeout), MP_ROM_PTR(&obj_i2c_timeout)},
    {MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&obj_i2c_read)},
    {MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&obj_i2c_write)},
    {MP_ROM_QSTR(MP_QSTR_writeRead), MP_ROM_PTR(&obj_i2c_writeRead)},

//const
#define C(name)                                       \
    {                                                 \
        MP_ROM_QSTR(MP_QSTR_##name), MP_ROM_INT(name) \
    }
    C(I2C_BUS_SPEED_STANDARD),
    C(I2C_BUS_SPEED_FAST),
    C(I2C_BUS_SPEED_FAST_PLUS),
    C(I2C_BUS_SPEED_HIGH),
//C(),
#undef C
};
STATIC MP_DEFINE_CONST_DICT(i2c_locals, i2c_locals_table);

const mp_obj_type_t i2c_type = {
    {&mp_type_type},
    .name = MP_QSTR_I2C,
    .print = i2c_print,
    .make_new = i2c_make_new,
    .locals_dict = (mp_obj_dict_t *)&i2c_locals,
};
