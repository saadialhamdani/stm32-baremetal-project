

Bare-metal firmware for the NUCLEO-F401RE (STM32F401RE, Cortex-M4), written from scratch with no HAL, CubeIDE, or vendor startup files. The first project blinks the onboard LED (PA5) using a custom linker script, hand-written C startup code (vector table, .data copy, .bss zero), and direct register writes to RCC and GPIOA.

Build: `cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=arm-gcc-toolchain.cmake && cmake --build build` (needs arm-none-eabi-gcc). Flash by copying `build/blinky.bin` to the Nucleo's USB drive.
