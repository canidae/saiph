CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
OBJECTS = Amulet.o Armor.o Boots.o Cloak.o Corpse.o Food.o Gloves.o Helmet.o Item.o Monster.o Shield.o Shirt.o Suit.o Weapon.o

analyzers: $(OBJECTS)

Amulet.o: Amulet.h Amulet.cpp
Armor.o: Armor.h Armor.cpp
Boots.o: Boots.h Boots.cpp
Cloak.o: Cloak.h Cloak.cpp
Corpse.o: Corpse.h Corpse.cpp
Food.o: Food.h Food.cpp
Gloves.o: Gloves.h Gloves.cpp
Helmet.o: Helmet.h Helmet.cpp
Item.o: Item.h Item.cpp
Monster.o: Monster.h Monster.cpp
Shield.o: Shield.h Shield.cpp
Shirt.o: Shirt.h Shirt.cpp
Suit.o: Suit.h Suit.cpp
Weapon.o: Weapon.h Weapon.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
