#!/bin/bash

# Download Ubuntu base image
docker pull ubuntu:latest

# Build the image with ssh server
docker build -t ubuntu-ssh:latest .

# Run the container
docker run -d -p 2222:22 \
    --name ubuntu-ssh-server \
    ubuntu-ssh:latest
