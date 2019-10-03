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

#include <applibs/log.h>
#define DEBUG_I2C Log_Debug

#include "py/objtuple.h"
#include "py/objstr.h"
#include "py/runtime.h"
#include "py/stream.h"
#include "py/builtin.h"
#include "py/mphal.h"

/* DEVICE MODULE */

extern const mp_obj_type_t i2c_type;
extern const mp_obj_type_t led_type;

STATIC const mp_rom_map_elem_t dev_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_dev)},

    {MP_ROM_QSTR(MP_QSTR_LED), MP_ROM_PTR(&led_type)},
    {MP_ROM_QSTR(MP_QSTR_I2C), MP_ROM_PTR(&i2c_type)},
//  {MP_ROM_QSTR(MP_QSTR_SPI), MP_ROM_PTR(&spi_type)},

//const
#define C(name)                                       \
    {                                                 \
        MP_ROM_QSTR(MP_QSTR_##name), MP_ROM_INT(name) \
    }
//C(),
#undef C
};

STATIC MP_DEFINE_CONST_DICT(dev_module_globals, dev_module_globals_table);

const mp_obj_module_t mp_module_dev = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&dev_module_globals,
};