# Board `mps2_an385` (ARM Cortex-M3 AN385)

Related:
- https://docs.zephyrproject.org/2.6.0/boards/arm/mps2_an385/doc/index.html
- https://docs.zephyrproject.org/3.0.0/guides/networking/qemu_eth_setup.html#networking-with-eth-qemu
- https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/arm/mps2_an385/mps2_an385.dts

TODO:
- Add support for CCM SRAM
- Integrate SLIP protocol for networking

Specs:
- 4MB Flash
- 4MB SRAM

Other supported mps2 boards:
```
[lucas@fedora uc-os3-qemu-arm-demo]$ qemu-system-arm -machine help | grep mps
mps2-an385           ARM MPS2 with AN385 FPGA image for Cortex-M3
mps2-an386           ARM MPS2 with AN386 FPGA image for Cortex-M4
mps2-an500           ARM MPS2 with AN500 FPGA image for Cortex-M7
mps2-an505           ARM MPS2 with AN505 FPGA image for Cortex-M33
mps2-an511           ARM MPS2 with AN511 DesignStart FPGA image for Cortex-M3
mps2-an521           ARM MPS2 with AN521 FPGA image for dual Cortex-M33
mps3-an524           ARM MPS3 with AN524 FPGA image for dual Cortex-M33
mps3-an547           ARM MPS3 with AN547 FPGA image for Cortex-M55
```