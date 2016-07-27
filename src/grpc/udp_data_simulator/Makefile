CXX = g++ -w
CPPFLAGS += -pthread -I../lib/h/ -I/usr/local/include/ -I../server/
CXXFLAGS += -std=c++11
LDFLAGS  += -L/usr/local/lib -L../lib -lpthread -lprotobuf -lprotos

ROOT_DIR    = ../../..
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM)
SRC_DIR     = $(ROOT_DIR)/src/grpc/udp_data_simulator
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	PLATFORM    = linux
else
	PLATFORM    = mac
endif

FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJS  = $(subst .cpp,.o,$(FILES))

all: udp_data_simulator install

udp_data_simulator: $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

install: udp_data_simulator 
	cp udp_data_simulator $(INSTALL_DIR)/udp_data_simulator

clean:
	rm -f *.o udp_data_simulator
