#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mphal.h"

#include <applibs/gpio.h>
#include <applibs/log.h>
#define DEBUG_LED Log_Debug

#define MAX_LED 3
#define RED 8
#define GREEN 9
#define BLUE 10

#define LED_ON 0
#define LED_OFF 1

typedef struct led_obj_s
{
    mp_obj_base_t base;
    int id;
    int fd;
    int val;
} led_obj_t;

extern const mp_obj_type_t led_type;

static led_obj_t led_obj[MAX_LED] = {
    {{&led_type}, RED, -1, LED_OFF},
    {{&led_type}, GREEN, -1, LED_OFF},
    {{&led_type}, BLUE, -1, LED_OFF},
};

STATIC void led_print(const mp_print_t *print, mp_obj_t SELF, mp_print_kind_t kind)
{
    led_obj_t *self = MP_OBJ_TO_PTR(SELF);
    mp_printf(print, "LED ( id = %d, fd = %d )", self->id, self->fd);
}

/// \classmethod \constructor(id, ...)
STATIC mp_obj_t led_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args)
{
    led_obj_t *obj;
    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);
    int id = mp_obj_get_int(args[0]);
    /* find object */
    int i;
    for (i = 0; i < MAX_LED; i++)
    {
        if (id == led_obj[i].id)
            goto next;
    }
    nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "LED ( %d ) doesn't exist", id));
    return mp_const_none;

/* get object */
next:
    obj = &led_obj[i];
    obj->fd = GPIO_OpenAsOutput((GPIO_Id)obj->id, (GPIO_OutputMode_Type)0, (GPIO_Value_Type)obj->val);
    return MP_OBJ_FROM_PTR(obj);
}

/* LED CLASS */

mp_obj_t led_on(mp_obj_t SELF)
{
    led_obj_t *self = MP_OBJ_TO_PTR(SELF);
    GPIO_SetValue(self->fd, (GPIO_Value_Type)LED_ON);
    self->val = LED_ON;
    return mp_const_none;
}

mp_obj_t led_off(mp_obj_t SELF)
{
    led_obj_t *self = MP_OBJ_TO_PTR(SELF);
    GPIO_SetValue(self->fd, (GPIO_Value_Type)LED_OFF);
    self->val = LED_OFF;
    return mp_const_none;
}

mp_obj_t led_toggle(mp_obj_t SELF)
{
    led_obj_t *self = MP_OBJ_TO_PTR(SELF);
    self->val ^= 1;
    GPIO_SetValue(self->fd, (GPIO_Value_Type)self->val);
    return mp_const_none;
}

mp_obj_t led_set(mp_obj_t SELF, mp_obj_t VAL)
{
    led_obj_t *self = MP_OBJ_TO_PTR(SELF);
    int val = mp_obj_get_int(VAL);
    GPIO_SetValue(self->fd, (GPIO_Value_Type)val);
    self->val = val;
    return mp_const_none;
}

mp_obj_t led_get(mp_obj_t SELF)
{
    led_obj_t *self = MP_OBJ_TO_PTR(SELF);
    return mp_obj_new_int(self->val);
}

/* LED CLASS */
STATIC MP_DEFINE_CONST_FUN_OBJ_2(obj_led_set, led_set);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(obj_led_get, led_get);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(obj_led_on, led_on);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(obj_led_off, led_off);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(obj_led_toggle, led_toggle);

STATIC const mp_rom_map_elem_t led_locals_table[] = {
    {MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&obj_led_set)},
    {MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&obj_led_get)},
    {MP_ROM_QSTR(MP_QSTR_on), MP_ROM_PTR(&obj_led_on)},
    {MP_ROM_QSTR(MP_QSTR_off), MP_ROM_PTR(&obj_led_off)},
    {MP_ROM_QSTR(MP_QSTR_toggle), MP_ROM_PTR(&obj_led_toggle)},
//const
#define C(name)                                       \
    {                                                 \
        MP_ROM_QSTR(MP_QSTR_##name), MP_ROM_INT(name) \
    }
    C(RED),
    C(GREEN),
    C(BLUE),
#undef C
};
STATIC MP_DEFINE_CONST_DICT(led_locals, led_locals_table);

const mp_obj_type_t led_type = {
    {&mp_type_type},
    .name = MP_QSTR_LED,
    .print = led_print,
    .make_new = led_make_new,
    .locals_dict = (mp_obj_dict_t *)&led_locals,
};
