# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = DungeonAnalyzer.o ExploreAnalyzer.o FightAnalyzer.o HealthAnalyzer.o LevelAnalyzer.o

analyzers: $(OBJECTS)

DungeonAnalyzer.o: DungeonAnalyzer.h DungeonAnalyzer.cpp
ExploreAnalyzer.o: ExploreAnalyzer.h ExploreAnalyzer.cpp
FightAnalyzer.o: FightAnalyzer.h FightAnalyzer.cpp
HealthAnalyzer.o: HealthAnalyzer.h HealthAnalyzer.cpp
LevelAnalyzer.o: LevelAnalyzer.h LevelAnalyzer.cpp

.PHONY: clean
clean:
	$(RM) $(OBJECTS) *.gch
