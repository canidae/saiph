#ifndef PLAYER_H
#define PLAYER_H

using namespace std;

struct Attributes {
	/* numeric values */
	int strength;
	int dexterity;
	int constitution;
	int intelligence;
	int wisdom;
	int charisma;
	/* alignment shows up as text */
	int alignment; // -1 chaotic, 0 neutral, 1 lawful
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
	/* status that shows up as text */
	int encumbrance; // 0 <nothing>, 1 burdened, 2 stressed, 3 strained, 4 overtaxed, 5 overloaded
	int hunger; // -3 fainting, -2 weak, -1 hungry, 0 <nothing>, 1 satiated, 2 oversatiated
	bool blind;
	bool confused;
	bool foodpoisoned;
	bool hallucinating;
	bool ill;
	bool slimed;
	bool stunned;
};

struct Position {
	int col;
	int row;
};

class Player {
	public:
		/* variables */
		Attributes attributes;
		Status status;
		Position position;

		/* constructors */
		Player();

		/* destructors */
		~Player();

		/* methods */

	private:
};
#endif
