# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
#OBJECTS = Amulet.o Armor.o Beatitude.o Blind.o Dig.o Donate.o Door.o Elbereth.o Enhance.o Excalibur.o Explore.o Fight.o Food.o Genocide.o Health.o Lamp.o Loot.o MonsterInfo.o Passtune.o Potion.o Ring.o Rub.o Scroll.o Shop.o Sokoban.o Throne.o Unihorn.o Valkyrie.o Vault.o Wand.o Weapon.o Wish.o
OBJECTS = Analyzer.o Amulet.o Door.o Elbereth.o Explore.o Fight.o Health.o Loot.o

analyzers: $(OBJECTS)

Analyzer.o: Analyzer.h Analyzer.cpp
Amulet.o: Amulet.h Amulet.cpp
Armor.o: Armor.h Armor.cpp
Beatitude.o: Beatitude.h Beatitude.cpp
Blind.o: Blind.h Blind.cpp
Dig.o: Dig.h Dig.cpp
Donate.o: Donate.h Donate.cpp
Door.o: Door.h Door.cpp
Elbereth.o: Elbereth.h Elbereth.cpp
Enhance.o: Enhance.h Enhance.cpp
Excalibur.o: Excalibur.h Excalibur.cpp
Explore.o: Explore.h Explore.cpp
Fight.o: Fight.h Fight.cpp
Food.o: Food.h Food.cpp
Genocide.o: Genocide.h Genocide.cpp
Health.o: Health.h Health.cpp
Lamp.o: Lamp.h Lamp.cpp
Loot.o: Loot.h Loot.cpp
MonsterInfo.o: MonsterInfo.h MonsterInfo.cpp
Passtune.o: Passtune.h Passtune.cpp
Potion.o: Potion.h Potion.cpp
Ring.o: Ring.h Ring.cpp
Rub.o: Rub.h Rub.cpp
Scroll.o: Scroll.h Scroll.cpp
Shop.o: Shop.h Shop.cpp
Throne.o: Throne.h Throne.cpp
Unihorn.o: Unihorn.h Unihorn.cpp
Valkyrie.o: Valkyrie.h Valkyrie.cpp
Vault.o: Vault.h Vault.cpp
Wand.o: Wand.h Wand.cpp
Weapon.o: Weapon.h Weapon.cpp
Wish.o: Wish.h Wish.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
