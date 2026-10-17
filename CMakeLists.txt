cmake_minimum_required(VERSION 3.20)
project(blinky C)

add_executable(blinky.elf startup.c main.c)

target_compile_options(blinky.elf PRIVATE
    -mcpu=cortex-m4 -mthumb -O0 -g -Wall)

target_link_options(blinky.elf PRIVATE
    -mcpu=cortex-m4 -mthumb -nostdlib
    -T${CMAKE_SOURCE_DIR}/STM32F401RE.ld
    -Wl,-Map=blinky.map)

add_custom_command(TARGET blinky.elf POST_BUILD
    COMMAND arm-none-eabi-size blinky.elf
    COMMAND arm-none-eabi-objcopy -O binary blinky.elf blinky.bin)