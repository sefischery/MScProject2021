#!/bin/bash
set -e
docker build --tag nb-iot_webapp-image -f Dockerfile ../
docker image prune -f
