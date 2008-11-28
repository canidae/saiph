#include "Health.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Health::Health(Saiph *saiph) : Analyzer("Health"), saiph(saiph), resting(false), prev_st(INT_MAX), prev_dx(INT_MAX), prev_co(INT_MAX), prev_in(INT_MAX), prev_wi(INT_MAX), prev_ch(INT_MAX) {
}

/* methods */
void Health::analyze() {
	/* do something if our health is low */
	int hp = saiph->world->player.hitpoints;
	int hp_max = saiph->world->player.hitpoints_max;
	if (hp > 0 && hp < hp_max * 4 / 7) {
		/* hp below 4/7 (about 57%), do something! */
		bool doing_something = false;
		if (hp < hp_max * 2 / 7) {
			/* try quaffing healing potion */
			req.request = REQUEST_QUAFF_HEALING;
			req.priority = HEALTH_QUAFF_FOR_HP;
			if (saiph->request(req)) {
				doing_something = true;
			} else if (hp < 6 || hp < hp_max / 7) {
				/* quaffing won't work... how about pray? */
				req.request = REQUEST_PRAY;
				req.priority = HEALTH_PRAY_FOR_HP;
				if (saiph->request(req))
					doing_something = true;
			}
		}
		if (!doing_something) {
			/* try elberething */
			req.request = REQUEST_ELBERETH_OR_REST;
			req.priority = HEALTH_REST_FOR_HP_LOW;
			if (saiph->request(req))
				resting = true;
		} else {
			/* turn off resting if we pray/quaff.
			 * in case we quaff a healing potion and end up with 80% hp,
			 * so that we won't continue elberething in an attempt to get
			 * 90% or more.
			 * when we quaff it usually means we're in trouble, can't
			 * waste time elberething then */
			resting = false;
		}
	}
	if (saiph->world->player.blind || saiph->world->player.confused || saiph->world->player.hallucinating || saiph->world->player.foodpoisoned || saiph->world->player.ill || saiph->world->player.stunned) {
		/* apply unihorn */
		req.request = REQUEST_APPLY_UNIHORN;
		req.priority = (saiph->world->player.foodpoisoned || saiph->world->player.ill) ? HEALTH_CURE_DEADLY : HEALTH_CURE_NON_DEADLY;
		if (!saiph->request(req) && (saiph->world->player.foodpoisoned || saiph->world->player.ill)) {
			/* crap. it's deadly, and unihorn won't work. try eating eucalyptus leaf */
			req.request = REQUEST_EAT;
			req.priority = HEALTH_CURE_DEADLY;
			req.data = "eucalyptus leaf";
			if (saiph->request(req))
				return; // yay, it'll work
			/* no eucalyptus leaf, try praying */
			req.request = REQUEST_PRAY;
			req.priority = HEALTH_CURE_DEADLY;
			saiph->request(req);
		}
	}
	if (resting) {
		/* still resting */
		if (hp > hp_max * 6 / 7) {
			resting = false; // enough hp (greater than about 86%) to continue our journey
		} else {
			req.request = REQUEST_ELBERETH_OR_REST;
			req.priority = HEALTH_REST_FOR_HP_HIGH;
			if (!saiph->request(req)) {
				/* noone would handle our request.
				 * we're bones */
			}
		}
	}
	if (saiph->world->player.lycanthropy) {
		/* cure lycanthropy */
		req.request = REQUEST_EAT;
		req.priority = HEALTH_CURE_LYCANTHROPY;
		req.data = "sprig of wolfsbane";
		if (!saiph->request(req)) {
			/* no? try praying instead */
			req.request = REQUEST_PRAY;
			saiph->request(req);
		}
	}
	if (saiph->world->player.polymorphed) {
		/* cure polymorph */
		req.request = REQUEST_PRAY;
		req.priority = HEALTH_CURE_POLYMORPH;
		saiph->request(req);
	}
	if (prev_st < saiph->world->player.strength || prev_dx < saiph->world->player.dexterity || prev_co < saiph->world->player.constitution || prev_in < saiph->world->player.intelligence || prev_wi < saiph->world->player.wisdom || prev_ch < saiph->world->player.charisma) {
		/* we lost some stats. apply unihorn */
		req.request = REQUEST_APPLY_UNIHORN;
		req.priority = HEALTH_CURE_NON_DEADLY;
		saiph->request(req);
	}
	prev_st = saiph->world->player.strength;
	prev_dx = saiph->world->player.dexterity;
	prev_co = saiph->world->player.constitution;
	prev_in = saiph->world->player.intelligence;
	prev_wi = saiph->world->player.wisdom;
	prev_ch = saiph->world->player.charisma;
}

void Health::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_SLOWING_DOWN, 0) != string::npos || messages.find(MESSAGE_LIMBS_ARE_STIFFENING, 0) != string::npos || messages.find(MESSAGE_LIMBS_TURNED_TO_STONE, 0) != string::npos) {
		/* bloody *trice, this is bad */
		req.request = REQUEST_EAT;
		req.priority = HEALTH_CURE_DEADLY;
		req.data = "partly eaten lizard corpse";
		if (!saiph->request(req)) {
			/* no? try again non-partly eaten */
			req.data = "lizard corpse";
			if (!saiph->request(req)) {
				/* oh crap */
				req.request = REQUEST_PRAY;
				saiph->request(req);
			}
		}
	}
}
