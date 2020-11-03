#!/usr/bin/env /bin/bash

set -e

TAG='gzkfast_build:1.0'

docker image rm -f ${TAG}

docker build --tag ${TAG} .

