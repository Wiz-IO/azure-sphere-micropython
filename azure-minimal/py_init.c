/*
    Created on: 01.10.2019
    Author: Georgi Angelov
        http://www.wizio.eu/
        https://github.com/Wiz-IO/azure-sphere-micropython
*/        

#include "py/gc.h"

static char *stack_top;
#if MICROPY_ENABLE_GC
static char heap[32 * 1024];
#endif

void py_init(void)
{
    int stack_dummy;
    stack_top = (char *)&stack_dummy;
#if MICROPY_ENABLE_GC
    gc_init(heap, heap + sizeof(heap));
#endif
}

void gc_collect(void)
{
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
}
