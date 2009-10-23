#ifndef SAIPH_H
#define SAIPH_H

#include <string>
#include "Coordinate.h"

/* for parsing effects */
#define MAX_EFFECTS 8
#define MAX_TEXT_LENGTH 16

namespace analyzer {
	class Analyzer;
}

class Saiph {
public:
	static void analyze();
	static void parseMessages(const std::string& messages);
	static bool parseAttributeRow(const char* attributerow);
	static bool parseStatusRow(const char* statusrow, char* levelname, int* turn);

	static const int& alignment();
	static const int& charisma();
	static const int& constitution();
	static const int& dexterity();
	static const int& intelligence();
	static const int& strength();
	static const int& wisdom();
	static const int& armor();
	static const int& encumbrance();
	static const int& experience();
	static const int& hunger();
	static const int& hitpoints();
	static const int& hitpointsMax();
	static const int& power();
	static const int& powerMax();
	static const bool& blind();
	static const bool& confused();
	static const bool& foodpoisoned();
	static const bool& hallucinating();
	static const bool& ill();
	static const bool& slimed();
	static const bool& stunned();
	static const bool& hurtLeg();
	static const bool& hurtLeg(const bool& hurt_leg);
	static const bool& polymorphed();
	static const bool& polymorphed(const bool& polymorphed);
	static const bool& engulfed();
	static const bool& engulfed(const bool& engulfed);
	static const Coordinate& position();
	static const Coordinate& position(const Coordinate& position);
	static const int& zorkmids();
	static const unsigned long long int& intrinsics();
	static const unsigned long long int& addIntrinsics(const unsigned long long int& intrinsics);
	static const unsigned long long int& removeIntrinsics(const unsigned long long int& intrinsics);
	static const unsigned long long int& extrinsics();
	static const unsigned long long int& addExtrinsics(const unsigned long long int& extrinsics);
	static const unsigned long long int& removeExtrinsics(const unsigned long long int& extrinsics);
	static const int& lastPrayed();
	static const int& lastPrayed(const int& last_prayed);

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
	static bool _stunned;
	static bool _hurt_leg;
	static bool _polymorphed;
	static bool _engulfed;
	/* position */
	static Coordinate _position;
	/* zorkmids */
	static int _zorkmids;
	/* intrinsics/extrinsics */
	static unsigned long long int _intrinsics;
	static unsigned long long int _extrinsics;
	/* last turn she prayed */
	static int _last_prayed;
	/* for parsing effects */
	static char _effects[MAX_EFFECTS][MAX_TEXT_LENGTH];
};
#endif
