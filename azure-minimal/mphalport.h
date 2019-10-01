#include <unistd.h>

#define mp_hal_ticks_cpu() 0
static inline mp_uint_t mp_hal_ticks_ms(void) { return 0; }
static inline void mp_hal_set_interrupt_char(char c) {}

#define RAISE_ERRNO(err_flag, error_val) \
    { if (err_flag == -1) \
        { mp_raise_OSError(error_val); } }




/* ATT: next not exist in Azure Sphere, workaround */
#define F_GETFL 0
#define F_SETFL 0
static inline int fcntl(int fd, int cmd, ... ){ return -1; }
