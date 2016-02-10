CXX = g++ -w
CPPFLAGS += -pthread -I../lib/h/ -I/usr/local/include/
CXXFLAGS += -std=c++11
LDFLAGS  += -L/usr/local/lib -L../lib -lpthread -ldl -lrpc_service  -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf

ROOT_DIR    = ../../..
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
        PLATFORM    = linux
else
        PLATFORM    = mac
endif


all: client install

client: main.o AgentClient.o AgentClientParser.o
	$(CXX) $^ $(LDFLAGS) -o $@

install: client
	cp client $(INSTALL_DIR)/agent_client
	ar rvs libclient.a AgentClient.o
	mv libclient.a ../lib/.

clean:
	rm -f *.o client libclient.a
