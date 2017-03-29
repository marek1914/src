CFLAGS := `pkg-config --libs --cflags opencv` -g

TARGET := bin/hull bin/findContours

all: $(TARGET)

bin/hull: hull.cpp
	g++ -o $@ hull.cpp $(CFLAGS) 
	
bin/findContours: findContours.cpp
	g++ -o $@ findContours.cpp $(CFLAGS) 
