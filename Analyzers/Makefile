# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -g
OBJECTS = Beatitude.o Door.o Elbereth.o Enhance.o Excalibur.o Explore.o Fight.o Food.o Health.o Loot.o Potion.o Pray.o Scroll.o Wand.o Wish.o

analyzers: $(OBJECTS)

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
Potion.o: Potion.h Potion.cpp
Pray.o: Pray.h Pray.cpp
Scroll.o: Scroll.h Scroll.cpp
Wand.o: Wand.h Wand.cpp
Wish.o: Wish.h Wish.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
