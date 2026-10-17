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

    dst = &_sbss;
    while (dst < &_ebss){

        *dst = 0; 
        dst++;

    }

    main();

    while (1) {
    }
}


void Default_Handler(void) {
    while (1) {
    }
}


typedef void (*handler_t)(void);

__attribute__((section(".isr_vector"), used))
const handler_t vector_table[] = {
    (handler_t)&_estack,   
    Reset_Handler,         
    Default_Handler,       
    Default_Handler,       
};