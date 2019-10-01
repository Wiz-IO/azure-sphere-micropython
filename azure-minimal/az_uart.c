/*
    Created on: 01.10.2019
    Author: Georgi Angelov
        http://www.wizio.eu/
        https://github.com/Wiz-IO/azure-sphere-micropython
*/        

#include <stdio.h>
#define UART_STRUCTS_VERSION 1
#include <applibs/uart.h>
#include <applibs/log.h>

int uartFd = -1;

void redirect(int fd, void *file)
{
    int *p = (int *)((char *)file + 60);
    *p = fd;
}

void open_uart(int N)
{
    UART_Config uartConfig;
    UART_InitConfig(&uartConfig);
    uartConfig.baudRate = 115200;
    uartConfig.flowControl = UART_FlowControl_None;
    uartFd = UART_Open(N, &uartConfig);
    if (uartFd > 0)
    {
        redirect(uartFd, stderr);
        redirect(uartFd, stdout);
        setvbuf(stderr, NULL, _IONBF, 0);  
        setvbuf(stdout, NULL, _IONBF, 0);          
    }
}

// Receive single character
int mp_hal_stdin_rx_chr(void) {
    unsigned char c = 0;
    //TODO
    return c;
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, uint32_t len) {
    Log_Debug("%.*s", len, str);
}
