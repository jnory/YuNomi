FROM alpine:3.4

RUN apk update && \
    apk --no-cache add cmake gcc g++ make wget && \
    wget http://www.hdfgroup.org/ftp/HDF5/current/src/hdf5-1.8.17.tar.gz && \
    tar zxf hdf5-1.8.17.tar.gz && \
    cd hdf5-1.8.17 && \
    /hdf5-1.8.17/configure --enable-cxx --prefix=/opt/hdf5 && \
    make install && \
    cd / && \
    rm hdf5-1.8.17.tar.gz && \
    rm -r hdf5-1.8.17

ENV PATH $PATH:/opt/hdf5