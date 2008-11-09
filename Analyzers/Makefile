# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = Armor.o Beatitude.o Door.o Elbereth.o Enhance.o Excalibur.o Explore.o Fight.o Food.o Health.o Loot.o MonsterInfo.o Potion.o Pray.o Ring.o Scroll.o Unihorn.o Valkyrie.o Vault.o Wand.o Weapon.o Wish.o

analyzers: $(OBJECTS)

Armor.o: Armor.h Armor.cpp
Beatitude.o: Beatitude.h Beatitude.cpp
Door.o: Door.h Door.cpp
Elbereth.o: Elbereth.h Elbereth.cpp
Enhance.o: Enhance.h Enhance.cpp
Excalibur.o: Excalibur.h Excalibur.cpp
Explore.o: Explore.h Explore.cpp
Fight.o: Fight.h Fight.cpp
Food.o: Food.h Food.cpp
Health.o: Health.h Health.cpp
Loot.o: Loot.h Loot.cpp
MonsterInfo.o: MonsterInfo.h MonsterInfo.cpp
Potion.o: Potion.h Potion.cpp
Pray.o: Pray.h Pray.cpp
Ring.o: Ring.h Ring.cpp
Scroll.o: Scroll.h Scroll.cpp
Unihorn.o: Unihorn.h Unihorn.cpp
Valkyrie.o: Valkyrie.h Valkyrie.cpp
Vault.o: Vault.h Vault.cpp
Wand.o: Wand.h Wand.cpp
Weapon.o: Weapon.h Weapon.cpp
Wish.o: Wish.h Wish.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
