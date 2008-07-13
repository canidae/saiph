# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = Door.o Explore.o Health.o

analyzers: $(OBJECTS)

Door.o: Door.h Door.cpp
Explore.o: Explore.h Explore.cpp
Health.o: Health.h Health.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
