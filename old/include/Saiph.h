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

	static int alignment();
	static int charisma();
	static int constitution();
	static int dexterity();
	static int intelligence();
	static int strength();
	static int wisdom();
	static int armor();
	static int encumbrance();
	static int experience();
	static int hunger();
	static int hitpoints();
	static int hitpointsMax();
	static int power();
	static int powerMax();
	static bool blind();
	static bool confused();
	static bool foodpoisoned();
	static bool hallucinating();
	static bool ill();
	static bool slimed();
	static bool stoned();
	static bool stunned();
	static bool hurtLeg();
	static bool hurtLeg(bool hurt_leg);
	static bool polymorphed();
	static bool polymorphed(bool polymorphed);
	static bool engulfed();
	static bool engulfed(bool engulfed);
	static bool inAPit();
	static bool infravision();
	static int skill(int which);
	static int maxSkill(int which);
	static void updateSkills(int* curp);
	static const Coordinate& position();
	static const Coordinate& position(const Coordinate& position);
	static int zorkmids();
	static unsigned long long conducts();
	static unsigned long long addConducts(unsigned long long conducts);
	static unsigned long long removeConducts(unsigned long long conducts);
	static unsigned long long intrinsics();
	static unsigned long long addIntrinsics(unsigned long long intrinsics);
	static unsigned long long removeIntrinsics(unsigned long long intrinsics);
	static unsigned long long extrinsics();
	static unsigned long long addExtrinsics(unsigned long long extrinsics);
	static unsigned long long removeExtrinsics(unsigned long long extrinsics);
	static int lastPrayed();
	static int lastPrayed(int last_prayed);
	static const std::string& name();
	static const std::string& race();
	static int role();
	static int gender();
	static int minSpeed();
	static int maxSpeed();
	static int minMovesThisTurn();
	static int minMovesThisTurn(int min);

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
	/* current skills, 0 = restricted */
	static int _current_skills[];
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
	/* set by World to the number of guaranteed moves this turn */
	static int _min_moves_this_turn;
};
#endif
