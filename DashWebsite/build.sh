#!/bin/bash
set -e
docker build --tag nb-iot_webapp-image .
docker image prune -f
