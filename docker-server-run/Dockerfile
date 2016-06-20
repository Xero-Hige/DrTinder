#!/bin/sh

# Set the base image to Ubuntu
FROM resin/rpi-raspbian

# Update the repository sources list
RUN apt-get update

# Install required packages
RUN apt-get install -y apt-utils
RUN apt-get install -y git ca-certificates curl libgflags-dev libsnappy-dev libbz2-dev zlib1g-dev libcurl4-openssl-dev autoconf libtool make g++ cmake python openssl libssl-dev ssh openssh-client

# Clone repository and checkout AppServer
RUN git clone https://github.com/Xero-Hige/DrTinder.git
RUN pwd
RUN ls -la
RUN cd DrTinder ; git checkout origin/AppServer

# Download git submodules
RUN cd DrTinder ; git submodule init
RUN cd DrTinder ; git submodule update

# Generate librocksdb.a
RUN cd DrTinder/source/libs/rocksdb ; sudo make install

# Run amalgamate.py from jsoncpp library
RUN cd DrTinder/source/libs/jsoncpp ; python amalgamate.py

# Install googletest
#RUN cd DrTinder/source/libs/googletest ; cmake . ; sudo make install

# Install cpprest
RUN cd DrTinder/source/libs/restclient-cpp ; ./autogen.sh ; ./configure ; sudo make install




# Run cmake on AppServer
RUN cd DrTinder/source/Servidor ; cmake .

# Compile AppServer
RUN cd DrTinder/source/Servidor ; make

# Run AppServer
CMD DrTinder/source/Servidor/server
