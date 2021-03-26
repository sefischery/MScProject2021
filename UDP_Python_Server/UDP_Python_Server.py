#!/usr/bin/python3
import socket
import datetime
import requests

ip = "40.85.115.125"
local_ip = "0.0.0.0"  # binds to all local interfaces(not just localhost)
local_UDP_port = 9889  # set to whatever port you use for UDP
UDP_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
UDP_socket.bind((local_ip, local_UDP_port))

print(f"Server is now listening on {local_ip}:{local_UDP_port}")
while True:
    data, address = UDP_socket.recvfrom(4096)
    if data:
        recv_date = str(datetime.datetime.now())
        data_str = data.decode('UTF-8')
        print(f"Server received: {data.decode('UTF-8')} at {recv_date}")
        data = {
            "date": recv_date,
            "payload-size": len(data_str),
            "content": data_str,
            "type": "plaintext",
            "technology": "NB-IoT"
        }
        print(data)
        request_to_send = requests.post(url=f"http://{ip}/message", json=data)
        print(f"I SENT {request_to_send}")
