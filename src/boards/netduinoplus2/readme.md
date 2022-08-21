# Board `netduinoplus2` (STM32 - ARM Cortex M4 - ARMv7-m)

TODO:
- Add support for CCM SRAM
- Integrate SLIP protocol for networking

Specs:
- 1MB Flash
- 128KB SRAM
- 64KB CCM SRAM

QEMU support:
- UARTs

Expected output (`make run`):
```
[100%] Built target ucos3-qemu-app.elf
make -C build run --no-print-directory
0 - uart 0x40011000 - Hello from QEMU ARM stm32f405 running Micrium OS 3 !
0 - uart 0x40004400 - Hello from QEMU ARM stm32f405 running Micrium OS 3 !
1 - uart 0x40011000 - Hello from QEMU ARM stm32f405 running Micrium OS 3 !
1 - uart 0x40004400 - Hello from QEMU ARM stm32f405 running Micrium OS 3 !
2 - uart 0x40011000 - Hello from QEMU ARM stm32f405 running Micrium OS 3 !
2 - uart 0x40004400 - Hello from QEMU ARM stm32f405 running Micrium OS 3 !
3 - uart 0x40011000 - Hello from QEMU ARM stm32f405 running Micrium OS 3 !
3 - uart 0x40004400 - Hello from QEMU ARM stm32f405 running Micrium OS 3 !
```

File `build/io.txt` should have been created and should contain the 4 other UARTs data.

## Notes:

- Page 81 of RM0090 Rev 19:
> After reset, the CPU clock frequency is 16 MHz and 0 wait state (WS) is configured in the FLASH_ACR register.


## Micrium OS 3 demo for QEMU (Cortex M4)

- [QEMU - STMicroelectronics STM32 boards](https://emu.readthedocs.io/en/latest/system/arm/stm32.html)
  - https://github.com/qemu/qemu/blob/079b1252e9de384385c9da910262312ec2e574c8/include/hw/arm/stm32f405_soc.h
- Target `netduinoplus2`
- Microcontroller [STM32F405RGT6](https://www.st.com/en/microcontrollers-microprocessors/stm32f405rg.html#documentation) 
  - Arm Cortex-M4 (Armv7E-M)
  - 192 KB of SRAM

- QEMU Machine implementation:
  - https://github.com/qemu/qemu/blob/079b1252e9de384385c9da910262312ec2e574c8/hw/arm/netduinoplus2.c
  - https://github.com/qemu/qemu/blob/aab8cfd4c3614a049b60333a3747aedffbd04150/hw/arm/stm32f405_soc.c