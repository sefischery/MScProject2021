#!/bin/bash
set -ex

pushd ../WebApplication/Docker
./build.sh
popd

pushd ../UDP_Python_Server/Docker
./build.sh
popd

./deploy.sh