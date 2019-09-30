#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define UART_STRUCTS_VERSION 1
#include <applibs/log.h>
#include <applibs/storage.h>

#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "lib/utils/pyexec.h"

bool compile_only = false;

typedef struct _mp_reader_azure_t
{
    bool close_fd;
    int fd;
    size_t len;
    size_t pos;
    byte buf[20];
} mp_reader_azure_t;

STATIC mp_uint_t mp_reader_azure_readbyte(void *data)
{
    mp_reader_azure_t *reader = (mp_reader_azure_t *)data;
    if (reader->pos >= reader->len)
    {
        if (reader->len == 0)
        {
            return MP_READER_EOF;
        }
        else
        {
            int n = read(reader->fd, reader->buf, sizeof(reader->buf));
            if (n <= 0)
            {
                reader->len = 0;
                return MP_READER_EOF;
            }
            reader->len = n;
            reader->pos = 0;
        }
    }
    return reader->buf[reader->pos++];
}

STATIC void mp_reader_azure_close(void *data)
{
    mp_reader_azure_t *reader = (mp_reader_azure_t *)data;
    if (reader->close_fd)
    {
        close(reader->fd);
    }
    m_del_obj(mp_reader_azure_t, reader);
}

void mp_reader_new_file_from_fd(mp_reader_t *reader, int fd, bool close_fd)
{
    mp_reader_azure_t *rp = m_new_obj(mp_reader_azure_t);
    rp->close_fd = close_fd;
    rp->fd = fd;
    int n = read(rp->fd, rp->buf, sizeof(rp->buf));
    if (n == -1)
    {
        if (close_fd)
        {
            close(fd);
        }
        mp_raise_OSError(errno);
    }
    rp->len = n;
    rp->pos = 0;
    reader->data = rp;
    reader->readbyte = mp_reader_azure_readbyte;
    reader->close = mp_reader_azure_close;
}

void mp_reader_new_file(mp_reader_t *reader, const char *filename)
{
    int fd = Storage_OpenFileInImagePackage(filename);
    if (fd < 0)
    {
        Log_Debug("[ERROR] Open file %s: %s\n", filename, strerror(errno));
        mp_raise_OSError(errno);
    }
    mp_reader_new_file_from_fd(reader, fd, true);
}

mp_lexer_t *mp_lexer_new_from_fd(qstr filename, int fd, bool close_fd)
{
    mp_reader_t reader;
    mp_reader_new_file_from_fd(&reader, fd, close_fd);
    return mp_lexer_new(filename, reader);
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename)
{
    mp_reader_t reader;
    mp_reader_new_file(&reader, filename);
    return mp_lexer_new(qstr_from_str(filename), reader);
}

STATIC int handle_uncaught_exception(mp_obj_base_t *exc)
{
    Log_Debug("[ERROR] Uncaught exception\n");
    return 1;
}

int execute_from_lexer(int source_kind, const void *source, mp_parse_input_kind_t input_kind, bool is_repl)
{
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0)
    {
        mp_lexer_t *lex = 0;
        if (source_kind == 0)
        {
            const char *line = source;
            lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, line, strlen(line), false);
        }
        else
        {
            lex = mp_lexer_new_from_file((const char *)source);
        }

        qstr source_name = lex->source_name;

#if MICROPY_PY___FILE__
        if (input_kind == MP_PARSE_FILE_INPUT)
        {
            mp_store_global(MP_QSTR___file__, MP_OBJ_NEW_QSTR(source_name));
        }
#endif

        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);

#if defined(MICROPY_UNIX_COVERAGE)
        // allow to print the parse tree in the coverage build
        if (mp_verbose_flag >= 3)
        {
            printf("----------------\n");
            mp_parse_node_print(parse_tree.root, 0);
            printf("----------------\n");
        }
#endif

        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, is_repl);

        if (!compile_only)
        {
            // execute it
            mp_call_function_0(module_fun);
            // check for pending exception
            if (MP_STATE_VM(mp_pending_exception) != MP_OBJ_NULL)
            {
                mp_obj_t obj = MP_STATE_VM(mp_pending_exception);
                MP_STATE_VM(mp_pending_exception) = MP_OBJ_NULL;
                nlr_raise(obj);
            }
        }

        nlr_pop();
        return 0;
    }
    else
    {
        return handle_uncaught_exception(nlr.ret_val);
    }
}

int do_file(const char *file)
{
    return execute_from_lexer(1, file, MP_PARSE_FILE_INPUT, false);
}

int do_str(const char *str)
{
    return execute_from_lexer(0, str, MP_PARSE_FILE_INPUT, false);
}