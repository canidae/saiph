#ifndef PLAYER_H
#define PLAYER_H
/* text length for reading textual attribute/status */
#define MAX_EFFECTS 8
#define MAX_TEXT_LENGTH 16

#include <iostream>

class Player {
	public:
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
		/* effects */
		bool blind;
		bool confused;
		bool foodpoisoned;
		bool hallucinating;
		bool ill;
		bool slimed;
		bool stunned;
		/* position */
		char level[MAX_TEXT_LENGTH];
		int row;
		int col;
		/* zorkmids */
		int zorkmids;
		/* turn */
		int turn;
		/* intrinsics */
		bool cold_resistance;
		bool disintegration_resistance;
		bool fire_resistance;
		bool poison_resistance;
		bool shock_resistance;
		bool sleep_resistance;
		bool telepathy;
		bool teleport_control;
		bool teleportitis;
		/* lycanthropy */
		bool lycanthropy;
		/* hurt leg */
		bool hurt_leg;

		Player();

		bool parseAttributeRow(const char *attributerow);
		bool parseStatusRow(const char *statusrow);

	private:
		/* for parsing text */
		char effects[MAX_EFFECTS][MAX_TEXT_LENGTH];
};
#endif
