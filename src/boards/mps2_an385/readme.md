# Board `mps2_an385` (ARM Cortex-M3 AN385)

TODO:
- Add support for CCM SRAM
- Integrate SLIP protocol for networking


Glossary:
- CMSDK: Code Model Software Development Kit
- FVP: Fast Models Fixed Virtual Platforms
- SMM: Soft Macro Model 

Related:
- https://docs.zephyrproject.org/2.6.0/boards/arm/mps2_an385/doc/index.html
- https://docs.zephyrproject.org/3.0.0/guides/networking/qemu_eth_setup.html#networking-with-eth-qemu
- https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/arm/mps2_an385/mps2_an385.dts

CMSIS: **https://github.com/ARMmbed/mbed-os/tree/master/targets/TARGET_ARM_FM/TARGET_FVP_MPS2/TARGET_FVP_MPS2_M3/device**
- CMSDK_CM3.h: https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_ARM_SSG/TARGET_MPS2/TARGET_MPS2_M3/device/CMSDK_CM3.h
- Drivers: https://github.com/ARMmbed/mbed-os/tree/master/targets/TARGET_ARM_FM/TARGET_FVP_MPS2

Specs:
- 4MB Flash
- 4MB SRAM

Ethernet Driver - smsc911x :
- Driver implementation
  - Zephyr RTOS: https://github.com/zephyrproject-rtos/zephyr/blob/main/drivers/ethernet/eth_smsc911x.c
  - Linux: https://elixir.bootlin.com/linux/latest/source/drivers/net/ethernet/smsc/smsc911x.c
  - mbedOS: https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_ARM_FM/TARGET_FVP_MPS2/drivers/lan91c111.c
    - https://github.com/ARMmbed/mbed-os/blob/master/targets/TARGET_ARM_FM/TARGET_FVP_MPS2/drivers/lan91c111.h
  - QEMU driver emulation: https://github.com/qemu/qemu/blob/master/hw/net/smc91c111.c
- Micrium implementation: **uC-TCP-IP/Dev/Ether/LAN911x**

```cpp
typedef struct
{
  __IO   uint32_t  DATA;                     /* Offset: 0x000 (R/W) Data Register    */
  __IO   uint32_t  STATE;                    /* Offset: 0x004 (R/W) Status Register  */
  __IO   uint32_t  CTRL;                     /* Offset: 0x008 (R/W) Control Register */
  union {
    __I    uint32_t  INTSTATUS;              /* Offset: 0x00C (R/ ) Interrupt Status Register */
    __O    uint32_t  INTCLEAR;               /* Offset: 0x00C ( /W) Interrupt Clear Register  */
    };
  __IO   uint32_t  BAUDDIV;                  /* Offset: 0x010 (R/W) Baudrate Divider Register */

} CMSDK_UART_TypeDef;
```

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