#include <stdint.h>

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;

int main(void);

void Reset_Handler(void) {
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata)
    {
        *dst = *src;
        dst++;
        src++;
    }

    // zero .bss  <- you write this

    main();

    while (1) {
    }
}