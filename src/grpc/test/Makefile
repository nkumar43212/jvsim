GTEST_DIR = ../../gtest
CXX = g++ -w
CPPFLAGS += -I/usr/local/include -pthread -I../lib/h -I../client -I../server -isystem $(GTEST_DIR)/include
CXXFLAGS += -std=c++11
LDFLAGS += -L/usr/local/lib -L../lib -lpthread -ldl -lclient -lserver -lgrpc++_unsecure -lgrpc -lgpr -lprotobuf -lprotos -lrpc_service -ljsoncpp -linih
PROTOC = protoc

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

ROOT_DIR    = ../../..
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	PLATFORM    = linux
else
	PLATFORM    = mac
endif

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
agent_test : gtest_main.a agent_test_id_manager_ut.o agent_test_arg_parser_ut.o agent_test_path_validator_ut.o agent_test_global_config_ut.o agent_test_consolidator_ut.o agent_test_consolidator_null_ut.o agent_test_consolidator_proc_ut.o agent_test_counter_ut.o agent_test_oc.o agent_test.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

install: agent_test
	cp agent_test $(INSTALL_DIR)/agent_test

clean:
	rm -f *.o *.pb.cc *.pb.h agent_test
