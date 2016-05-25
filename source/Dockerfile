# Set the base image to Ubuntu
FROM ubuntu:14.04

# Update the repository sources list
RUN apt-get update

# Install rocksdb packages
RUN apt-get install -y libgflags-dev
RUN apt-get install -y libsnappy-dev
RUN apt-get install -y libbz2-dev
RUN apt-get install -y zlib1g-dev

# Install git
RUN apt-get install -y git

# Clone repository and checkout AppServer
RUN git clone https://github.com/Xero-Hige/DrTinder/
RUN pwd
RUN ls -la
RUN cd DrTinder ; git checkout origin/AppServer

# Download rocksdb module
RUN cd DrTinder ; git submodule init
RUN cd DrTinder ; git submodule update

# Install make
RUN apt-get install -y make

# Install g++
RUN apt-get install -y g++

# Generate librocksdb.a
RUN cd DrTinder/source/libs/rocksdb ; sudo make install

# Install cmake
RUN apt-get install -y cmake

# Install python
RUN apt-get install -y python

# Run amalgamate.py from jsoncpp library
RUN cd DrTinder/source/libs/jsoncpp ; python amalgamate.py

# Run cmake
RUN cd DrTinder/source/Servidor ; cmake .

# Install openssl library
RUN apt-get install -y openssl
RUN apt-get install -y libssl-dev

# Compile AppServer
RUN cd DrTinder/source/Servidor ; make

# Run AppServer
CMD DrTinder/source/Servidor/server
