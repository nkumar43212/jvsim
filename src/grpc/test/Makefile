GTEST_DIR = ../../gtest
CXX = g++ -w
CPPFLAGS += -I/usr/local/include -pthread -I../lib/h -I../client -isystem $(GTEST_DIR)/include
CXXFLAGS += -std=c++11
LDFLAGS += -L/usr/local/lib -L../lib -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lpthread -ldl -lrpc_service
PROTOC = protoc

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

ROOT_DIR    = ../../..
PLATFORM    = mac
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM)

all: agent_test install

# Build the gtest library
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc
            
gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc
            
gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^
	cp gtest_main.a ../lib/gtest_main.a

# Build unit tests
agent_test.o : agent_test.cpp agent_test.hpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c agent_test.cpp

agent_test : ../client/AgentClient.o agent_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -lpthread $^ -o $@

install: agent_test
	cp agent_test $(INSTALL_DIR)/agent_test

clean:
	rm -f *.o *.pb.cc *.pb.h agent_test
