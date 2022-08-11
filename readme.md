**TODO currently on this error: Line 657 of `uC-TCP-IP/IF/net_if_ether.c`**

# QEMU

- Documentation: https://qemu.readthedocs.io/en/latest/system/invocation.html
- How Zephyr handles QEMU : https://github.com/zephyrproject-rtos/zephyr/blob/main/cmake/emu/qemu.cmake
- Zephyr networking in QEMU:
  - https://github.com/zephyrproject-rtos/net-tools/blob/master/README.md
  - https://github.com/zephyrproject-rtos/net-tools/blob/master/README%20NAT.md

# Micrium

- [weston-embedded / uC-OS3](https://github.com/weston-embedded/uC-OS3)
- [µC documentation Spaces](https://micrium.atlassian.net/wiki/spaces)
  - [µC/OS-III Documentation](https://micrium.atlassian.net/wiki/spaces/osiiidoc/overview?homepageId=132386)
- [CMSIS-Core (Cortex-M)](https://arm-software.github.io/CMSIS_5/Core/html/index.html)
- [TCP/IP stack doc](https://micrium.atlassian.net/wiki/spaces/TCPIPDOC/pages/565482/Wired+Ethernet+Interface+Setup#WiredEthernetInterfaceSetup-Figure-%C2%B5C/TCP-IPSourceCode)
  - uC-TCP-IP/Examples/Init/init_ether.c

# Micrium OS 3 demo for QEMU (Cortex M4)

- [QEMU - STMicroelectronics STM32 boards](https://emu.readthedocs.io/en/latest/system/arm/stm32.html)
  - https://github.com/qemu/qemu/blob/079b1252e9de384385c9da910262312ec2e574c8/include/hw/arm/stm32f405_soc.h
- Target `netduinoplus2`
- Microcontroller [STM32F405RGT6](https://www.st.com/en/microcontrollers-microprocessors/stm32f405rg.html#documentation) 
  - Arm Cortex-M4 (Armv7E-M)
  - 192 KB of SRAM

- QEMU Machine implementation:
  - https://github.com/qemu/qemu/blob/079b1252e9de384385c9da910262312ec2e574c8/hw/arm/netduinoplus2.c
  - https://github.com/qemu/qemu/blob/aab8cfd4c3614a049b60333a3747aedffbd04150/hw/arm/stm32f405_soc.c

Page 81 of RM0090 Rev 19:
> After reset, the CPU clock frequency is 16 MHz and 0 wait state (WS) is configured in the FLASH_ACR register.

## Todo
- Link in Core Couple Memory RAM (CCM RAM) : `__attribute__((section(".ccmram")))` does not work yet.
- NVIC Priority Grouping might be initialized twice (not sure somewhere in ucos3 and HAL_Init())
- Optimized value of `LIB_MEM_CFG_HEAP_SIZE`


## Notes
- `qemu-system-arm -accel help` > `tcg`
- `qemu-system-arm -device help`
- `qemu-system-arm -device e1000,help`
- `qemu-system-arm -machine netduinoplus2,help`
- `qemu-system-arm -machine netduinoplus2 -chardev help`
  - stdio
  - vc
  - serial
  - pipe
  - pty
  - testdev
  - null
  - ringbuf
  - wctablet
  - msmouse
  - socket
  - udp
  - file
  - mux
  - parallel
  - memory
- No clock tree support in QEMU:
  - https://stackoverflow.com/questions/56853507/timer-supply-to-cpu-in-qemu
- Tuning heap alloc size: `LIB_MEM_CFG_HEAP_SIZE`
  - Move `CPU_INT08U   Mem_Heap[LIB_MEM_CFG_HEAP_SIZE];` to CCM


---

# Micrium OS 3 demo for QEMU (Cortex M3)

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

---

# Notes
- Add MPS2 AN386 board for emulated Cortex-M4 test coverage: https://github.com/zephyrproject-rtos/zephyr/issues/45319

Zephyr run QEMU (x86) command for `hello_world` sample:
```
cd /home/lucas/zephyrproject/hello_world/build && /home/lucas/zephyr-sdk-0.14.2/sysroots/x86_64-pokysdk-linux/usr/bin/qemu-system-i386 -m 4 -cpu qemu32,+nx,+pae -device isa-debug-exit,iobase=0xf4,iosize=0x04 -no-reboot -nographic -no-acpi -net none -pidfile qemu.pid -chardev stdio,id=con,mux=on -serial chardev:con -mon chardev=con,mode=readline -icount shift=5,align=off,sleep=off -rtc clock=vm -kernel /home/lucas/zephyrproject/hello_world/build/zephyr/zephyr.elf
```

Zephyr run QEMU (arm cortex m3) command for `hello_world` sample:
```
COMMAND = cd /home/lucas/zephyrproject/hello_world/build && /home/lucas/zephyr-sdk-0.14.2/sysroots/x86_64-pokysdk-linux/usr/bin/qemu-system-arm -cpu cortex-m3 -machine lm3s6965evb -nographic -vga none -net none -pidfile qemu.pid -chardev stdio,id=con,mux=on -serial chardev:con -mon chardev=con,mode=readline -icount shift=6,align=off,sleep=off -rtc clock=vm -kernel /home/lucas/zephyrproject/hello_world/build/zephyr/zephyr.elf
```

Zephyr run QEMU command for `qemu-zephyr-dev` project:
```
COMMAND = cd /home/lucas/zephyrproject/zephyr-qemu-dev/build && /home/lucas/zephyr-sdk-0.14.2/sysroots/x86_64-pokysdk-linux/usr/bin/qemu-system-arm -cpu cortex-m3 -machine lm3s6965evb -nographic -vga none -net none -pidfile qemu.pid -chardev stdio,id=con,mux=on -serial chardev:con -mon chardev=con,mode=readline -serial unix:/tmp/slip.sock -s -S -kernel /home/lucas/zephyrproject/zephyr-qemu-dev/build/zephyr/zephyr.elf
```

> The current QEMU console configuration directly connects the console serial port to the backend using '-serial' option.

> This is less than ideal because only single console instance can be connected to a backend and aggregation of multiple console outputs is not possible (e.g. multiple console serial ports and semihosting console to single console backend).

> In order to solve this problem, single multiplexed chardev console backend is declared and all consoles are connected to it.


Chardev:
```
Character device options:
-chardev help
-chardev null,id=id[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev socket,id=id[,host=host],port=port[,to=to][,ipv4=on|off][,ipv6=on|off][,nodelay=on|off]
         [,server=on|off][,wait=on|off][,telnet=on|off][,websocket=on|off][,reconnect=seconds][,mux=on|off]
         [,logfile=PATH][,logappend=on|off][,tls-creds=ID][,tls-authz=ID] (tcp)
-chardev socket,id=id,path=path[,server=on|off][,wait=on|off][,telnet=on|off][,websocket=on|off][,reconnect=seconds]
         [,mux=on|off][,logfile=PATH][,logappend=on|off][,abstract=on|off][,tight=on|off] (unix)
-chardev udp,id=id[,host=host],port=port[,localaddr=localaddr]
         [,localport=localport][,ipv4=on|off][,ipv6=on|off][,mux=on|off]
         [,logfile=PATH][,logappend=on|off]
-chardev msmouse,id=id[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev vc,id=id[[,width=width][,height=height]][[,cols=cols][,rows=rows]]
         [,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev ringbuf,id=id[,size=size][,logfile=PATH][,logappend=on|off]
-chardev file,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev pipe,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev pty,id=id[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev stdio,id=id[,mux=on|off][,signal=on|off][,logfile=PATH][,logappend=on|off]
-chardev serial,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev tty,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev parallel,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev parport,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
```

- Discussion on QEMU time sync with the host
  - https://github.com/zephyrproject-rtos/zephyr/issues/14173
  - https://qemu.readthedocs.io/en/latest/devel/tcg-icount.html
  - related to flags `-icount shift=0,align=off,sleep=off -rtc clock=vm`

---
## Versions

- QEMU emulator version 7.0.0 (v7.0.0)

## Ressources
- Interesting repos:
  - https://github.com/japaric-archived/lm3s6965evb
  - https://github.com/pokitoz/qemu-lm3s6965evb
    - Particularly:
      - Semihost console: https://github.com/pokitoz/qemu-lm3s6965evb/blob/master/start.c
      - MPU: https://github.com/pokitoz/qemu-lm3s6965evb/blob/master/main.c
  - https://github.com/Introduction-To-System-On-Chip/QEMU_lm3s6965evb
  - https://github.com/moslevin/mark3-bsp-qemu_lm3s6965evb