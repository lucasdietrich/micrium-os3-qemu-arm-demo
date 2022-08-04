# Micrium OS 3 demo for QEMU (Cortex M3)

Use another target to run the OS: 
- `netduinoplus2`: https://github.com/qemu/qemu/blob/079b1252e9de384385c9da910262312ec2e574c8/include/hw/arm/stm32f405_soc.h
  - More SRAM (192kB)
  - More uarts
- Add MPS2 AN386 board for emulated Cortex-M4 test coverage: https://github.com/zephyrproject-rtos/zephyr/issues/45319

- [weston-embedded / uC-OS3](https://github.com/weston-embedded/uC-OS3)
- [µC documentation Spaces](https://micrium.atlassian.net/wiki/spaces)
  - [µC/OS-III Documentation](https://micrium.atlassian.net/wiki/spaces/osiiidoc/overview?homepageId=132386)
- [CMSIS-Core (Cortex-M)](https://arm-software.github.io/CMSIS_5/Core/html/index.html)

Using `qemu-system-arm` machine `lm3s6965evb` Stellaris LM3S6965EVB (Cortex-M3).
- Armv7-M
- `OS_CPU_SysTickHandler` (uC-OS3/Ports/ARM-Cortex-M/ARMv7-M/os_cpu_c.c) must be placed on entry 15 of the Cortex-M vector table.
- `OS_CPU_PendSVHandler` must be placed on entry 14 of the Cortex-M vector table.
- `FCPU` Internal oscillator frequency (Hz) is 12MHz.
  - Value can be found in datasheet and in file zephyr/dts/arm/ti/lm3s6965.dtsi from zephyr project tree.


This file is also interesting:
- zephyr/boards/arm/qemu_cortex_m3/qemu_cortex_m3.dts

## Questions/issues/notes
- Codespace starts at 0x00000000
- I think sysclock didn't started

## Build
Build the project with `make` command.

## Emulate/debug in QEMU

Simply build and run `make qemu` then `F5`.

Or this is the typical command to run QEMU:
```
qemu-system-arm -cpu cortex-m3 -machine lm3s6965evb -semihosting -s -S -nographic -device loader,file=build/ucos3-qemu-app.elf
# -S for stop on startup
# -s flag for debug on port 1234
```


## Ideas for testing
- Increase CPU frequency to 50Mhz
- SRAM/ROM size for networking
- TCP/IP official stack bugs
- Porting code to QEMU (IAR or GCC) ? Custom section in code/linker script ?
- Using pipes to communicate with QEMU
  - For networking use https://github.com/marcinbor85/slip/blob/master/slip.c
    - Check: /home/lucas/zephyrproject/zephyr/drivers/net/slip.c
    - /home/lucas/zephyrproject/zephyr/drivers/net/ppp.c
  - Zephyr UART pipe: /home/lucas/zephyrproject/zephyr/drivers/console/uart_pipe.c
  - Zephyr UART stellaris: /home/lucas/zephyrproject/zephyr/drivers/console/uart_pipe.c

## Clocks

Search for `system clock` in the datasheet.

## Questions/issues
- Why codespace is at address 0x00000000?
- I think sysclock didn't started

## Versions

- QEMU emulator version 7.0.0 (v7.0.0)
