# Use ubuntu image
FROM ubuntu:latest

# Install required packages
RUN apt-get update
RUN apt-get upgrade
RUN apt-get install -y openssh-server vim python3 gcc g++ make

# Create ssh server folder
RUN mkdir /var/run/sshd

# Set a password for the root user 
RUN echo 'root:root' | chpasswd

# Modify sshd_config
RUN sed -i 's/#Port.*/Port 22/' /etc/ssh/sshd_config
RUN sed -i 's/#PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed -i 's/#PasswordAuthentication.*/PasswordAuthentication yes/' /etc/ssh/sshd_config

# Expose SSH port
EXPOSE 22

# Start the SSH server
CMD ["/usr/sbin/sshd", "-D"]
