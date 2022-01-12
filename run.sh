#!/usr/bin/env /bin/bash

set -e

VERSION=$(date +"%M%d%y")
PROJECT=jac18281828/gzkfast

TAG=${PROJECT}:${VERSION}

docker build . -t ${TAG} && \
   docker run --rm -i ${TAG}

