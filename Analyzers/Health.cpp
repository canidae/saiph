#include "Health.h"

/* constructors */
Health::Health(Saiph *saiph) : Analyzer("Health"), saiph(saiph) {
	this->resting = false;
}

/* methods */
void Health::command(string *command) {
	*command = action;
}

void Health::finish() {
	/* figure out if we're in danger of dying */
	action = "";
	/* food */
	if (saiph->world->player.hunger == FAINTING) {
		action = PRAY;
		priority = HEALTH_PRAY_FOR_FOOD;
		return;
	}
	/* hp */
	int hp = saiph->world->player.hitpoints;
	int hp_max = saiph->world->player.hitpoints_max;
	if (hp > 0 && hp < hp_max * 3 / 5) {
		/* hp below 60%. heal up */
		request.request = REQUEST_ELBERETH_OR_REST;
		request.priority = HEALTH_REST_FOR_HP;
		resting = true;
		if (!saiph->requestAction(request)) {
			/* noone would handle our request.
			 * we're bones */
		}
	} else if (resting) {
		/* still resting */
		if (hp > hp_max * 9 / 10) {
			resting = false; // enough hp (>90%) to continue our journey
		} else {
			request.request = REQUEST_ELBERETH_OR_REST;
			request.priority = HEALTH_REST_FOR_HP;
			if (!saiph->requestAction(request)) {
				/* noone would handle our request.
				 * we're bones */
			}
		}
	}
}
