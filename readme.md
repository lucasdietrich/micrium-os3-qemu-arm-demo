# Micrium OS 3 running in QEMU

Author: Lucas Dietrich <ld.adecy@gmail.com>

- [weston-embedded / uC-OS3](https://github.com/weston-embedded/uC-OS3)
- [µC documentation Spaces](https://micrium.atlassian.net/wiki/spaces)
  - [µC/OS-III Documentation](https://micrium.atlassian.net/wiki/spaces/osiiidoc/overview?homepageId=132386)
- [CMSIS-Core (Cortex-M)](https://arm-software.github.io/CMSIS_5/Core/html/index.html)

Using `qemu-system-arm` machine `lm3s6965evb` Stellaris LM3S6965EVB (Cortex-M3).

- Armv7-M

## Ideas for testing
- SRAM/ROM size for networking
- TCP/IP official stack bugs
- Porting code to QEMU (IAR or GCC) ? Custom section in code/linker script ?

## Questions/issues
- Why codespace is at address 0x00000000?
- I think sysclock didn't started

## Versions

- QEMU emulator version 7.0.0 (v7.0.0)
