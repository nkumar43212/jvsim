CXX = g++ -w
CPPFLAGS += -pthread -I../lib/h/ -I/usr/local/include/ -I../server/
CXXFLAGS += -std=c++11
LDFLAGS  += -L/usr/local/lib -L../lib -lpthread -ldl -lrpc_service  -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lprotos

ROOT_DIR    = ../../..
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM)
SRC_DIR     = $(ROOT_DIR)/src/grpc/client
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
        PLATFORM    = linux
else
        PLATFORM    = mac
endif

FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJS  = $(subst .cpp,.o,$(FILES))

all: client install

client: $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

install: client
	cp client $(INSTALL_DIR)/agent_client
	ar rvs libclient.a AgentClient.o AgentClientLag.o
	mv libclient.a ../lib/.

clean:
	rm -f *.o client libclient.a
