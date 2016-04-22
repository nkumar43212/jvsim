ROOT_DIR    = ../../..
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM)
SRC_DIR     = $(ROOT_DIR)/src/grpc/mgd_server
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	PLATFORM    = linux
	DSO_EXT     = so
else
	PLATFORM    = mac
	DSO_EXT     = dylib
endif

CXX      = g++ -w
CPPFLAGS += -I/usr/local/include -pthread -I../lib/h -I../server
CXXFLAGS += -std=c++11
LDFLAGS  += -L/usr/local/lib -L../lib -lpthread -ldl -lrpc_service -lgrpc++_unsecure -lgrpc -lgpr -loc -lprotos -lprotobuf -ljsoncpp

FILES    = $(wildcard $(SRC_DIR)/*.cpp)
OBJS     = $(subst .cpp,.o,$(FILES))

LIBS     = ../lib/librpc_service.a ../lib/libprotos.a

all: mgd_server install

mgd_server: $(OBJS) $(LIBS)
	$(CXX) $^ $(LDFLAGS) -o $@

install: mgd_server
	cp mgd_server $(INSTALL_DIR)/mgd_server

clean:
	rm -f *.o mgd_server
