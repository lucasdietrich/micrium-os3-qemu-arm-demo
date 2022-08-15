#
# Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
#
# SPDX-License-Identifier: Apache-2.0
#

import socket
import random
import struct

PORT = 10001

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

sock.bind(('', 10001))

print(f"Waiting for incoming UDP packet on port {PORT} ...")

while True:
    buf, addr = sock.recvfrom(50)
    print(f"Received {len(buf)} bytes from {addr}")
    sock.sendto(buf, addr)
