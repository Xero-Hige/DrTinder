# Set the base image to Ubuntu
FROM ubuntu:14.04

# Install required packages
RUN apt-get update && apt-get install -y \
  autoconf \
  cmake \
  curl \
  git \
  g++ \
  libbz2-dev \
  libcurl4-openssl-dev \
  libgflags-dev \
  libsnappy-dev \
  libssl-dev \
  libtool \
  make \
  openssl \
  python \
  zlib1g-dev


# Clone repository and checkout AppServer
RUN git clone https://github.com/Xero-Hige/DrTinder/
RUN cd DrTinder ; git checkout origin/AppServer

# Download git submodules
RUN cd DrTinder ; git submodule init && git submodule update

# Install rocksdb
RUN cd DrTinder/source/libs/rocksdb ; make install

# Install jsoncpp
RUN cd DrTinder/source/libs/jsoncpp ; python amalgamate.py

# Install googletest
RUN cd DrTinder/source/libs/googletest ; cmake . ; make install

# Install cpprest
RUN cd DrTinder/source/libs/restclient-cpp ; ./autogen.sh ; ./configure ; make install

# Install swiften
RUN cd DrTinder/source/libs/swift ; ./scons SWIFTEN_INSTALLDIR=/usr/local /usr/local

# Install boost
RUN apt-get update && apt-get install -y libboost-all-dev

# Add exe permissions for google style check
RUN cd DrTinder ; chmod +x execute.sh

# Run cmake on AppServer
RUN cd DrTinder/source/Servidor ; cmake .

# Compile AppServer
RUN cd DrTinder/source/Servidor ; make

# Run Tests and upload codecov
CMD DrTinder/source/Servidor/Test
