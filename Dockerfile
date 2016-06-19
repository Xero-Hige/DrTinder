# Set the base image to Ubuntu
FROM ubuntu:14.04

# Update the repository sources list
RUN apt-get update

# Install required packages
RUN apt-get install -y libgflags-dev
RUN apt-get install -y libsnappy-dev
RUN apt-get install -y libbz2-dev
RUN apt-get install -y zlib1g-dev
RUN apt-get install -y libcurl4-openssl-dev
RUN apt-get install -y autoconf
RUN apt-get install -y libtool

# Install git
RUN apt-get install -y git

# Install make
RUN apt-get install -y make

# Install g++
RUN apt-get install -y g++

# Install cmake
RUN apt-get install -y cmake

# Install python
RUN apt-get install -y python

# Install openssl library
RUN apt-get install -y openssl
RUN apt-get install -y libssl-dev



# Clone repository and checkout AppServer
RUN git clone https://github.com/Xero-Hige/DrTinder/
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
RUN cd DrTinder/source/libs/googletest ; cmake . ; sudo make install

# Install cpprest
RUN cd DrTinder/source/libs/restclient-cpp ; ./autogen.sh ; ./configure ; sudo make install




# Run cmake on AppServer
RUN cd DrTinder/source/Servidor ; cmake .

# Compile AppServer
RUN cd DrTinder/source/Servidor ; make

# Run AppServer
CMD DrTinder/source/Servidor/unit_test ; cd DrTinder ; curl -s https://codecov.io/bash ; ./execute.sh
