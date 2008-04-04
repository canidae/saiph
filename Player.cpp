#include "Player.h"

/* constructors */
Player::Player() {
	/* zero attributes */
	attributes.strength = 0;
	attributes.dexterity = 0;
	attributes.constitution = 0;
	attributes.intelligence = 0;
	attributes.wisdom = 0;
	attributes.charisma = 0;
	attributes.alignment = 0;

	/* zero status */
	status.dungeon = 0;
	status.zorkmids = 0;
	status.hitpoints = 0;
	status.hitpoints_max = 0;
	status.power = 0;
	status.power_max = 0;
	status.armor_class = 0;
	status.experience = 0;
	status.turn = 0;
	status.encumbrance = 0;
	status.hunger = 0;
	status.blind = false;
	status.confused = false;
	status.foodpoisoned = false;
	status.hallucinating = false;
	status.ill = false;
	status.slimed = false;
	status.stunned = false;

	/* zero position */
	col = 0;
	row = 0;
}

/* methods */
bool Player::parseAttributeRow(const char *attributerow) {
	/* fetch attributes */
	char alignment[MAX_TEXT_LENGTH];
	alignment[0] = '\0';
	int matched = sscanf(attributerow, "%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%s", &attributes.strength, &attributes.dexterity, &attributes.constitution, &attributes.intelligence, &attributes.wisdom, &attributes.charisma, alignment);
	if (matched < 7) {
		cerr << "Error parsing attribute line, expected 7 values, found " << matched << endl;
		cerr << attributerow << endl;
		return false;
	}
	if (alignment[0] == 'L')
		attributes.alignment = LAWFUL;
	else if (alignment[0] == 'N')
		attributes.alignment = NEUTRAL;
	else    
		attributes.alignment = CHAOTIC;
	return true;
}

bool Player::parseStatusRow(const char *statusrow) {
	/* fetch status */
	status.encumbrance = 0;
	status.hunger = 0;
	status.blind = false;
	status.confused = false;
	status.foodpoisoned = false;
	status.hallucinating = false;
	status.ill = false;
	status.slimed = false;
	status.stunned = false;
	char effects[5][MAX_TEXT_LENGTH];
	int matched = sscanf(statusrow, "%*[^:]:%d%*[^:]:%d%*[^:]:%d(%d%*[^:]:%d(%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%s%s%s%s%s", &status.dungeon, &status.zorkmids, &status.hitpoints, &status.hitpoints_max, &status.power, &status.power_max, &status.armor_class, &status.experience, &status.turn, effects[0], effects[1], effects[2], effects[3], effects[4]);
	if (matched < 9) {
		cerr << "Error parsing status line, expected at least 9 values, found " << matched << endl;
		cerr << statusrow << endl;
		return false;
	}
	int effects_found = matched - 9;
	for (int a = 0; a < effects_found; ++a) {
		if (strcmp(effects[a], "Burdened") == 0) {
			status.encumbrance = BURDENED;
		} else if (strcmp(effects[a], "Stressed") == 0) {
			status.encumbrance = STRESSED;
		} else if (strcmp(effects[a], "Strained") == 0) {
			status.encumbrance = STRAINED;
		} else if (strcmp(effects[a], "Overtaxed") == 0) {
			status.encumbrance = OVERTAXED;
		} else if (strcmp(effects[a], "Overloaded") == 0) {
			status.encumbrance = OVERLOADED;
		} else if (strcmp(effects[a], "Fainting") == 0) {
			status.hunger = FAINTING;
		} else if (strcmp(effects[a], "Weak") == 0) {
			status.hunger = WEAK;
		} else if (strcmp(effects[a], "Hungry") == 0) {
			status.hunger = HUNGRY;
		} else if (strcmp(effects[a], "Satiated") == 0) {
			status.hunger = SATIATED;
		} else if (strcmp(effects[a], "Oversatiated") == 0) {
			status.hunger = OVERSATIATED;
		} else if (strcmp(effects[a], "Blind") == 0) {
			status.blind = true;
		} else if (strcmp(effects[a], "Conf") == 0) {
			status.confused = true;
		} else if (strcmp(effects[a], "FoodPois") == 0) {
			status.foodpoisoned = true;
		} else if (strcmp(effects[a], "Hallu") == 0) {
			status.hallucinating = true;
		} else if (strcmp(effects[a], "Ill") == 0) {
			status.ill = true;
		} else if (strcmp(effects[a], "Slime") == 0) {
			status.slimed = true;
		} else if (strcmp(effects[a], "Stun") == 0) {
			status.stunned = true;
		}
	}
	return true;
}
