CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -pg -gdwarf-2
OBJECTS = ArmorData.o ItemData.o SuitData.o

analyzers: $(OBJECTS)

ArmorData.o: ArmorData.h ArmorData.cpp
ItemData.o: ItemData.h ItemData.cpp
SuitData.o: SuitData.h SuitData.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
