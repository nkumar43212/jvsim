FROM ubuntu:14.04
MAINTAINER Surya Nimmagadda <nscsekhar@juniper.net> && Abbas Sakarwala <abbas@juniper.net>

# Install base packages
RUN cd /home  \
&& sudo apt-get update \
&& sudo apt-get install -y git wget curl zip unzip pkg-config \
&& sudo apt-get install -y build-essential autoconf libtool \
&& apt-get install -y python \
&& apt-get install -y python-setuptools python-pip

# install relevant gRPC tools
RUN cd /home \
&& git clone https://github.com/grpc/grpc grpc \
&& cd /home/grpc  \
&& git submodule update --init \
&& make \
&& sudo make install \
&& cd /home/grpc/third_party/protobuf/ \
&&  ./autogen.sh \
&&  ./configure \
&& make \
&& sudo make install

# install python bindings
RUN sudo python -m pip install --upgrade pip \
&&  sudo python -m pip install grpcio \
&&  sudo python -m pip install grpcio-tools 

# Install mosquitto
RUN cd /home \
&& sudo apt-get install -y pkg-config cmake openssl libc-ares-dev libssl-dev python-mosquitto \
&& wget http://mosquitto.org/files/source/mosquitto-1.3.5.tar.gz \
&& tar xzf mosquitto-1.3.5.tar.gz \
&& cd mosquitto-1.3.5 \
&& cmake . \
&& sudo make install \
&& pip install paho-mqtt \
&& useradd mosquitto

# Install jsoncpp
RUN cd /home \
&& git clone https://github.com/open-source-parsers/jsoncpp.git \
&& cd jsoncpp \
&& mkdir -p build/debug \
&& cd build/debug \
&& cmake -DCMAKE_BUILD_TYPE=debug -DJSONCPP_LIB_BUILD_STATIC=ON-DJSONCPP_LIB_BUILD_SHARED=OFF -G "Unix Makefiles" ../.. \
&& make \
&& make install

# Install and setup ssh service
RUN apt-get -y install ssh \
&& mkdir /var/run/sshd \
&& echo 'root:grpc' | chpasswd \
&& sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
&& sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile



ENV ROOTPATH=/home/jvsim
ENV LD_LIBRARY_PATH=/usr/local/lib

RUN  mkdir /home/jvsim
COPY bin /home/jvsim/bin
COPY config /home/jvsim/config
RUN  mkdir /home/jvsim/logs
COPY occam /home/jvsim/occam
COPY protos /home/jvsim/protos
COPY src /home/jvsim/src
COPY Makefile /home/jvsim/Makefile

WORKDIR /home/jvsim

RUN sudo ldconfig \
&& export LD_LIBRARY_PATH=/usr/local/lib \
&& export ROOTPATH=/home/jvsim \
&& make all

# Expose the default telemetry GRPC agent server.
# Change to desired value if needed.
EXPOSE 50051
EXPOSE 22

CMD service ssh restart && bash
