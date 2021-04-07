#!/bin/bash
set -ex
docker build --tag udp_python_server-image -f Dockerfile ../
docker image prune -f
