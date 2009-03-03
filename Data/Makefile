CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
OBJECTS = ArmorData.o BootsData.o CloakData.o CorpseData.o FoodData.o GlovesData.o HelmetData.o ItemData.o MonsterData.o ShieldData.o ShirtData.o SuitData.o

analyzers: $(OBJECTS)

ArmorData.o: ArmorData.h ArmorData.cpp
BootsData.o: BootsData.h BootsData.cpp
CloakData.o: CloakData.h CloakData.cpp
CorpseData.o: CorpseData.h CorpseData.cpp
FoodData.o: FoodData.h FoodData.cpp
GlovesData.o: GlovesData.h GlovesData.cpp
HelmetData.o: HelmetData.h HelmetData.cpp
ItemData.o: ItemData.h ItemData.cpp
MonsterData.o: MonsterData.h MonsterData.cpp
ShieldData.o: ShieldData.h ShieldData.cpp
ShirtData.o: ShirtData.h ShirtData.cpp
SuitData.o: SuitData.h SuitData.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
