# makefile for the filters

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS += MonsterFilter.o

MonsterFilter.o: MonsterFilter.h MonsterFilter.cpp

.PHONY: clean
clean:
	$(RM) $(OBJECTS) *.gch
