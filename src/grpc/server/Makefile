CXX      = g++ -w
CPPFLAGS += -I/usr/local/include -pthread -I../lib/h
CXXFLAGS += -std=c++11
LDFLAGS  += -L/usr/local/lib -L../lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl -lrpc_service /usr/local/lib/libmosquittopp.dylib -loc -lprotos

ROOT_DIR    = ../../..
PLATFORM    = mac
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM)

all: server install

server: main.o AgentServer.o AgentMessageBus.o AgentSubscription.o OpenConfig.o 
	$(CXX) $^ $(LDFLAGS) -o $@

install: server
	cp server $(INSTALL_DIR)/agent_server

clean:
	rm -f *.o server 
