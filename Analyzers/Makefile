# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = Explore.o Health.o

analyzers: $(OBJECTS)

Explore.o: Explore.h Explore.cpp
Health.o: Health.h Health.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
