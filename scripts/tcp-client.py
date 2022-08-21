#
# Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
#
# SPDX-License-Identifier: Apache-2.0
#

import socket
import random
import struct

IP = "192.0.2.3"
PORT = 10001

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)

sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

sock.connect((IP, PORT))

data = b'a'*1024*24
tosend = len(data)

while tosend > 0:
    res = sock.send(data)
    if res > 0:
        tosend -= res
        print("Sent {} bytes".format(res))
    elif res == 0:
        print("Disconnected")
        break
    else:
        print(f"Error {res}")
        break

sock.close()
