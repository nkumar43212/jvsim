CXX      = g++ -w
CPPFLAGS += -I/usr/local/include -I../lib/h -DINI_STOP_ON_FIRST_ERROR=1
CXXFLAGS += -std=c++11

ROOT_DIR    = .
INSTALL_DIR = $(ROOT_DIR)/bin/$(PLATFORM) 
SRC_DIR     = $(ROOT_DIR)
CPP_FILES   = $(wildcard $(SRC_DIR)/*.cpp)
C_FILES     = $(wildcard $(SRC_DIR)/*.c)
CPP_OBJS    = $(subst .cpp,.o,$(CPP_FILES))
C_OBJS      = $(subst .c,.o,$(C_FILES))

all: libinih

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

libinih: $(CPP_OBJS) $(C_OBJS)
	ar rvs libinih.a $^
	mv libinih.a ../lib/

clean:
	rm -f *.o ../lib/libinih.a
