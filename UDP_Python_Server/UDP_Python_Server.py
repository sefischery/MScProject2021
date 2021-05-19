#!/usr/bin/python3
import socket
import datetime
import requests
import base64

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
        try:
            data_str = data.decode('UTF-8')
            _type = "plaintext"
        except:
            data_str = base64.b64encode(data).decode('UTF-8')
            _type = "encryption"

        print(f"Server received: {data_str} at {recv_date}")

        data = {
            "date": recv_date,
            "payload-size": len(data_str),
            "content": data_str,
            "type": _type,
            "technology": "NB-IoT"
        }
        print(f"Data is the following: {data}")
        endpoint_path = f"http://{ip}/message"
        print(f"Sending HTTP POST request to {endpoint_path}")
        request_to_send = requests.post(url=f"{endpoint_path}", json=data)
        print(f"Server sent the following request: {request_to_send}")
