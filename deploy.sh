#!/bin/bash
set -ex
docker-compose up -d nb-iot_webapp-service udp_python_server
