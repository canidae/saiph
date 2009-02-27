CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -pg -gdwarf-2
OBJECTS = ArmorData.o CloakData.o GlovesData.o HelmetData.o ItemData.o MonsterData.o ShieldData.o ShirtData.o SuitData.o

analyzers: $(OBJECTS)

ArmorData.o: ArmorData.h ArmorData.cpp
ItemData.o: ItemData.h ItemData.cpp
MonsterData.o: MonsterData.h MonsterData.cpp
SuitData.o: SuitData.h SuitData.cpp
ShirtData.o: ShirtData.h ShirtData.cpp
GlovesData.o: GlovesData.h GlovesData.cpp
CloakData.o: CloakData.h CloakData.cpp
HelmetData.o: HelmetData.h HelmetData.cpp
ShieldData.o: ShieldData.h ShieldData.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
