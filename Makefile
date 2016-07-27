ROOT_DIR = .
SERVER_DIR = $(ROOT_DIR)/src/grpc/server
CLIENT_DIR = $(ROOT_DIR)/src/grpc/client
MGD_DIR  = $(ROOT_DIR)/src/grpc/mgd_server
TEST_DIR = $(ROOT_DIR)/src/grpc/test
UDP_DATA_SIMULATOR_DIR = $(ROOT_DIR)/src/grpc/udp_data_simulator
LOG_DIR = $(ROOT_DIR)/logs/
SRC_DIR = $(ROOT_DIR)/src/grpc
LIB_DIR = $(ROOT_DIR)/src/grpc/lib
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	PLATFORM    = linux
else
	PLATFORM    = mac
endif
BIN_DIR = $(ROOT_DIR)/bin/$(PLATFORM)


all: libs server client mgd_server test udp_data_simulator

server: libs
	cd $(SERVER_DIR); make

client: libs
	cd $(CLIENT_DIR); make

mgd_server: libs
	cd $(MGD_DIR); make

test: client server libs
	cd $(TEST_DIR); make

udp_data_simulator: libs
	cd $(UDP_DATA_SIMULATOR_DIR); make

libs:
	cd $(SRC_DIR)/lib-protos; make
	cd $(SRC_DIR)/lib-rpc-service; make
	cd $(SRC_DIR)/lib-oc; make
	cd $(SRC_DIR)/lib-inih; make

clean-logs:
	cd $(LOG_DIR); rm *; touch test

clean:
	cd $(SERVER_DIR); make clean
	cd $(CLIENT_DIR); make clean
	cd $(MGD_DIR); make clean
	cd $(TEST_DIR); make clean
	cd $(UDP_DATA_SIMULATOR_DIR); make clean
	cd $(LIB_DIR); rm *.a
	cd $(BIN_DIR); rm -f agent*; rm -f mgd*; rm -f udp_data_*

