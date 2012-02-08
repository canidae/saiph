module data.saiph;

import data.coordinate;

class Saiph {
public:
	/* attributes */
	static int alignment;
	static int charisma;
	static int constitution;
	static int dexterity;
	static int intelligence;
	static int strength;
	static int wisdom;
	/* status */
	static int armor;
	static int encumbrance;
	static int experience;
	static int hunger;
	static int hitpoints;
	static int hitpointsMax;
	static int power;
	static int powerMax;
	/* effects */
	static bool blind;
	static bool confused;
	static bool poisoned;
	static bool hallucinating;
	static bool ill;
	static bool slimed;
	static bool stoned;
	static bool stunned;
	static bool hurtLeg;
	static bool polymorphed;
	static bool engulfed;
	static bool inPit;
	/* position */
	static Coordinate position;
	/* zorkmids */
	static int zorkmids;
}
