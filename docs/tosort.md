# TODO


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

## Micrium OS 3 demo for QEMU (Cortex M3)

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

## Clocks

Search for `system clock` in the datasheet.

---

## Notes
- Add MPS2 AN386 board for emulated Cortex-M4 test coverage: https://github.com/zephyrproject-rtos/zephyr/issues/45319

## Zephyr SLIP

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

## Zephyr mps2_an385 smsc911x driver

Zephyr run QEMU command for `echo_server` project built with `overlay-smsc911x.conf`
```
COMMAND = cd /home/lucas/zephyrproject/echo_server/build && /home/lucas/zephyr-sdk-0.14.2/sysroots/x86_64-pokysdk-linux/usr/bin/qemu-system-arm -cpu cortex-m3 -machine mps2-an385 -nographic -vga none -nic tap,model=lan9118,script=no,downscript=no,ifname=zeth -pidfile qemu.pid -chardev stdio,id=con,mux=on -serial chardev:con -mon chardev=con,mode=readline -s -S -kernel /home/lucas/zephyrproject/echo_server/build/zephyr/zephyr.elf
```

Monitor with wireshark (from windows host):
```
ssh lucas@fedora sudo tcpdump -U -s0 'not port 22' -i zeth -w - | "C:\Program Files\Wireshark\Wireshark.exe" -k -i -
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
  - 