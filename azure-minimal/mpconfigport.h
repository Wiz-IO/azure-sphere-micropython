#include <stdint.h>

// options to control how MicroPython is built

// You can disable the built-in MicroPython compiler by setting the following
// config option to 0.  If you do this then you won't get a REPL prompt, but you
// will still be able to execute pre-compiled scripts, compiled with mpy-cross.
#define MICROPY_ENABLE_COMPILER                     (1)

#define MICROPY_QSTR_BYTES_IN_HASH                  (1)
#define MICROPY_QSTR_EXTRA_POOL                     mp_qstr_frozen_const_pool
#define MICROPY_ALLOC_PATH_MAX                      (256)
#define MICROPY_ALLOC_PARSE_CHUNK_INIT              (16)
#define MICROPY_EMIT_X64                            (0)
#define MICROPY_EMIT_THUMB                          (0)
#define MICROPY_EMIT_INLINE_THUMB                   (0)
#define MICROPY_COMP_MODULE_CONST                   (0)
#define MICROPY_COMP_CONST                          (0)
#define MICROPY_COMP_DOUBLE_TUPLE_ASSIGN            (0)
#define MICROPY_COMP_TRIPLE_TUPLE_ASSIGN            (0)
#define MICROPY_MEM_STATS                           (0)
#define MICROPY_DEBUG_PRINTERS                      (0)
#define MICROPY_ENABLE_GC                           (1)
#define MICROPY_GC_ALLOC_THRESHOLD                  (0)
#define MICROPY_REPL_EVENT_DRIVEN                   (0)
#define MICROPY_HELPER_REPL                         (1)
#define MICROPY_HELPER_LEXER_UNIX                   (0)
#define MICROPY_ENABLE_SOURCE_LINE                  (0)
#define MICROPY_ENABLE_DOC_STRING                   (0)
#define MICROPY_ERROR_REPORTING                     (MICROPY_ERROR_REPORTING_TERSE)
#define MICROPY_BUILTIN_METHOD_CHECK_SELF_ARG       (0)
#define MICROPY_PY_ASYNC_AWAIT                      (0)
#define MICROPY_PY_BUILTINS_BYTEARRAY               (1)
#define MICROPY_PY_BUILTINS_DICT_FROMKEYS           (0)
#define MICROPY_PY_BUILTINS_MEMORYVIEW              (0)
#define MICROPY_PY_BUILTINS_ENUMERATE               (0)
#define MICROPY_PY_BUILTINS_FILTER                  (0)
#define MICROPY_PY_BUILTINS_FROZENSET               (0)
#define MICROPY_PY_BUILTINS_REVERSED                (0)
#define MICROPY_PY_BUILTINS_SET                     (0)
#define MICROPY_PY_BUILTINS_SLICE                   (0)
#define MICROPY_PY_BUILTINS_PROPERTY                (0)
#define MICROPY_PY_BUILTINS_MIN_MAX                 (0)
#define MICROPY_PY_BUILTINS_STR_COUNT               (0)
#define MICROPY_PY_BUILTINS_STR_OP_MODULO           (0)
#define MICROPY_PY___FILE__                         (0)
#define MICROPY_PY_GC                               (0)
#define MICROPY_PY_ARRAY                            (0)
#define MICROPY_PY_ATTRTUPLE                        (0)
#define MICROPY_PY_COLLECTIONS                      (0)
#define MICROPY_PY_MATH                             (0)
#define MICROPY_PY_CMATH                            (0)
#define MICROPY_PY_IO                               (0)
#define MICROPY_PY_STRUCT                           (0)
#define MICROPY_PY_SYS                              (0)
#define MICROPY_MODULE_FROZEN_MPY                   (1)
#define MICROPY_CPYTHON_COMPAT                      (0)
#define MICROPY_LONGINT_IMPL                        (MICROPY_LONGINT_IMPL_NONE)
#define MICROPY_FLOAT_IMPL                          (MICROPY_FLOAT_IMPL_DOUBLE)

#define MICROPY_MAKE_POINTER_CALLABLE(p)            ((void*)((mp_uint_t)(p) | 1))

#define UINT_FMT            "%u"
#define INT_FMT             "%d"
typedef int mp_int_t;       // must be pointer size
typedef unsigned mp_uint_t; // must be pointer size
typedef long mp_off_t;

#define MP_PLAT_PRINT_STRN(str, len) mp_hal_stdout_tx_strn_cooked(str, len)


#define MICROPY_PORT_BUILTINS \
    { MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&mp_builtin_open_obj) },


// Extended modules
#define MICROPY_PY_UTIME_MP_HAL (1)
extern const struct _mp_obj_module_t mp_module_utime;
extern const struct _mp_obj_module_t mp_module_pin;
extern const struct _mp_obj_module_t mp_module_socket;
#define MICROPY_PORT_BUILTIN_MODULES \
    { MP_ROM_QSTR(MP_QSTR_utime),       MP_ROM_PTR(&mp_module_utime) },   \
    { MP_ROM_QSTR(MP_QSTR_Pin),         MP_ROM_PTR(&mp_module_pin) },     \
    { MP_ROM_QSTR(MP_QSTR_usocket),     MP_ROM_PTR(&mp_module_socket) },  \
    /* eof this */



// We need to provide a declaration/definition of alloca()
#include <alloca.h>

#define MICROPY_HW_BOARD_NAME       "AZURE SPHERE"
#define MICROPY_HW_MCU_NAME         "MT3620"

#define MICROPY_MIN_USE_STDOUT      (1)

#ifdef __thumb__
#define MICROPY_MIN_USE_CORTEX_CPU (1)
#define MICROPY_MIN_USE_STM32_MCU (1)
#endif

#define MP_STATE_PORT MP_STATE_VM

#define MICROPY_PORT_ROOT_POINTERS \
    const char *readline_hist[8];
