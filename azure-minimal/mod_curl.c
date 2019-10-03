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

#include <curl/curl.h>
#include <applibs/log.h>
#define DEBUG_CURL Log_Debug

#include "py/objtuple.h"
#include "py/objstr.h"
#include "py/runtime.h"
#include "py/stream.h"
#include "py/builtin.h"
#include "py/mphal.h"

static CURL *curlHandle = NULL;
typedef struct
{
    char *data;
    size_t size;
} MemoryBlock;
static MemoryBlock block;
static size_t curl_callback(void *chunks, size_t chunkSize, size_t chunksCount, void *memoryBlock)
{
    MemoryBlock *block = (MemoryBlock *)memoryBlock;
    size_t additionalDataSize = chunkSize * chunksCount;
    block->data = (char *)realloc(block->data, block->size + additionalDataSize + 1);
    if (block->data == NULL)
    {
        DEBUG_CURL("[ERROR] CURL callback\n");
        abort(); // !?
    }
    memcpy(block->data + block->size, chunks, additionalDataSize);
    block->size += additionalDataSize;
    block->data[block->size] = 0; // Ensure the block of memory is null terminated.
    return additionalDataSize;
}

STATIC mp_obj_t curl_setopt(mp_obj_t OPT, mp_obj_t PRM)
{
    if (NULL == curlHandle)
        return mp_obj_new_bool(0); // error
    int res = -1;
    int opt = mp_obj_get_int(OPT);
    mp_obj_type_t *type = mp_obj_get_type(PRM);
    if (type == &mp_type_str)
    {
        const char *s_prm = mp_obj_str_get_str(PRM);
        res = curl_easy_setopt(curlHandle, opt, s_prm);
    }
    else if (type == &mp_type_int)
    {
        int i_prm = mp_obj_get_int(PRM);
        res = curl_easy_setopt(curlHandle, opt, i_prm);
    }
    else
    {
        DEBUG_CURL("[ERROR] setopt unknown type\n");
    }
    return mp_obj_new_bool(res == 0); // false = error
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(obj_curl_setopt, curl_setopt);

STATIC mp_obj_t curl_init(void)
{
    if (NULL == curlHandle)
    {
        if (0 == curl_global_init(CURL_GLOBAL_ALL))
        {
            if ((curlHandle = curl_easy_init()))
            {
                curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, curl_callback);
                curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &block);
                return mp_obj_new_bool(1); // ok true
            }
            else
            {
                curl_global_cleanup();
            }
        }
    }
    DEBUG_CURL("[ERROR] curl_init \n");
    return mp_obj_new_bool(0); // error false
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(obj_curl_init, curl_init);

STATIC mp_obj_t curl_end(void)
{
    if (curlHandle)
    {
        curl_easy_cleanup(curlHandle); // Clean up sample's cURL resources.
        curl_global_cleanup();         // Clean up cURL library's resources.
        curlHandle = NULL;
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(obj_curl_end, curl_end);

STATIC mp_obj_t perform(void)
{
    int res = -1;
    if (curlHandle)
    {
        res = curl_easy_perform(curlHandle);
        if (0 == res)
        {
            //DEBUG_CURL("[CURL] Response: %.*s\n", block.size, block.data);
            return mp_obj_new_str(block.data, block.size);
        }
        else
        {
            DEBUG_CURL("[ERROR] curl_easy_perform = %d\n", res);
        }
    }
    //RAISE_ERRNO(res, errno);
    return mp_obj_new_int(res); // error
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(obj_curl_perform, perform);

STATIC const mp_rom_map_elem_t mp_module_curl_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_curl)},
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&obj_curl_init)},
    {MP_ROM_QSTR(MP_QSTR_end), MP_ROM_PTR(&obj_curl_end)},
    {MP_ROM_QSTR(MP_QSTR_setopt), MP_ROM_PTR(&obj_curl_setopt)},
    {MP_ROM_QSTR(MP_QSTR_perform), MP_ROM_PTR(&obj_curl_perform)},

#define C(name)                                       \
    {                                                 \
        MP_ROM_QSTR(MP_QSTR_##name), MP_ROM_INT(name) \
    }
    C(CURLOPT_URL),
    C(CURLOPT_PORT),
    C(CURLOPT_USERAGENT),
    C(CURLOPT_HTTPGET),
    C(CURLOPT_POST),
    C(CURLOPT_POSTFIELDS),
    C(CURLOPT_PUT),
    C(CURLOPT_CONNECTTIMEOUT),
    C(CURLOPT_FOLLOWLOCATION),

    C(CURLOPT_SSL_VERIFYPEER),
    C(CURLOPT_SSLVERSION),
    C(CURL_SSLVERSION_DEFAULT),
    C(CURL_SSLVERSION_TLSv1),
    C(CURL_SSLVERSION_SSLv2),
    C(CURL_SSLVERSION_SSLv3),
    C(CURL_SSLVERSION_TLSv1_0),
    C(CURL_SSLVERSION_TLSv1_1),
    C(CURL_SSLVERSION_TLSv1_2),
    C(CURL_SSLVERSION_TLSv1_3),
    C(CURLOPT_CAINFO),
    C(CURLOPT_SSLCERT),
    C(CURLOPT_SSLKEY),

//C(),
#undef C
};
STATIC MP_DEFINE_CONST_DICT(mp_module_curl_globals, mp_module_curl_globals_table);

const mp_obj_module_t mp_module_curl = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&mp_module_curl_globals,
};