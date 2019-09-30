#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "py/obj.h"

#include <applibs/gpio.h>
#include <applibs/log.h>
#define LOG_PIN 
//Log_Debug

STATIC mp_obj_t mod_pin_input(mp_obj_t ID)
{
    int id = mp_obj_get_int(ID);
    LOG_PIN("[LOG] Pin input: %d\n", id);
    int fd = GPIO_OpenAsInput((GPIO_Id)id);
    LOG_PIN("[LOG] Pin open FD = %d\n", fd);
    return mp_obj_new_int((mp_int_t)fd);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_pin_input_obj, mod_pin_input);

STATIC mp_obj_t mod_pin_output(mp_obj_t ID, mp_obj_t MODE, mp_obj_t VAL)
{
    int id = mp_obj_get_int(ID);
    int mode = mp_obj_get_int(MODE);
    int val = mp_obj_get_int(VAL);
    LOG_PIN("[LOG] Pin output: %d\n", id);
    int fd = GPIO_OpenAsOutput((GPIO_Id)id, (GPIO_OutputMode_Type)mode, (GPIO_Value_Type)val);
    LOG_PIN("[LOG] Pin open FD = %d, MODE = %d, VAL = %d\n", fd, mode, val);
    return mp_obj_new_int((mp_int_t)fd);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_pin_output_obj, mod_pin_output);

STATIC mp_obj_t mod_pin_close(mp_obj_t FD)
{
    int fd = mp_obj_get_int(FD);
    LOG_PIN("[LOG] Pin.close FD = %d\n", fd);
    if (fd > -1)
        close(fd);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_pin_close_obj, mod_pin_close);

STATIC mp_obj_t mod_pin_get(mp_obj_t FD)
{
    int error = -1;
    int fd = mp_obj_get_int(FD);
    LOG_PIN("[LOG] Pin.get FD = %d\n", fd);
    GPIO_Value_Type outValue;
    if (fd > -1)
        error = GPIO_GetValue(fd, &outValue);
    if (0 == error)
        return mp_obj_new_int((mp_int_t)outValue);
    return mp_obj_new_int((mp_int_t)-1);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_pin_get_obj, mod_pin_get);

STATIC mp_obj_t mod_pin_set(mp_obj_t FD, mp_obj_t VAL)
{
    int error = -1;
    int fd = mp_obj_get_int(FD);
    int val = mp_obj_get_int(VAL);
    LOG_PIN("[LOG] Pin.set FD = %d, VAL = %d\n", fd, val);
    if (fd > -1)
        error = GPIO_SetValue(fd, (GPIO_Value_Type)val);
    return mp_obj_new_int((mp_int_t)error);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_pin_set_obj, mod_pin_set);

STATIC const mp_rom_map_elem_t pin_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_Pin)},

    {MP_ROM_QSTR(MP_QSTR_input), MP_ROM_PTR(&mod_pin_input_obj)},
    {MP_ROM_QSTR(MP_QSTR_output), MP_ROM_PTR(&mod_pin_output_obj)},

    {MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&mod_pin_close_obj)},

    {MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&mod_pin_get_obj)},
    {MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&mod_pin_set_obj)},
};

STATIC MP_DEFINE_CONST_DICT(pin_module_globals, pin_module_globals_table);

const mp_obj_module_t mp_module_pin = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&pin_module_globals,
};