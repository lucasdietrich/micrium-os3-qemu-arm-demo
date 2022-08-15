#
# Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
#
# SPDX-License-Identifier: Apache-2.0
#

def decode(h: int):
    for bit in range(32):
        if h & (1 << bit):
            print(f"bit {bit} set")

while True:
    try:
        h = int(input("Enter a hex number: "), 16)
        decode(h)
    except KeyboardInterrupt:
        break;
    finally:
        print()