# run it like this:
# make clean && make && export NETHACKOPTIONS=@/home/canidae/projects/saiph/nethackrc && ./saiph

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
LDFLAGS = -lutil
OBJECTS = Analyzer.o Connection.o Dungeon.o Filter.o MessageParser.o Player.o Saiph.o World.o

saiph: $(OBJECTS)
	$(MAKE) -C Filters
	$(CXX) $(OBJECTS) Filters/*.o $(LDFLAGS) -o saiph

Analyzer.o: Analyzer.h Analyzer.cpp
Connection.o: Connection.h Connection.cpp
Dungeon.o: Dungeon.h Dungeon.cpp
Filter.o: Filter.h Filter.cpp
Player.o: Player.h Player.cpp
MessageParser.o: MessageParser.h MessageParser.cpp
Saiph.o: Saiph.h Saiph.cpp
World.o: World.h World.cpp

.PHONY: clean
clean:
	$(RM) $(OBJECTS) *.gch saiph
	$(MAKE) -C Filters clean
