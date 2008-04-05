#ifndef PLAYER_H
/* defines */
#define PLAYER_H
/* text length for reading textual attribute/status */
#define MAX_EFFECTS 8
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

/* forward declare */
class Player;

/* includes */
#include <iostream>

/* namespace */
using namespace std;

/* the player class holds various info about the player */
class Player {
	public:
		/* variables */
		/* attributes */
		int alignment; // see defined constants
		int charisma;
		int constitution;
		int dexterity;
		int intelligence;
		int strength;
		int wisdom;
		/* status */
		int armor_class;
		int encumbrance; // see defined constants
		int experience;
		int hunger; // see defined constants
		int hitpoints;
		int hitpoints_max;
		int power;
		int power_max;
		int turn;
		/* effects */
		bool blind;
		bool confused;
		bool foodpoisoned;
		bool hallucinating;
		bool ill;
		bool slimed;
		bool stunned;
		/* position */
		int dungeon;
		int row;
		int col;
		/* zorkmids */
		int zorkmids;
		/* for parsing text */
		char effects[MAX_EFFECTS][MAX_TEXT_LENGTH];

		/* constructors */
		Player();

		/* destructors */
		~Player();

		/* methods */
		bool parseAttributeRow(const char *attributerow);
		bool parseStatusRow(const char *statusrow);
};
#endif
