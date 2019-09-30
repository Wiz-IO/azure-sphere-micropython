#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include "py/obj.h"

void delay_us(unsigned int us)
{
    struct timespec st;
    st.tv_sec = us / 1000000;
    st.tv_nsec = us % 1000000 * 1000;
    nanosleep(&st, NULL);
}

void delay_ms(unsigned int ms)
{
    struct timespec st;
    st.tv_sec = ms / 1000;
    st.tv_nsec = ms % 1000 * 1000000;
    nanosleep(&st, NULL);
}

STATIC mp_obj_t mod_time_sleep(mp_obj_t arg)
{
#if MICROPY_PY_BUILTINS_FLOAT
    mp_float_t val = mp_obj_get_float(arg);
    if (val > 0.001)
        delay_us(val * 1000000);
    else
        delay_ms(val * 1000);
#else
    sleep(mp_obj_get_int(arg));
#endif
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_time_sleep_obj, mod_time_sleep);

STATIC const mp_rom_map_elem_t time_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_utime)},

    {MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&mod_time_sleep_obj)},

};

STATIC MP_DEFINE_CONST_DICT(time_module_globals, time_module_globals_table);

const mp_obj_module_t mp_module_utime = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&time_module_globals,
};