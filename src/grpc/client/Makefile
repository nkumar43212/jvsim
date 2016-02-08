CXX = g++ -w
CPPFLAGS += -pthread -I../lib/h/ -I/usr/local/include/
CXXFLAGS += -std=c++11
LDFLAGS  += -L/usr/local/lib -L../lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl -lrpc_service

ROOT_DIR    = ../../..
PLATFORM    = mac
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM)


all: client install

client: main.o AgentClient.o AgentClientParser.o
	$(CXX) $^ $(LDFLAGS) -o $@

install: client
	cp client $(INSTALL_DIR)/agent_client

clean:
	rm -f *.o agent_client
