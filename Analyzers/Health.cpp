#include "Health.h"
#include "../Saiph.h"
#include "../World.h"

using namespace analyzer;
using namespace std;

/* constructors/destructor */
Health::Health() : Analyzer("Health"), resting(false) {
}

/* methods */
void Health::analyze() {
	/* do something if our health is low */
	if (Saiph::hitpoints > 0 && Saiph::hitpoints < Saiph::hitpoints_max * 4 / 7) {
		/* hp below 4/7 (about 57%), do something! */
		bool doing_something = false;
		if (Saiph::hitpoints < Saiph::hitpoints_max * 2 / 7) {
			/* try quaffing healing potion */
			req.request = REQUEST_QUAFF_HEALING;
			req.priority = PRIORITY_HEALTH_QUAFF_FOR_HP;
			if (saiph->request(req)) {
				doing_something = true;
			} else if (Saiph::hitpoints < 6 || Saiph::hitpoints < Saiph::hitpoints_max / 7) {
				/* quaffing won't work... how about pray? */
				req.request = REQUEST_PRAY;
				req.priority = PRIORITY_HEALTH_PRAY_FOR_HP;
				if (saiph->request(req))
					doing_something = true;
			}
		}
		if (!doing_something) {
			/* try elberething */
			req.request = REQUEST_ELBERETH_OR_REST;
			req.priority = PRIORITY_HEALTH_REST_FOR_HP_LOW;
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
	if (Saiph::confused || Saiph::hallucinating || Saiph::foodpoisoned || Saiph::ill || Saiph::stunned) {
		/* apply unihorn */
		req.request = REQUEST_APPLY_UNIHORN;
		req.priority = (Saiph::foodpoisoned || Saiph::ill) ? PRIORITY_HEALTH_CURE_DEADLY : PRIORITY_HEALTH_CURE_NON_DEADLY;
		if (!saiph->request(req) && (Saiph::foodpoisoned || Saiph::ill)) {
			/* crap. it's deadly, and unihorn won't work. try eating eucalyptus leaf */
			req.request = REQUEST_EAT;
			req.priority = PRIORITY_HEALTH_CURE_DEADLY;
			req.data = "eucalyptus leaf";
			if (saiph->request(req))
				return; // yay, it'll work
			/* no eucalyptus leaf, try praying */
			req.request = REQUEST_PRAY;
			req.priority = PRIORITY_HEALTH_CURE_DEADLY;
			saiph->request(req);
		}
	}
	if (resting) {
		/* still resting */
		if (Saiph::hitpoints > Saiph::hitpoints_max * 6 / 7) {
			resting = false; // enough hp (greater than about 86%) to continue our journey
		} else {
			req.request = REQUEST_ELBERETH_OR_REST;
			req.priority = PRIORITY_HEALTH_REST_FOR_HP_HIGH;
			if (!saiph->request(req)) {
				/* noone would handle our request.
				 * we're bones */
			}
		}
	}
	if (Saiph::intrinsics & PROPERTY_LYCANTHROPY) {
		/* cure lycanthropy */
		req.request = REQUEST_EAT;
		req.priority = PRIORITY_HEALTH_CURE_LYCANTHROPY;
		req.data = "sprig of wolfsbane";
		if (!saiph->request(req)) {
			/* no? try praying instead */
			req.request = REQUEST_PRAY;
			saiph->request(req);
		}
	}
	if (Saiph::polymorphed) {
		/* cure polymorph */
		req.request = REQUEST_PRAY;
		req.priority = PRIORITY_HEALTH_CURE_POLYMORPH;
		saiph->request(req);
	}
}

void Health::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_SLOWING_DOWN) != string::npos || messages.find(MESSAGE_LIMBS_ARE_STIFFENING) != string::npos || messages.find(MESSAGE_LIMBS_TURNED_TO_STONE) != string::npos) {
		/* bloody *trice, this is bad */
		/* TODO: eat [partly eaten] lizard corpse */
		/* pray if all else fails, don't even bother checking if it's safe to pray, we're dead anyways */
		World::setAction(static_cast<action::Action *>(new action::Pray(this, PRIORITY_HEALTH_CURE_DEADLY)));
	} else if (messages.find(MESSAGE_YOU_FEEL_CLUMSY) != string::npos || messages.find(MESSAGE_YOU_FEEL_FOOLISH) != string::npos ||messages.find(MESSAGE_YOU_FEEL_FRAGILE) != string::npos ||messages.find(MESSAGE_YOU_FEEL_REPULSIVE) != string::npos ||messages.find(MESSAGE_YOU_FEEL_STUPID) != string::npos ||messages.find(MESSAGE_YOU_FEEL_WEAK) != string::npos) {
		/* TODO: stat loss, apply unihorn */
	}
}
