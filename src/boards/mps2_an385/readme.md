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
****
## Ethernet driver - SMSC LAN911x

Micrium supports the SMSC (now Microship) LAN911x Ethernet controller, partially
implemented in QEMU.

Known limitations (version 7.0.0 of QEMU with current Micrium stack):
- `TSFL` interrupt flag is not handled in QEMU, so a patch is needed for it (TODO)
  - (**REQUIRED**) A temporary workaround is to always check FIFO status registers.
  Check function `NetDev_ISR_Handler` of `uC-TCP-IP/Dev/Ether/LAN911x/net_dev_lan911x.c`

Also, additionnal points should be noted for micrium driver `uC-TCP-IP/Dev/Ether/LAN911x/net_dev_lan911x.c` :

- (**REQUIRED**) Moreover Zephyr RTOS driver sets `INT_CFG_IRQ_POL` bit of `LAN91_INT_CFG`. 
- (**OPTIONNAL**) Zephyr RTOS driver uses 220us of interrupt deassertion interval
```cpp
    reg_val  =  LAN91_INT_CFG;
    reg_val |= (22 << 24)       |
                INT_CFG_IRQ_EN  |
                INT_CFG_IRQ_POL |
                INT_CFG_IRQ_TYPE;
    LAN91_INT_CFG = reg_val;
```

- TODO: Some UDP packets still fail to be send (because of pending operation).
  - Check line 7312 of uC-TCP-IP/IF/net_if.c : `NetIF_DevTxRdyWait(p_if, p_err);`