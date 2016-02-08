CXX      = g++ -w
CPPFLAGS += -I/usr/local/include -pthread
CXXFLAGS += -std=c++11
LDFLAGS  += -L/usr/local/lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl
PROTOC   = protoc

PROTOS_PATH = ../../../protos

OBJ = port.pb.o firewall.pb.o logical_port.pb.o lsp_stats.pb.o

all: proto libprotos

proto:
	$(PROTOC) --proto_path=$(PROTOS_PATH) --cpp_out=. $(PROTOS_PATH)/*.proto

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

libprotos: $(OBJ)
	ar rvs libprotos.a $^
	mv libprotos.a ../lib/.  
	mv *.h ../lib/h/

clean:
	rm -f *.o *.pb.cc *.pb.h *.a
