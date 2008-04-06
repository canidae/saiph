#include "HealthAnalyzer.h"

/* constructors */
HealthAnalyzer::HealthAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
}

/* methods */
int HealthAnalyzer::parseMessages(string *messages) {
	if (messages->find(HA_ENGRAVE_WITH, 0) != string::npos) {
		action = HA_USE_HANDS;
		return 100;
	} else if (messages->find(HA_ENGRAVE_DUST, 0) != string::npos) {
		action = HA_ENGRAVE_ELBERETH;
		return 100;
	} else if (messages->find(HA_ENGRAVE_DUST_ADD, 0) != string::npos) {
		action = HA_ENGRAVE_ELBERETH;
		return 100;
	} else if (messages->find(HA_ENGRAVE_ADD, 0) != string::npos) {
		action = HA_YES;
		return 100;
	}
	return 0;
}

int HealthAnalyzer::finish() {
	/* figure out if we're in danger of dying */
	/* illness */
	if (saiph->world->player.ill) {
		/* find out the best way to cure this
		 * spell, potion, unihorn, pray
		 *
		 * pray for now */
		action = HA_PRAY;
		return 100;
	}
	/* food */
	if (saiph->world->player.hunger == FAINTING) {
		action = HA_PRAY;
		return 100;
	//} else if (saiph->world->player.hunger == WEAK) {
	//} else if (saiph->world->player.hunger == HUNGRY) {
	}
	/* hp */
	int hp = saiph->world->player.hitpoints;
	int hp_max = saiph->world->player.hitpoints_max;
	if (hp > 0 && (hp < 6 || hp * 7 <= hp_max)) {
		/* almost dead, find an urgent way to heal up.
		 * spell, potion, (elbereth?), pray
		 *
		 * pray for now */
		//saiph->setNextCommand(HA_PRAY, 100);
		action = HA_ENGRAVE;
		return 90;
	} else if (hp > 0 && hp * 2 < hp_max) {
		/* health is going low.
		 * elbereth, run, potion, lots of options
		 *
		 * elbereth for now */
		action = HA_ENGRAVE;
		return 90;
	}
	return 0;
}

void HealthAnalyzer::command() {
	char command[2];
	switch (action) {
		case HA_ENGRAVE:
			command[0] = ENGRAVE;
			command[1] = '\0';
			saiph->world->command(command);
			break;

		case HA_ENGRAVE_ELBERETH:
			saiph->world->command(ELBERETH);
			break;

		case HA_PRAY:
			saiph->world->command(PRAY);
			break;

		case HA_USE_HANDS:
			command[0] = HANDS;
			command[1] = '\0';
			saiph->world->command(command);
			break;

		case HA_YES:
			command[0] = YES;
			command[1] = '\0';
			saiph->world->command(command);
			break;

		default:
			cerr << "undefined action in DoorAnalyzer: " << action << endl;
			exit(1);
			break;
	}
}
