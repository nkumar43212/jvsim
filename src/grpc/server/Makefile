ROOT_DIR    = ../../..
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
        PLATFORM    = linux
	DSO_EXT     = so
else
        PLATFORM    = mac
	DSO_EXT     = dylib
endif

CXX      = g++ -w
CPPFLAGS += -I/usr/local/include -pthread -I../lib/h
CXXFLAGS += -std=c++11
LDFLAGS  += /usr/local/lib/libmosquittopp.$(DSO_EXT) -L/usr/local/lib -L../lib -lpthread -ldl -lrpc_service -lgrpc++_unsecure -lgrpc -lgpr -loc -lprotos -lprotobuf


all: server install

server: main.o AgentServer.o AgentMessageBus.o AgentSubscription.o OpenConfig.o AgentConsolidator.o AgentConsolidatorSystem.o AgentSystem.o
	$(CXX) $^ $(LDFLAGS) -o $@

install: server
	cp server $(INSTALL_DIR)/agent_server

clean:
	rm -f *.o server 
