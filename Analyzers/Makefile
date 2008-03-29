# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = FightAnalyzer.o HealthAnalyzer.o

analyzers: $(OBJECTS)

FightAnalyzer.o: FightAnalyzer.h FightAnalyzer.cpp
HealthAnalyzer.o: HealthAnalyzer.h HealthAnalyzer.cpp

.PHONY: clean
clean:
	$(RM) $(OBJECTS) *.gch
