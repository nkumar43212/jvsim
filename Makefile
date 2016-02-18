ROOT_DIR = .
SERVER_DIR = $(ROOT_DIR)/src/grpc/server
CLIENT_DIR = $(ROOT_DIR)/src/grpc/client
TEST_DIR = $(ROOT_DIR)/src/grpc/test
BIN_DIR = $(ROOT_DIR)/bin/mac/
LOG_DIR = $(ROOT_DIR)/logs/
SRC_DIR = $(ROOT_DIR)/src/grpc
LIB_DIR = $(ROOT_DIR)/src/grpc/lib

all: libs server client test

server:
	cd $(SERVER_DIR); make

client:
	cd $(CLIENT_DIR); make

test:
	cd $(TEST_DIR); make

libs:
	cd $(SRC_DIR)/lib-oc; make
	cd $(SRC_DIR)/lib-protos; make
	cd $(SRC_DIR)/lib-rpc-service; make

clean-logs:
	cd $(LOG_DIR); rm *; touch test
clean:
	cd $(SERVER_DIR); make clean
	cd $(CLIENT_DIR); make clean
	cd $(TEST_DIR); make clean
	cd $(BIN_DIR); rm agent*
	cd $(LIB_DIR); rm *.a

	
