#!/bin/bash

# Stop and delete the container
docker stop ubuntu-ssh-server
docker rm ubuntu-ssh-server

# Delete the image
docker rmi ubuntu-ssh:latest
