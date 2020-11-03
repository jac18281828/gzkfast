FROM debian:10

RUN apt update && \
    apt -y install build-essential g++ cmake libgsl-dev

WORKDIR /gzkfast

COPY CMakeLists.txt .
COPY src src/
COPY include include/
COPY utils utils/

#build gemmatch
RUN cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_VERBOSE_MAKEFILE=on "-GUnix Makefiles"

RUN (cd build && make -j all)

RUN ls -l build


