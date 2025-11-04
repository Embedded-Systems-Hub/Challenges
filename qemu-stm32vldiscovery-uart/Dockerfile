FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    qemu-system-arm \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    make \
    telnet \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /work
