# Micrium OS 3 demo for QEMU (Cortex M3)

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

- This file is also interesting:
  - zephyr/boards/arm/qemu_cortex_m3/qemu_cortex_m3.dts

## Ideas for testing
- SRAM/ROM size for networking
- TCP/IP official stack bugs
- Porting code to QEMU (IAR or GCC) ? Custom section in code/linker script ?

## Questions/issues
- Why codespace is at address 0x00000000?
- I think sysclock didn't started

## Emulate on QEMU

Simply build and run `make qemu` then `F5`.

Or this is the typical command to run QEMU:
```
qemu-system-arm -cpu cortex-m3 -machine lm3s6965evb -semihosting -s -S -nographic -device loader,file=build/ucos3-qemu-app.elf
# -S for stop on startup
# -s flag for debug on port 1234
```

## Versions

- QEMU emulator version 7.0.0 (v7.0.0)
