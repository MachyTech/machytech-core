FROM debian AS builder
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libx11-dev \
    xorg-dev \
    libglu1-mesa-dev \
    libcurl4-openssl-dev \
    libjson-c-dev
