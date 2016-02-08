CXX      = g++ -w
CPPFLAGS += -I/usr/local/include -pthread
CXXFLAGS += -std=c++11
LDFLAGS  += -L/usr/local/lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl
PROTOC   = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = ../../../protos

OBJ    = agent.pb.o agent.grpc.pb.o jvision_top.pb.o

all: proto librpc_service

proto:
	$(PROTOC) --proto_path=$(PROTOS_PATH) --cpp_out=. $(PROTOS_PATH)/agent.proto
	$(PROTOC) --proto_path=$(PROTOS_PATH) --cpp_out=. $(PROTOS_PATH)/jvision_top.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $(PROTOS_PATH)/agent.proto

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

librpc_service: $(OBJ) 
	ar rvs librpc_service.a $^
	mv librpc_service.a ../lib/.  
	mv *.h ../lib/h/

clean:
	rm -f *.o *.pb.cc *.pb.h *.a
