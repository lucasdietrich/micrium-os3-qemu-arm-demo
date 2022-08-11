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