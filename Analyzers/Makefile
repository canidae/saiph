# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = DoorAnalyzer.o Explore.o FoodAnalyzer.o HealthAnalyzer.o LevelAnalyzer.o LootAnalyzer.o MonsterAnalyzer.o

analyzers: $(OBJECTS)

DoorAnalyzer.o: DoorAnalyzer.h DoorAnalyzer.cpp
Explore.o: Explore.h Explore.cpp
FoodAnalyzer.o: FoodAnalyzer.h FoodAnalyzer.cpp
HealthAnalyzer.o: HealthAnalyzer.h HealthAnalyzer.cpp
LevelAnalyzer.o: LevelAnalyzer.h LevelAnalyzer.cpp
LootAnalyzer.o: LootAnalyzer.h LootAnalyzer.cpp
MonsterAnalyzer.o: MonsterAnalyzer.h MonsterAnalyzer.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
