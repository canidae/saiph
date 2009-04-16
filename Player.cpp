#include <string.h>
#include <string>
#include "Globals.h"
#include "Player.h"

/* static variables */
/* attributes */
int Player::alignment = NEUTRAL; // see defined constants
int Player::charisma = 0;
int Player::constitution = 0;
int Player::dexterity = 0;
int Player::intelligence = 0;
int Player::strength = 0;
int Player::wisdom = 0;
/* status */
int Player::armor_class = 0;
int Player::encumbrance = UNENCUMBERED; // see defined constants
int Player::experience = 0;
int Player::hunger = CONTENT; // see defined constants
int Player::hitpoints = 0;
int Player::hitpoints_max = 0;
int Player::power = 0;
int Player::power_max = 0;
/* effects */
bool Player::blind = false;
bool Player::confused = false;
bool Player::foodpoisoned = false;
bool Player::hallucinating = false;
bool Player::ill = false;
bool Player::slimed = false;
bool Player::stunned = false;
/* position */
char Player::level[MAX_TEXT_LENGTH] = {'\0'};
int Player::row = 0;
int Player::col = 0;
/* zorkmids */
int Player::zorkmids = 0;
/* turn */
int Player::turn = 0;
/* intrinsics */
unsigned long long int Player::intrinsics = 0;
unsigned long long int Player::extrinsics = 0;
/* lycanthropy */
bool Player::lycanthropy = false;
/* hurt leg */
bool Player::hurt_leg = false;
/* polymorphed */
bool Player::polymorphed = false;
/* engulfed */
bool Player::engulfed = false;
/* levitating */
bool Player::levitating = false;
/* for parsing text */
char Player::effects[MAX_EFFECTS][MAX_TEXT_LENGTH] = {{'\0'}};

/* constructors/destructor */
Player::Player() {
}

/* methods */
bool Player::parseAttributeRow(const char *attributerow) {
	/* fetch attributes */
	int matched = sscanf(attributerow, "%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%s", &strength, &dexterity, &constitution, &intelligence, &wisdom, &charisma, effects[0]);
	if (matched < 7)
		return false;
	if (effects[0][0] == 'L')
		alignment = LAWFUL;
	else if (effects[0][0] == 'N')
		alignment = NEUTRAL;
	else
		alignment = CHAOTIC;
	return true;
}

bool Player::parseStatusRow(const char *statusrow) {
	/* fetch status */
	encumbrance = 0;
	hunger = 0;
	blind = false;
	confused = false;
	foodpoisoned = false;
	hallucinating = false;
	ill = false;
	slimed = false;
	stunned = false;
	int matched = sscanf(statusrow, "%16[^$*]%*[^:]:%d%*[^:]:%d(%d%*[^:]:%d(%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%s%s%s%s%s", level, &zorkmids, &hitpoints, &hitpoints_max, &power, &power_max, &armor_class, &experience, &turn, effects[0], effects[1], effects[2], effects[3], effects[4]);
	if (matched < 9)
		return false;
	int effects_found = matched - 9;
	for (int e = 0; e < effects_found; ++e) {
		if (strcmp(effects[e], "Burdened") == 0) {
			encumbrance = BURDENED;
		} else if (strcmp(effects[e], "Stressed") == 0) {
			encumbrance = STRESSED;
		} else if (strcmp(effects[e], "Strained") == 0) {
			encumbrance = STRAINED;
		} else if (strcmp(effects[e], "Overtaxed") == 0) {
			encumbrance = OVERTAXED;
		} else if (strcmp(effects[e], "Overloaded") == 0) {
			encumbrance = OVERLOADED;
		} else if (strcmp(effects[e], "Fainting") == 0) {
			hunger = FAINTING;
		} else if (strcmp(effects[e], "Fainted") == 0) {
			hunger = FAINTING;
		} else if (strcmp(effects[e], "Weak") == 0) {
			hunger = WEAK;
		} else if (strcmp(effects[e], "Hungry") == 0) {
			hunger = HUNGRY;
		} else if (strcmp(effects[e], "Satiated") == 0) {
			hunger = SATIATED;
		} else if (strcmp(effects[e], "Oversatiated") == 0) {
			hunger = OVERSATIATED;
		} else if (strcmp(effects[e], "Blind") == 0) {
			blind = true;
		} else if (strcmp(effects[e], "Conf") == 0) {
			confused = true;
		} else if (strcmp(effects[e], "FoodPois") == 0) {
			foodpoisoned = true;
		} else if (strcmp(effects[e], "Hallu") == 0) {
			hallucinating = true;
		} else if (strcmp(effects[e], "Ill") == 0) {
			ill = true;
		} else if (strcmp(effects[e], "Slime") == 0) {
			slimed = true;
		} else if (strcmp(effects[e], "Stun") == 0) {
			stunned = true;
		}
	}
	return true;
}
