#ifndef PLAYER_H
/* defines */
#define PLAYER_H
/* text length for reading textual attribute/status */
#define MAX_TEXT_LENGTH 16
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

/* includes */
#include <iostream>

/* forward declare */
class Player;

/* includes */

/* namespace */
using namespace std;

/* all values found in the attribute row */
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

/* all values found in the status row */
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

/* the player class holds various info about the player */
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
