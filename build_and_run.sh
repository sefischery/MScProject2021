#!/bin/bash
set -ex

pushd DashWebsite
./build.sh
popd

pushd UDP_Python_Server
./build.sh
popd

./deploy.sh