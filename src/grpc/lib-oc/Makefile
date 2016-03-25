CXX      = g++ -w
CPPFLAGS += -I/usr/local/include -pthread -I../lib/h -I../server
CXXFLAGS += -std=c++11

ROOT_DIR    = ../../..
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM) 
SRC_DIR     = $(ROOT_DIR)/src/grpc/lib-oc
FILES       = $(wildcard $(SRC_DIR)/*.cpp)
OC_OBJS        = $(subst .cpp,.o,$(FILES))

all: liboc

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

liboc: $(OC_OBJS) oc.o
	ar rvs liboc.a $^
	mv liboc.a ../lib/

clean:
	rm -f *.o *.a ../lib/liboc.a
