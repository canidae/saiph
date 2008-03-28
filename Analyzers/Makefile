# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = HealthAnalyzer.o

HealthAnalyzer.o: HealthAnalyzer.h HealthAnalyzer.cpp

.PHONY: clean
clean:
	$(RM) $(OBJECTS) *.gch
