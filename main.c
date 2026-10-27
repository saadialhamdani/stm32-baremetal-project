#include <stdint.h>

/* STM32F401RE register addresses (RM0368, memory map).
 *
 *   RCC base   0x40023800   AHB1ENR at offset 0x30
 *   GPIOA base 0x40020000   MODER   at offset 0x00
 *                           ODR     at offset 0x14
 */
#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830UL)
#define GPIOA_MODER (*(volatile uint32_t *)0x40020000UL)
#define GPIOA_ODR   (*(volatile uint32_t *)0x40020014UL)

/* The onboard green LED (LD2) on the NUCLEO-F401RE is wired to PA5. */
#define LED_PIN 5

int main(void)
{
    /* Enable the GPIOA peripheral clock (AHB1ENR bit 0 = GPIOAEN). */
    RCC_AHB1ENR |= (1U << 0);

    /* Errata workaround: a write that enables a peripheral clock may not
     * have taken effect by the time the next instruction accesses that
     * peripheral, in which case the access is silently lost. Reading the
     * register back stalls the bus until the write has landed. */
    (void)RCC_AHB1ENR;

    /* MODER holds two bits per pin, so PA5 occupies bits 11:10.
     * Clear those two bits, then set them to 01 = general purpose output.
     *
     * The ~ is load-bearing. MODER resets to 0xA8000000 because PA13, PA14
     * and PA15 come up in alternate-function mode for SWDIO, SWCLK and
     * JTDI. Masking without the ~ zeroes the whole register, which drops
     * the debug pins to input mode and kills SWD access to the chip. The
     * LED still blinks, so the damage is invisible until a debugger will
     * not attach. */
    GPIOA_MODER &= ~(3U << (LED_PIN * 2));
    GPIOA_MODER |=  (1U << (LED_PIN * 2));

    while (1) {
        /* A toggle is a read-modify-write, so this has to go through ODR.
         * BSRR is atomic but can only set or clear, never flip. With no
         * interrupts enabled there is nothing here to race against. */
        GPIOA_ODR ^= (1U << LED_PIN);

        /* Crude busy-wait. volatile keeps the optimiser from deleting it.
         * The loop count is arbitrary and the resulting period depends on
         * the optimisation level, so this is a placeholder until SysTick
         * provides a real timebase. */
        for (volatile uint32_t i = 0; i < 500000U; i++) {
            /* spin */
        }
    }
}
