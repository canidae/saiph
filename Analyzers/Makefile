# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = Door.o Elbereth.o Explore.o Fight.o Health.o Level.o Loot.o

analyzers: $(OBJECTS)

Door.o: Door.h Door.cpp
Elbereth.o: Elbereth.h Elbereth.cpp
Explore.o: Explore.h Explore.cpp
Fight.o: Fight.h Fight.cpp
Health.o: Health.h Health.cpp
Level.o: Level.h Level.cpp
Loot.o: Loot.h Loot.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
