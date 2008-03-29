# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = DoorAnalyzer.o ExploreAnalyzer.o FightAnalyzer.o HealthAnalyzer.o

analyzers: $(OBJECTS)

DoorAnalyzer.o: DoorAnalyzer.h DoorAnalyzer.cpp
ExploreAnalyzer.o: ExploreAnalyzer.h ExploreAnalyzer.cpp
FightAnalyzer.o: FightAnalyzer.h FightAnalyzer.cpp
HealthAnalyzer.o: HealthAnalyzer.h HealthAnalyzer.cpp

.PHONY: clean
clean:
	$(RM) $(OBJECTS) *.gch
