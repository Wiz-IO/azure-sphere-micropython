/*
    Created on: 01.10.2019
    Author: Georgi Angelov
        http://www.wizio.eu/
        https://github.com/Wiz-IO/azure-sphere-micropython
*/  

#include <unistd.h>
#include <applibs/log.h>
#include <applibs/storage.h>
#include "py/runtime.h"

void open_uart(int N);
void py_init(void);
int do_file(const char *file);
int do_str(const char *str);

#define PY_FILE_NAME "main.py"

int main(int argc, char **argv)
{
    open_uart(4);
    Log_Debug("Azure Sphere 2019 Georgi Angelov\n");
    Log_Debug("[PYTHON] BEGIN\n\n");
    py_init();
    mp_init();
    //do_str("print(' <py> micropython (from string)'); print('')");
    do_file(PY_FILE_NAME);    
    mp_deinit();
    Log_Debug("\n[PYTHON] END\n");
    while (1)
        sleep(1);
    return 0;
}
