FROM debian:stable-slim
RUN export DEBIAN_FRONTEND=noninteractive && \
    apt update && \
        apt install -y -q --no-install-recommends \
        build-essential cmake libgsl-dev
RUN apt clean
RUN rm -rf /var/lib/apt/lists/*

WORKDIR /gzkfast

COPY CMakeLists.txt .
COPY src src/
COPY include include/
COPY utils utils/

ENV MAKEFLAGS=-j8

#build gemmatch
RUN cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_VERBOSE_MAKEFILE=on "-GUnix Makefiles"

RUN (cd build && make -j all)

RUN ls -l build


