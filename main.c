#include <stdint.h>

//RCC_AHB1ENR 0X40023800
//GPIOA ODR 0x40020014
//GPIOA_MODER 0X40020000
#define GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define GPIOA_ODR (*(volatile uint32_t*)0x40020014)

int main(void) {
    RCC_AHB1ENR |= (1U << 0);
    GPIOA_MODER &= (3U << 11);
    GPIOA_MODER |= (1U <<10);


    while(1){
        GPIOA_ODR ^= (1U << 5);
        for (volatile uint32_t i = 0; i < 500000; i++);

    }
    

}