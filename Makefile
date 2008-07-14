# run it like this:
# make clean && make && export NETHACKOPTIONS=@/home/canidae/projects/saiph/nethackrc && ./saiph 2>err.log

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
LDFLAGS = -lutil
OBJECTS = Analyzer.o Connection.o Map.o MonsterTracker.o Player.o Saiph.o World.o

saiph: $(OBJECTS)
	$(MAKE) -C Analyzers
	$(CXX) $(OBJECTS) Analyzers/*.o $(LDFLAGS) -o saiph

Analyzer.o: Analyzer.h Analyzer.cpp
Connection.o: Connection.h Connection.cpp
Map.o: Map.h Map.cpp
MonsterTracker.o: MonsterTracker.h MonsterTracker.cpp
Player.o: Player.h Player.cpp
Saiph.o: Saiph.h Saiph.cpp
World.o: World.h World.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch saiph
	$(MAKE) -C Analyzers clean
