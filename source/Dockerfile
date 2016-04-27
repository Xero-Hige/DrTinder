# Set the base image to Ubuntu
FROM ubuntu

# Update the repository sources list
RUN apt-get update

# Install rocksdb packages
RUN apt-get install libgflags-dev
RUN apt-get install libsnappy-dev
RUN apt-get install libbz2-dev
RUN apt-get install zlib1g-dev

# Expose port for application
EXPOSE 8080


