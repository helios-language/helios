FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    binutils \
    git \
    scons \
    build-essential \
    valgrind \
    libcmocka0 \
    libcmocka-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*