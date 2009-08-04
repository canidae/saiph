CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
LDFLAGS = -lutil -g3 -gdwarf-2
OBJECTS = Command.o Connection.o Coordinate.o Debug.o EventBus.o Inventory.o Item.o Level.o Local.o Monster.o PathNode.o Point.o Saiph.o Telnet.o World.o

saiph: $(OBJECTS) Actions/*.h Actions/*.cpp Analyzers/*.h Analyzers/*.cpp Data/*.h Data/*.cpp Events/*.h Events/*.cpp
	$(MAKE) -C Actions
	$(MAKE) -C Analyzers
	$(MAKE) -C Data
	$(MAKE) -C Events
	$(CXX) $(OBJECTS) Actions/*.o Analyzers/*.o Data/*.o Events/*.o $(LDFLAGS) -o saiph

Command.o: Command.h Command.cpp
Connection.o: Connection.h Connection.cpp
Coordinate.o: Coordinate.h Coordinate.cpp
Debug.o: Debug.h Debug.cpp
EventBus.o: EventBus.h EventBus.cpp
Inventory.o: Inventory.h Inventory.cpp
Item.o: Item.h Item.cpp
Level.o: Level.h Level.cpp
Local.o: Local.h Local.cpp
Monster.o: Monster.h Monster.cpp
PathNode.o: PathNode.h PathNode.cpp
Point.o: Point.h Point.cpp
Saiph.o: Saiph.h Saiph.cpp
Telnet.o: Telnet.h Telnet.cpp
World.o: World.h World.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch saiph
	$(MAKE) -C Actions clean
	$(MAKE) -C Analyzers clean
	$(MAKE) -C Data clean
	$(MAKE) -C Events clean

#Launch game
game:
	echo -e '\e[8;50;160t' # Sets the terminal size
	./saiph 2> err.log
