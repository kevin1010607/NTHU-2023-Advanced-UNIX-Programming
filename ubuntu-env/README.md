# Run Ubuntu environment using Docker

## Setup
```bash
# Download Ubuntu base image
docker pull ubuntu:latest

# Build the image with ssh server
docker build -t ubuntu-ssh:latest .

# Run the container
docker run -d -p 2222:22 \
    --name ubuntu-ssh-server \
    ubuntu-ssh:latest
```

## How to use

### Connect to the server using ssh
- User: root
- Password: root
```bash
# Connect to the server
ssh -p 2222 \
    -o UserKnownHostsFile=/dev/null \
    -o StrictHostKeyChecking=no \
    root@127.0.0.1
# Add ssh key to the server
ssh-copy-id -p 2222 \
    -o UserKnownHostsFile=/dev/null \
    -o StrictHostKeyChecking=no \
    root@127.0.0.1
```

### Send file
```bash
# Send file from local to the server
scp -r -P 2222 \
    -o UserKnownHostsFile=/dev/null \
    -o StrictHostKeyChecking=no \
    <path> \
    root@127.0.0.1:<path>
# Send file from the server to local
scp -r -P 2222 \
    -o UserKnownHostsFile=/dev/null \
    -o StrictHostKeyChecking=no \
    root@127.0.0.1:<path> \
    <path>
```

## Clean
```bash
# Stop and delete the container
docker stop ubuntu-ssh-server
docker rm ubuntu-ssh-server

# Delete the image
docker rmi ubuntu-ssh:latest
```
