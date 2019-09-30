#include <unistd.h>
#include <applibs/log.h>

#include "py/compile.h"
#include "py/runtime.h"

mp_import_stat_t mp_import_stat(const char *path)
{
    return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs)
{
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

void nlr_jump_fail(void *val)
{
    Log_Debug("[ERROR] nlr_jump_fail\n");
    while (1)
        sleep(1);
}

void NORETURN __fatal_error(const char *msg)
{
    Log_Debug("[ERROR] FATAL: %s\n", msg);
    while (1)
        sleep(1);
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr)
{
    Log_Debug("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif
