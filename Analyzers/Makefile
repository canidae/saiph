# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = Explore.o

analyzers: $(OBJECTS)

Explore.o: Explore.h Explore.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
