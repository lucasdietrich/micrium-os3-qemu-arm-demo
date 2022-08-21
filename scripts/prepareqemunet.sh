#!/bin/bash

# Do not run this script as root

sudo iptables -t nat -A POSTROUTING -j MASQUERADE -s 192.0.2.3
sudo sysctl -w net.ipv4.ip_forward=1

sudo ./scripts/net-setup.sh