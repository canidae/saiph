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
bool Saiph::lycanthropy = false;
bool Saiph::hurt_leg = false;
bool Saiph::polymorphed = false;
bool Saiph::levitating = false;
/* position */
Coordinate Saiph::position;
/* zorkmids */
int Saiph::zorkmids = 0;
/* intrinsics/extrinsics */
unsigned long long int Saiph::intrinsics = 0;
unsigned long long int Saiph::extrinsics = 0;

/* methods */
void Saiph::analyze() {
}

void Saiph::parseMessages(const string &messages) {
	if (messages.find(SAIPH_GAIN_COLD_RES1, 0) != string::npos)
		intrinsics |= PROPERTY_COLD;
	if (messages.find(SAIPH_LOSE_COLD_RES1, 0) != string::npos)
		intrinsics &= ~PROPERTY_COLD;
	if (messages.find(SAIPH_GAIN_DISINTEGRATION_RES1, 0) != string::npos || messages.find(SAIPH_GAIN_DISINTEGRATION_RES2, 0) != string::npos)
		intrinsics |= PROPERTY_DISINT;
	if (messages.find(SAIPH_GAIN_FIRE_RES1, 0) != string::npos || messages.find(SAIPH_GAIN_FIRE_RES2, 0) != string::npos)
		intrinsics |= PROPERTY_FIRE;
	if (messages.find(SAIPH_LOSE_FIRE_RES1, 0) != string::npos)
		intrinsics &= ~PROPERTY_FIRE;
	if (messages.find(SAIPH_GAIN_POISON_RES1, 0) != string::npos || messages.find(SAIPH_GAIN_POISON_RES2, 0) != string::npos)
		intrinsics |= PROPERTY_POISON;
	if (messages.find(SAIPH_LOSE_POISON_RES1, 0) != string::npos)
		intrinsics &= ~PROPERTY_POISON;
	if (messages.find(SAIPH_GAIN_SHOCK_RES1, 0) != string::npos || messages.find(SAIPH_GAIN_SHOCK_RES2, 0) != string::npos)
		intrinsics |= PROPERTY_SHOCK;
	if (messages.find(SAIPH_LOSE_SHOCK_RES1, 0) != string::npos)
		intrinsics &= ~PROPERTY_SHOCK;
	if (messages.find(SAIPH_GAIN_SLEEP_RES1, 0) != string::npos)
		intrinsics |= PROPERTY_SLEEP;
	if (messages.find(SAIPH_LOSE_SLEEP_RES1, 0) != string::npos)
		intrinsics &= ~PROPERTY_SLEEP;
	if (messages.find(SAIPH_GAIN_TELEPATHY1, 0) != string::npos)
		intrinsics |= PROPERTY_ESP;
	if (messages.find(SAIPH_LOSE_TELEPATHY1, 0) != string::npos)
		intrinsics &= ~PROPERTY_ESP;
	if (messages.find(SAIPH_GAIN_TELEPORT_CONTROL1, 0) != string::npos || messages.find(SAIPH_GAIN_TELEPORT_CONTROL2, 0) != string::npos)
		intrinsics |= PROPERTY_TELEPORT_CONTROL;
	if (messages.find(SAIPH_GAIN_TELEPORTITIS1, 0) != string::npos || messages.find(SAIPH_GAIN_TELEPORTITIS2, 0) != string::npos)
		intrinsics |= PROPERTY_TELEPORT;
	if (messages.find(SAIPH_LOSE_TELEPORTITIS1, 0) != string::npos)
		intrinsics &= ~PROPERTY_TELEPORT;
	if (messages.find(SAIPH_FEEL_PURIFIED, 0) != string::npos)
		lycanthropy = false;
	if (messages.find(SAIPH_FEEL_FEVERISH, 0) != string::npos)
		lycanthropy = true;
	if (messages.find(SAIPH_HURT_LEFT_LEG, 0) != string::npos || messages.find(SAIPH_HURT_RIGHT_LEG, 0) != string::npos)
		hurt_leg = true;
	if (messages.find(SAIPH_LEG_IS_BETTER, 0) != string::npos)
		hurt_leg = false;
	if (messages.find(SAIPH_POLYMORPH, 0) != string::npos)
		polymorphed = true;
	if (messages.find(SAIPH_UNPOLYMORPH, 0) != string::npos)
		polymorphed = false;
	if (messages.find(SAIPH_BEGIN_LEVITATION, 0) != string::npos || messages.find(SAIPH_BEGIN_LEVITATION_PIT, 0) != string::npos)
		levitating = true;
	if (messages.find(SAIPH_END_LEVITATION, 0) != string::npos || messages.find(SAIPH_END_LEVITATION_SINK, 0) != string::npos)
		levitating = false;
}
