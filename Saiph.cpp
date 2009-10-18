#include "Globals.h"
#include "Saiph.h"

using namespace analyzer;
using namespace std;

/* static variables */
/* attributes */
int Saiph::alignment = NEUTRAL; // see defined constants
int Saiph::charisma = 0;
int Saiph::constitution = 0;
int Saiph::dexterity = 0;
int Saiph::intelligence = 0;
int Saiph::strength = 0;
int Saiph::wisdom = 0;
/* status */
int Saiph::armor_class = 0;
int Saiph::encumbrance = UNENCUMBERED; // see defined constants
int Saiph::experience = 0;
int Saiph::hunger = CONTENT; // see defined constants
int Saiph::hitpoints = 0;
int Saiph::hitpoints_max = 0;
int Saiph::power = 0;
int Saiph::power_max = 0;
/* effects */
bool Saiph::blind = false;
bool Saiph::confused = false;
bool Saiph::foodpoisoned = false;
bool Saiph::hallucinating = false;
bool Saiph::ill = false;
bool Saiph::slimed = false;
bool Saiph::stunned = false;
bool Saiph::hurt_leg = false;
bool Saiph::polymorphed = false;
/* position */
Coordinate Saiph::position;
/* zorkmids */
int Saiph::zorkmids = 0;
/* intrinsics/extrinsics */
unsigned long long int Saiph::intrinsics = 0;
unsigned long long int Saiph::extrinsics = 0;
/* last pray turn */
int Saiph::last_pray_turn = 0;

/* methods */
void Saiph::analyze() {
}

void Saiph::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_COLD_RES_GAIN1, 0) != string::npos)
		intrinsics |= PROPERTY_COLD;
	if (messages.find(MESSAGE_COLD_RES_LOSE1, 0) != string::npos)
		intrinsics &= ~PROPERTY_COLD;
	if (messages.find(MESSAGE_DISINTEGRATION_RES_GAIN1, 0) != string::npos || messages.find(MESSAGE_DISINTEGRATION_RES_GAIN2, 0) != string::npos)
		intrinsics |= PROPERTY_DISINT;
	if (messages.find(MESSAGE_FIRE_RES_GAIN1, 0) != string::npos || messages.find(MESSAGE_FIRE_RES_GAIN2, 0) != string::npos)
		intrinsics |= PROPERTY_FIRE;
	if (messages.find(MESSAGE_FIRE_RES_LOSE1, 0) != string::npos)
		intrinsics &= ~PROPERTY_FIRE;
	if (messages.find(MESSAGE_POISON_RES_GAIN1, 0) != string::npos || messages.find(MESSAGE_POISON_RES_GAIN2, 0) != string::npos)
		intrinsics |= PROPERTY_POISON;
	if (messages.find(MESSAGE_POISON_RES_LOSE1, 0) != string::npos)
		intrinsics &= ~PROPERTY_POISON;
	if (messages.find(MESSAGE_SHOCK_RES_GAIN1, 0) != string::npos || messages.find(MESSAGE_SHOCK_RES_GAIN2, 0) != string::npos)
		intrinsics |= PROPERTY_SHOCK;
	if (messages.find(MESSAGE_SHOCK_RES_LOSE1, 0) != string::npos)
		intrinsics &= ~PROPERTY_SHOCK;
	if (messages.find(MESSAGE_SLEEP_RES_GAIN1, 0) != string::npos)
		intrinsics |= PROPERTY_SLEEP;
	if (messages.find(MESSAGE_SLEEP_RES_LOSE1, 0) != string::npos)
		intrinsics &= ~PROPERTY_SLEEP;
	if (messages.find(MESSAGE_TELEPATHY_GAIN1, 0) != string::npos)
		intrinsics |= PROPERTY_ESP;
	if (messages.find(MESSAGE_TELEPATHY_LOSE1, 0) != string::npos)
		intrinsics &= ~PROPERTY_ESP;
	if (messages.find(MESSAGE_TELEPORT_CONTROL_GAIN1, 0) != string::npos || messages.find(MESSAGE_TELEPORT_CONTROL_GAIN2, 0) != string::npos)
		intrinsics |= PROPERTY_TELEPORT_CONTROL;
	if (messages.find(MESSAGE_TELEPORTITIS_GAIN1, 0) != string::npos || messages.find(MESSAGE_TELEPORTITIS_GAIN2, 0) != string::npos)
		intrinsics |= PROPERTY_TELEPORT;
	if (messages.find(MESSAGE_TELEPORTITIS_LOSE1, 0) != string::npos)
		intrinsics &= ~PROPERTY_TELEPORT;
	if (messages.find(MESSAGE_LYCANTHROPY_LOSE1, 0) != string::npos)
		intrinsics |= PROPERTY_LYCANTHROPY;
	if (messages.find(MESSAGE_LYCANTHROPY_GAIN1, 0) != string::npos)
		intrinsics &= ~PROPERTY_LYCANTHROPY;
	if (messages.find(MESSAGE_HURT_LEFT_LEG, 0) != string::npos || messages.find(MESSAGE_HURT_RIGHT_LEG, 0) != string::npos)
		hurt_leg = true;
	if (messages.find(MESSAGE_LEG_IS_BETTER, 0) != string::npos)
		hurt_leg = false;
	if (messages.find(MESSAGE_POLYMORPH, 0) != string::npos)
		polymorphed = true;
	if (messages.find(MESSAGE_UNPOLYMORPH, 0) != string::npos)
		polymorphed = false;
	if (messages.find(MESSAGE_LEVITATION_GAIN1, 0) != string::npos || messages.find(MESSAGE_LEVITATION_GAIN2, 0) != string::npos)
		extrinsics |= PROPERTY_LEVITATION;
	if (messages.find(MESSAGE_LEVITATION_LOSE1, 0) != string::npos || messages.find(MESSAGE_LEVITATION_LOSE2, 0) != string::npos)
		extrinsics &= ~PROPERTY_LEVITATION;
}
