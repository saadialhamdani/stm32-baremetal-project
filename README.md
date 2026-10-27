# STM32 Bare-Metal Firmware from Scratch

Bare-metal firmware for the NUCLEO-F401RE (STM32F401RE, ARM Cortex-M4), written
without HAL, CubeMX, CubeIDE, or any vendor startup files. Everything between
reset and `main()` is in this repository.

The application itself is deliberately trivial — it blinks the onboard LED. The
point is the three pieces underneath it that are normally generated for you:
the linker script, the startup code, and the register-level peripheral access.

## What's actually here

**`STM32F401RE.ld`** — the memory map and section placement.

| Region | Origin | Length |
|---|---|---|
| FLASH | `0x08000000` | 512K |
| RAM   | `0x20000000` | 96K  |

The interesting part is `.data`. Initialised globals have to *live* in flash
(RAM is volatile) but be *addressed* in RAM. The script places the section with
`>RAM AT>FLASH`, which gives it two addresses: `_sdata` (its RAM address, where
code expects to find it) and `_sidata` (its flash address, where the initial
values are actually stored). `_estack` is set to the top of RAM, since the
Cortex-M stack grows downward.

**`startup.c`** — what runs before `main()`.

The vector table goes in its own `.isr_vector` section so the linker can place
it at the very start of flash, where the processor looks on reset. Entry 0 is
the initial stack pointer, entry 1 is `Reset_Handler`. `Reset_Handler` copies
`.data` from `_sidata` to `_sdata`, zeroes `.bss`, and calls `main()`. Without
that copy, every initialised global would hold garbage; without the zeroing, so
would every uninitialised one.

**`main.c`** — RCC and GPIOA through `volatile` pointers to memory-mapped
registers. No abstraction layer.

## Build

Requires `arm-none-eabi-gcc`, CMake 3.20+, and Ninja.

```sh
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=arm-gcc-toolchain.cmake
cmake --build build
```

Output:

```
   text	   data	    bss	    dec	    hex	filename
    220	      0	      0	    220	     dc	blinky.elf
```

220 bytes, no `.data` and no `.bss` — the whole program is code and a four-entry
vector table. The build also emits `blinky.map`, which is the quickest way to
confirm the linker put each section where the script said it should go.

## Flash

Copy `build/blinky.bin` onto the Nucleo's USB mass-storage drive. The ST-Link
firmware programs it to flash at `0x08000000`.

## Two bugs worth documenting

Both of these produced a correctly blinking LED, which is why they survived.

**Missing complement in the MODER mask.** `GPIOA_MODER &= (3U << 10)` clears
every bit *except* 11:10, rather than clearing only those two. `MODER` resets
to `0xA8000000` because PA13/PA14/PA15 come up in alternate-function mode for
SWDIO, SWCLK and JTDI — so the masked write zeroed the register and dropped the
debug pins to input mode, disabling SWD. The LED kept blinking and flashing
over USB mass storage kept working, so nothing visibly broke. Fixed with `~`.

**Missing readback after the RCC clock enable.** A write enabling a peripheral
clock may not take effect before the next instruction touches that peripheral,
in which case the access is silently dropped. A dummy read of the RCC register
stalls the bus until the write lands.

## Next

- Full vector table with real handlers, EXTI on the user button
- SysTick timebase to replace the busy-wait delay
- Bootloader: validate an application at `0x08008000`, relocate `VTOR`, jump
- Signature verification before that jump — secure boot
