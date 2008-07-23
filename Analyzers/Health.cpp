#include "Health.h"

/* constructors */
Health::Health(Saiph *saiph) : Analyzer("Health"), saiph(saiph) {
	this->resting = false;
}

/* methods */
void Health::finish() {
	/* do something if our health is low */
	int hp = saiph->world->player.hitpoints;
	int hp_max = saiph->world->player.hitpoints_max;
	if (hp > 0 && hp < hp_max * 3 / 5) {
		/* hp below 60%. heal up */
		req.request = REQUEST_ELBERETH_OR_REST;
		req.priority = HEALTH_REST_FOR_HP;
		resting = true;
		if (!saiph->request(req)) {
			/* elbereth won't work... how about pray? */
			if (hp < 6 || hp < hp_max / 7) {
				/* our hp is low enough, at least */
				req.request = REQUEST_PRAY;
				req.priority = HEALTH_PRAY_FOR_HP;
				saiph->request(req);
			}
		}
	} else if (resting) {
		/* still resting */
		if (hp > hp_max * 9 / 10) {
			resting = false; // enough hp (>90%) to continue our journey
		} else {
			req.request = REQUEST_ELBERETH_OR_REST;
			req.priority = HEALTH_REST_FOR_HP;
			if (!saiph->request(req)) {
				/* noone would handle our request.
				 * we're bones */
			}
		}
	}
}
