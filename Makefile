# run it like this:
# make clean && make && export NETHACKOPTIONS=@/home/canidae/projects/saiph/nethackrc && ./saiph

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
LDFLAGS = -lutil
OBJECTS = Analyzer.o Connection.o Dungeon.o Player.o Saiph.o World.o

saiph: $(OBJECTS)
	$(MAKE) -C Analyzers
	$(CXX) $(OBJECTS) Analyzers/*.o $(LDFLAGS) -o saiph

Analyzer.o: Analyzer.h Analyzer.cpp
Connection.o: Connection.h Connection.cpp
Dungeon.o: Dungeon.h Dungeon.cpp
Player.o: Player.h Player.cpp
Saiph.o: Saiph.h Saiph.cpp
World.o: World.h World.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch saiph
	$(MAKE) -C Analyzers clean
