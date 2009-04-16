#ifndef PLAYER_H
#define PLAYER_H
/* text length for reading textual attribute/status */
#define MAX_EFFECTS 8
#define MAX_TEXT_LENGTH 16

class Player {
	public:
		/* attributes */
		static int alignment; // see defined constants
		static int charisma;
		static int constitution;
		static int dexterity;
		static int intelligence;
		static int strength;
		static int wisdom;
		/* status */
		static int armor_class;
		static int encumbrance; // see defined constants
		static int experience;
		static int hunger; // see defined constants
		static int hitpoints;
		static int hitpoints_max;
		static int power;
		static int power_max;
		/* effects */
		static bool blind;
		static bool confused;
		static bool foodpoisoned;
		static bool hallucinating;
		static bool ill;
		static bool slimed;
		static bool stunned;
		/* position */
		static char level[MAX_TEXT_LENGTH];
		static int row;
		static int col;
		/* zorkmids */
		static int zorkmids;
		/* turn */
		static int turn;
		/* intrinsics */
		static unsigned long long int intrinsics;
		static unsigned long long int extrinsics;
		/* lycanthropy */
		static bool lycanthropy;
		/* hurt leg */
		static bool hurt_leg;
		/* polymorphed */
		static bool polymorphed;
		/* engulfed */
		static bool engulfed;
		/* levitating */
		static bool levitating;

		Player();

		static bool parseAttributeRow(const char *attributerow);
		static bool parseStatusRow(const char *statusrow);

	private:
		/* for parsing text */
		static char effects[MAX_EFFECTS][MAX_TEXT_LENGTH];
};
#endif
