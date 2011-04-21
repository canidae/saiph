#ifndef SAIPH_H
#define SAIPH_H

#include <string>
#include "Coordinate.h"

/* for parsing effects */
#define MAX_EFFECTS 8
#define MAX_TEXT_LENGTH 16

/* for parsing player attributes */
#define MESSAGE_BASE_ATTRIBUTES "  Base Attributes  "

namespace analyzer {
	class Analyzer;
}

class Saiph {
public:
	static void analyze();
	static void parseMessages(const std::string& messages);
	static bool parseAttributeRow(const char* attributerow);
	static bool parseStatusRow(const char* statusrow, char* levelname, int* turn);

	static int alignment() { return _alignment; }
	static int charisma() { return _charisma; }
	static int constitution() { return _constitution; }
	static int dexterity() { return _dexterity; }
	static int intelligence() { return _intelligence; }
	static int strength() { return _strength; }
	static int wisdom() { return _wisdom; }
	static int armor() { return _armor; }
	static int encumbrance() { return _encumbrance; }
	static int experience() { return _experience; }
	static int hunger() { return _hunger; }
	static int hitpoints() { return _hitpoints; }
	static int hitpointsMax() { return _hitpoints_max; }
	static int power() { return _power; }
	static int powerMax() { return _power_max; }
	static bool blind() { return _blind; }
	static bool confused() { return _confused; }
	static bool foodpoisoned() { return _foodpoisoned; }
	static bool hallucinating() { return _hallucinating; }
	static bool ill() { return _ill; }
	static bool slimed() { return _slimed; }
	static bool stoned() { return _stoned; }
	static bool stunned() { return _stunned; }
	static bool hurtLeg() { return _hurt_leg; }
	static bool hurtLeg(bool hurt_leg);
	static bool polymorphed() { return _polymorphed; }
	static bool polymorphed(bool polymorphed);
	static bool engulfed() { return _engulfed; }
	static bool engulfed(bool engulfed);
	static bool inAPit() { return _in_a_pit; }
	static const Coordinate& position() { return _position; }
	static const Coordinate& position(const Coordinate& position);
	static int zorkmids() { return _zorkmids; }
	static unsigned long long conducts() { return _conducts; }
	static unsigned long long addConducts(unsigned long long conducts);
	static unsigned long long removeConducts(unsigned long long conducts);
	static unsigned long long intrinsics() { return _intrinsics; }
	static unsigned long long addIntrinsics(unsigned long long intrinsics);
	static unsigned long long removeIntrinsics(unsigned long long intrinsics);
	static unsigned long long extrinsics();
	static unsigned long long addExtrinsics(unsigned long long extrinsics);
	static unsigned long long removeExtrinsics(unsigned long long extrinsics);
	static int lastPrayed() { return _last_prayed; }
	static int lastPrayed(int last_prayed);
	static const std::string& name() { return _name; }
	static const std::string& race() { return _race; }
	static int role() { return _role; }
	static int gender() { return _gender; }

private:
	/* attributes */
	static int _alignment;
	static int _charisma;
	static int _constitution;
	static int _dexterity;
	static int _intelligence;
	static int _strength;
	static int _wisdom;
	/* status */
	static int _armor;
	static int _encumbrance;
	static int _experience;
	static int _hunger;
	static int _hitpoints;
	static int _hitpoints_max;
	static int _power;
	static int _power_max;
	/* effects */
	static bool _blind;
	static bool _confused;
	static bool _foodpoisoned;
	static bool _hallucinating;
	static bool _ill;
	static bool _slimed;
	static bool _stoned;
	static bool _stunned;
	static bool _hurt_leg;
	static bool _polymorphed;
	static bool _engulfed;
	static bool _in_a_pit;
	/* position */
	static Coordinate _position;
	/* zorkmids */
	static int _zorkmids;
	/* conducts */
	static unsigned long long _conducts;
	/* intrinsics/extrinsics */
	static unsigned long long _intrinsics;
	static unsigned long long _extrinsics;
	/* last turn she prayed */
	static int _last_prayed;
	/* name */
	static std::string _name;
	/* race */
	static std::string _race;
	/* role */
	static int _role;
	/* gender */
	static int _gender;
	/* for parsing effects */
	static char _effects[MAX_EFFECTS][MAX_TEXT_LENGTH];
};
#endif
