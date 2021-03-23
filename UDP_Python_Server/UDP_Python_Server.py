#!/usr/bin/python3

import socket
import datetime
local_ip = "0.0.0.0" # binds to all local interfaces(not just localhost)
local_UDP_port = 9889 # set to whatever port you use for UDP
UDP_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
UDP_socket.bind((local_ip, local_UDP_port))

print(f"Server is now listening on {local_ip}:{local_UDP_port}")
while True:
    data, address = UDP_socket.recvfrom(4096)
    print(f"Server received: {data.decode('UTF-8')} at {str(datetime.datetime.now())}")