ROOT_DIR    = ../../..
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM)
SRC_DIR     = $(ROOT_DIR)/src/grpc/server
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
LDFLAGS  += /usr/local/lib/libmosquittopp.$(DSO_EXT) -L/usr/local/lib -L../lib -lpthread -ldl -lrpc_service -lgrpc++_unsecure -lgrpc -lgpr -loc -lprotos -lprotobuf -ljsoncpp

FILES    = $(wildcard $(SRC_DIR)/*.cpp)
OBJS     = $(subst .cpp,.o,$(FILES))
OBJS_LIB = AgentConsolidator.o AgentConsolidatorSystem.o AgentSystem.o AgentSystemFile.o AgentSystemNull.o AgentSystemProc.o AgentSystemFactory.o AgentUtils.o

LIBS     = ../lib/liboc.a ../lib/librpc_service.a ../lib/libprotos.a

all: server install

server: $(OBJS) $(LIBS)
	$(CXX) $^ $(LDFLAGS) -o $@

install: server
	cp server $(INSTALL_DIR)/agent_server
	ar rvs libserver.a $(OBJS_LIB)
	mv libserver.a ../lib/

clean:
	rm -f *.o server  ../lib/libserver.a
