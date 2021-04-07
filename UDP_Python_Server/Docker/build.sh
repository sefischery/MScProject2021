#!/bin/bash
set -e
docker build --tag udp_python_server-image .
docker image prune -f
