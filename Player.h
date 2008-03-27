#ifndef PLAYER_H
#define PLAYER_H

/* alignment */
#define CHAOTIC -1
#define NEUTRAL 0
#define LAWFUL 1
/* encumbrance */
#define UNENCUMBERED 0
#define BURDENED 1
#define STRESSED 2
#define STRAINED 3
#define OVERTAXED 4
#define OVERLOADED 5
/* hunger */
#define FAINTING -3
#define WEAK -2
#define HUNGRY -1
#define CONTENT 0
#define SATIATED 1
#define OVERSATIATED 2

#include <iostream>
#include "Globals.h"

using namespace std;

struct Attributes {
	/* numeric values */
	int strength;
	int dexterity;
	int constitution;
	int intelligence;
	int wisdom;
	int charisma;
	int alignment; // see defined constants
};

struct Status {
	/* numeric values */
	int dungeon;
	int zorkmids;
	int hitpoints;
	int hitpoints_max;
	int power;
	int power_max;
	int armor_class;
	int experience;
	int turn;
	int encumbrance; // see defined constants
	int hunger; // see defined constants
	bool blind;
	bool confused;
	bool foodpoisoned;
	bool hallucinating;
	bool ill;
	bool slimed;
	bool stunned;
};

class Player {
	public:
		/* variables */
		Attributes attributes;
		Status status;
		int row; // player position row
		int col; // player position col

		/* constructors */
		Player();

		/* destructors */
		~Player();

		/* methods */
		void parseAttributeRow(const char *attributerow);
		void parseStatusRow(const char *statusrow);
};
#endif
