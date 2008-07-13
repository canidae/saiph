#include "Health.h"

/* constructors */
Health::Health(Saiph *saiph) {
	this->saiph = saiph;
	this->resting = false;
}

/* methods */
int Health::parseMessages(string *messages) {
	if (messages->find(HA_ENGRAVE_WITH, 0) != string::npos)
		action = HANDS;
	else if (messages->find(HA_ENGRAVE_DUST, 0) != string::npos)
		action = ELBERETH;
	else if (messages->find(HA_ENGRAVE_DUST_ADD, 0) != string::npos)
		action = ELBERETH;
	else if (messages->find(HA_ENGRAVE_ADD, 0) != string::npos)
		action = YES;
	else
		return 0;
	return HEALTH_CONTINUE_ACTION;
}

int Health::finish() {
	/* figure out if we're in danger of dying */
	/* food */
	if (saiph->world->player.hunger == FAINTING) {
		action = PRAY;
		return HEALTH_PRAY_FOR_FOOD;
	}
	/* hp */
	int hp = saiph->world->player.hitpoints;
	int hp_max = saiph->world->player.hitpoints_max;
	if (hp > 0 && (hp < 6 || hp <= hp_max / 7)) {
		/* almost dead, find an urgent way to heal up.
		 * spell, potion, (elbereth?), pray
		 *
		 * pray for now */
		if (saiph->world->player.blind || saiph->world->player.confused || saiph->world->player.stunned || saiph->engulfed)
			return 0;
		action = ENGRAVE;
		resting = true;
		return HEALTH_ENGRAVE_FOR_HP;
	} else if (hp > 0 && hp < hp_max * 3 / 5) {
		/* health is going low.
		 * elbereth, run, potion, lots of options
		 *
		 * elbereth for now */
		if (saiph->world->player.blind || saiph->world->player.confused || saiph->world->player.stunned || saiph->engulfed)
			return 0;
		action = ENGRAVE;
		resting = true;
		return HEALTH_ENGRAVE_FOR_HP;
	} else if (resting) {
		/* when hp drops below 60% then rest up to 80% or more */
		if (hp > 0 && hp >= hp_max * 4 / 5) {
			resting = false;
		} else {
			action = ENGRAVE;
			return HEALTH_REST_FOR_HP;
		}
	}
	return 0;
}

void Health::command(string *command) {
	*command = action;
}
