CXX      = g++ -w
CPPFLAGS += -I/usr/local/include -pthread -I../lib/h -I../server
CXXFLAGS += -std=c++11

SENSOR_OC_OBJ = OpenConfigFirewall.o OpenConfigInterface.o OpenConfigPort.o OpenConfigFoo.o

all: liboc

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

liboc: $(SENSOR_OC_OBJ) oc.o
	ar rvs liboc.a $^
	mv liboc.a ../lib/

clean:
	rm -f *.o *.a
