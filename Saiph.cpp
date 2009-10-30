#include <string.h>
#include "Globals.h"
#include "Saiph.h"

using namespace analyzer;
using namespace std;

/* variables */
/* attributes */
int Saiph::_alignment = NEUTRAL; // see defined constants
int Saiph::_charisma = 0;
int Saiph::_constitution = 0;
int Saiph::_dexterity = 0;
int Saiph::_intelligence = 0;
int Saiph::_strength = 0;
int Saiph::_wisdom = 0;
/* status */
int Saiph::_armor = 0;
int Saiph::_encumbrance = UNENCUMBERED; // see defined constants
int Saiph::_experience = 0;
int Saiph::_hunger = CONTENT; // see defined constants
int Saiph::_hitpoints = 0;
int Saiph::_hitpoints_max = 0;
int Saiph::_power = 0;
int Saiph::_power_max = 0;
/* effects */
bool Saiph::_blind = false;
bool Saiph::_confused = false;
bool Saiph::_foodpoisoned = false;
bool Saiph::_hallucinating = false;
bool Saiph::_ill = false;
bool Saiph::_slimed = false;
bool Saiph::_stunned = false;
bool Saiph::_hurt_leg = false;
bool Saiph::_polymorphed = false;
bool Saiph::_engulfed = false;
/* position */
Coordinate Saiph::_position;
/* zorkmids */
int Saiph::_zorkmids = 0;
/* intrinsics/extrinsics */
unsigned long long int Saiph::_intrinsics = 0;
unsigned long long int Saiph::_extrinsics = 0;
/* last turn she prayed */
int Saiph::_last_prayed = 0;
/* effects */
char Saiph::_effects[MAX_EFFECTS][MAX_TEXT_LENGTH] = {
	{'\0'}
};

/* methods */
void Saiph::analyze() {
}

void Saiph::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_COLD_RES_GAIN1) != string::npos)
		_intrinsics |= PROPERTY_COLD;
	if (messages.find(MESSAGE_COLD_RES_LOSE1) != string::npos)
		_intrinsics &= ~PROPERTY_COLD;
	if (messages.find(MESSAGE_DISINTEGRATION_RES_GAIN1) != string::npos || messages.find(MESSAGE_DISINTEGRATION_RES_GAIN2) != string::npos)
		_intrinsics |= PROPERTY_DISINT;
	if (messages.find(MESSAGE_FIRE_RES_GAIN1) != string::npos || messages.find(MESSAGE_FIRE_RES_GAIN2) != string::npos)
		_intrinsics |= PROPERTY_FIRE;
	if (messages.find(MESSAGE_FIRE_RES_LOSE1) != string::npos)
		_intrinsics &= ~PROPERTY_FIRE;
	if (messages.find(MESSAGE_POISON_RES_GAIN1) != string::npos || messages.find(MESSAGE_POISON_RES_GAIN2) != string::npos)
		_intrinsics |= PROPERTY_POISON;
	if (messages.find(MESSAGE_POISON_RES_LOSE1) != string::npos)
		_intrinsics &= ~PROPERTY_POISON;
	if (messages.find(MESSAGE_SHOCK_RES_GAIN1) != string::npos || messages.find(MESSAGE_SHOCK_RES_GAIN2) != string::npos)
		_intrinsics |= PROPERTY_SHOCK;
	if (messages.find(MESSAGE_SHOCK_RES_LOSE1) != string::npos)
		_intrinsics &= ~PROPERTY_SHOCK;
	if (messages.find(MESSAGE_SLEEP_RES_GAIN1) != string::npos)
		_intrinsics |= PROPERTY_SLEEP;
	if (messages.find(MESSAGE_SLEEP_RES_LOSE1) != string::npos)
		_intrinsics &= ~PROPERTY_SLEEP;
	if (messages.find(MESSAGE_TELEPATHY_GAIN1) != string::npos)
		_intrinsics |= PROPERTY_ESP;
	if (messages.find(MESSAGE_TELEPATHY_LOSE1) != string::npos)
		_intrinsics &= ~PROPERTY_ESP;
	if (messages.find(MESSAGE_TELEPORT_CONTROL_GAIN1) != string::npos || messages.find(MESSAGE_TELEPORT_CONTROL_GAIN2) != string::npos)
		_intrinsics |= PROPERTY_TELEPORT_CONTROL;
	if (messages.find(MESSAGE_TELEPORTITIS_GAIN1) != string::npos || messages.find(MESSAGE_TELEPORTITIS_GAIN2) != string::npos)
		_intrinsics |= PROPERTY_TELEPORT;
	if (messages.find(MESSAGE_TELEPORTITIS_LOSE1) != string::npos)
		_intrinsics &= ~PROPERTY_TELEPORT;
	if (messages.find(MESSAGE_LYCANTHROPY_GAIN1) != string::npos)
		_intrinsics |= PROPERTY_LYCANTHROPY;
	if (messages.find(MESSAGE_LYCANTHROPY_LOSE1) != string::npos)
		_intrinsics &= ~PROPERTY_LYCANTHROPY;
	if (messages.find(MESSAGE_HURT_LEFT_LEG) != string::npos || messages.find(MESSAGE_HURT_RIGHT_LEG) != string::npos)
		_hurt_leg = true;
	if (messages.find(MESSAGE_LEG_IS_BETTER) != string::npos)
		_hurt_leg = false;
	if (messages.find(MESSAGE_POLYMORPH) != string::npos)
		_polymorphed = true;
	if (messages.find(MESSAGE_UNPOLYMORPH) != string::npos)
		_polymorphed = false;
	if (messages.find(MESSAGE_LEVITATION_GAIN1) != string::npos || messages.find(MESSAGE_LEVITATION_GAIN2) != string::npos)
		_extrinsics |= PROPERTY_LEVITATION;
	if (messages.find(MESSAGE_LEVITATION_LOSE1) != string::npos || messages.find(MESSAGE_LEVITATION_LOSE2) != string::npos)
		_extrinsics &= ~PROPERTY_LEVITATION;
}

bool Saiph::parseAttributeRow(const char* attributerow) {
	/* fetch attributes */
	int matched = sscanf(attributerow, "%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%s", &_strength, &_dexterity, &_constitution, &_intelligence, &_wisdom, &_charisma, _effects[0]);
	if (matched < 7)
		return false;
	if (_effects[0][0] == 'L')
		_alignment = LAWFUL;
	else if (_effects[0][0] == 'N')
		_alignment = NEUTRAL;
	else
		_alignment = CHAOTIC;
	return true;
}

bool Saiph::parseStatusRow(const char* statusrow, char* levelname, int* turn) {
	/* fetch status */
	_encumbrance = UNENCUMBERED;
	_hunger = CONTENT;
	_blind = false;
	_confused = false;
	_foodpoisoned = false;
	_hallucinating = false;
	_ill = false;
	_slimed = false;
	_stunned = false;
	int matched = sscanf(statusrow, "%16[^$*]%*[^:]:%d%*[^:]:%d(%d%*[^:]:%d(%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%s%s%s%s%s", levelname, &_zorkmids, &_hitpoints, &_hitpoints_max, &_power, &_power_max, &_armor, &_experience, turn, _effects[0], _effects[1], _effects[2], _effects[3], _effects[4]);
	if (matched < 9)
		return false;
	int effects_found = matched - 9;
	for (int e = 0; e < effects_found; ++e) {
		if (strcmp(_effects[e], "Burdened") == 0) {
			_encumbrance = BURDENED;
		} else if (strcmp(_effects[e], "Stressed") == 0) {
			_encumbrance = STRESSED;
		} else if (strcmp(_effects[e], "Strained") == 0) {
			_encumbrance = STRAINED;
		} else if (strcmp(_effects[e], "Overtaxed") == 0) {
			_encumbrance = OVERTAXED;
		} else if (strcmp(_effects[e], "Overloaded") == 0) {
			_encumbrance = OVERLOADED;
		} else if (strcmp(_effects[e], "Fainting") == 0) {
			_hunger = FAINTING;
		} else if (strcmp(_effects[e], "Fainted") == 0) {
			_hunger = FAINTING;
		} else if (strcmp(_effects[e], "Weak") == 0) {
			_hunger = WEAK;
		} else if (strcmp(_effects[e], "Hungry") == 0) {
			_hunger = HUNGRY;
		} else if (strcmp(_effects[e], "Satiated") == 0) {
			_hunger = SATIATED;
		} else if (strcmp(_effects[e], "Oversatiated") == 0) {
			_hunger = OVERSATIATED;
		} else if (strcmp(_effects[e], "Blind") == 0) {
			_blind = true;
		} else if (strcmp(_effects[e], "Conf") == 0) {
			_confused = true;
		} else if (strcmp(_effects[e], "FoodPois") == 0) {
			_foodpoisoned = true;
		} else if (strcmp(_effects[e], "Hallu") == 0) {
			_hallucinating = true;
		} else if (strcmp(_effects[e], "Ill") == 0) {
			_ill = true;
		} else if (strcmp(_effects[e], "Slime") == 0) {
			_slimed = true;
		} else if (strcmp(_effects[e], "Stun") == 0) {
			_stunned = true;
		}
	}
	return true;
}

const int& Saiph::alignment() {
	return _alignment;
}

const int& Saiph::charisma() {
	return _charisma;
}

const int& Saiph::constitution() {
	return _constitution;
}

const int& Saiph::dexterity() {
	return _dexterity;
}

const int& Saiph::intelligence() {
	return _intelligence;
}

const int& Saiph::strength() {
	return _strength;
}

const int& Saiph::wisdom() {
	return _wisdom;
}

const int& Saiph::armor() {
	return _armor;
}

const int& Saiph::encumbrance() {
	return _encumbrance;
}

const int& Saiph::experience() {
	return _experience;
}

const int& Saiph::hunger() {
	return _hunger;
}

const int& Saiph::hitpoints() {
	return _hitpoints;
}

const int& Saiph::hitpointsMax() {
	return _hitpoints_max;
}

const int& Saiph::power() {
	return _power;
}

const int& Saiph::powerMax() {
	return _power_max;
}

const bool& Saiph::blind() {
	return _blind;
}

const bool& Saiph::confused() {
	return _confused;
}

const bool& Saiph::foodpoisoned() {
	return _foodpoisoned;
}

const bool& Saiph::hallucinating() {
	return _hallucinating;
}

const bool& Saiph::ill() {
	return _ill;
}

const bool& Saiph::slimed() {
	return _slimed;
}

const bool& Saiph::stunned() {
	return _stunned;
}

const bool& Saiph::hurtLeg() {
	return _hurt_leg;
}

const bool& Saiph::hurtLeg(const bool& hurt_leg) {
	_hurt_leg = hurt_leg;
	return Saiph::hurtLeg();
}

const bool& Saiph::polymorphed() {
	return _polymorphed;
}

const bool& Saiph::polymorphed(const bool& polymorphed) {
	_polymorphed = polymorphed;
	return Saiph::polymorphed();
}

const bool& Saiph::engulfed() {
	return _engulfed;
}

const bool& Saiph::engulfed(const bool& engulfed) {
	_engulfed = engulfed;
	return Saiph::engulfed();
}

const Coordinate& Saiph::position() {
	return _position;
}

const Coordinate& Saiph::position(const Coordinate& position) {
	_position = position;
	return Saiph::position();
}

const int& Saiph::zorkmids() {
	return _zorkmids;
}

const unsigned long long int& Saiph::intrinsics() {
	return _intrinsics;
}

const unsigned long long int& Saiph::addIntrinsics(const unsigned long long int& intrinsics) {
	_intrinsics |= intrinsics;
	return Saiph::intrinsics();
}

const unsigned long long int& Saiph::removeIntrinsics(const unsigned long long int& intrinsics) {
	_intrinsics &= ~intrinsics;
	return Saiph::intrinsics();
}

const unsigned long long int& Saiph::extrinsics() {
	return _extrinsics;
}

const unsigned long long int& Saiph::addExtrinsics(const unsigned long long int& extrinsics) {
	_extrinsics |= extrinsics;
	return Saiph::extrinsics();
}

const unsigned long long int& Saiph::removeExtrinsics(const unsigned long long int& extrinsics) {
	_extrinsics &= ~extrinsics;
	return Saiph::extrinsics();
}

const int& Saiph::lastPrayed() {
	return _last_prayed;
}

const int& Saiph::lastPrayed(const int& last_prayed) {
	_last_prayed = last_prayed;
	return Saiph::lastPrayed();
}