#include "Health.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/Engrave.h"
#include "../Actions/Look.h"
#include "../Actions/Pray.h"
#include "../Actions/Rest.h"
#include "../EventBus.h"
#include "../Events/ElberethQuery.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Health::Health() : Analyzer("Health"), resting(false), prev_str(INT_MAX), prev_dex(INT_MAX), prev_con(INT_MAX), prev_int(INT_MAX), prev_wis(INT_MAX), prev_cha(INT_MAX) {
}

/* methods */
void Health::analyze() {
	/* do something if our health is low */
	if (Saiph::hitpoints > 0 && Saiph::hitpoints < Saiph::hitpoints_max * 4 / 7) {
		/* hp below 4/7 (about 57%), do something! */
		bool doing_something = false; // TODO: we probably don't need this variable later
		if (Saiph::hitpoints < Saiph::hitpoints_max * 2 / 7) {
			/* TODO: try quaffing healing potion */
			//req.priority = PRIORITY_HEALTH_QUAFF_FOR_HP;
			/* TODO: if we're quaffing, set doing_something = true */
			if (Saiph::hitpoints < 6 || Saiph::hitpoints < Saiph::hitpoints_max / 7) {
				/* TODO: should not enter this block if we could quaff */
				/* quaffing won't work... how about pray? */
				if (action::Pray::isSafeToPray()) {
					World::setAction(static_cast<action::Action *>(new action::Pray(this, PRIORITY_HEALTH_PRAY_FOR_HP)));
					doing_something = true;
				}
			}
		}
		if (!doing_something) {
			/* we're not going to quaff/pray, set resting = true to make her elbereth/rest instead */
			resting = true;
		}
	}
	if (Saiph::confused || Saiph::hallucinating || Saiph::foodpoisoned || Saiph::ill || Saiph::stunned) {
		/* TODO: apply unihorn */
		//req.priority = (Saiph::foodpoisoned || Saiph::ill) ? PRIORITY_HEALTH_CURE_DEADLY : PRIORITY_HEALTH_CURE_NON_DEADLY;
		resting = false; // this is to prevent us from trying to elbereth when we got some bad effect
		if (Saiph::foodpoisoned || Saiph::ill) {
			/* TODO: should not enter this block if we can use unihorn */
			/* TODO: eat eucalyptus leaf (if foodpoisoned or ill and no unihorn) */
			/* if we can't cure this in any other way, just pray even if it's not safe, because we'll die for sure if we don't */
			World::setAction(static_cast<action::Action *>(new action::Pray(this, PRIORITY_HEALTH_CURE_DEADLY)));
		}
	}
	if (resting) {
		/* still resting */
		if (Saiph::hitpoints > Saiph::hitpoints_max * 6 / 7) {
			resting = false; // enough hp (greater than about 86%) to continue our journey
		} else {
			ElberethQuery eq;
			EventBus::broadcast(static_cast<Event *>(&eq));
			if (eq.engraving_type == ELBERETH_MUST_CHECK) {
				/* we don't know, we must look */
				World::setAction(static_cast<action::Action *>(new action::Look(this)));
			} else if (eq.engraving_type == ELBERETH_DUSTED || eq.engraving_type == ELBERETH_NONE) {
				/* no elbereth or dusted elbereth, engrave or rest, depending on amount of elbereths */
				if (eq.number_of_elbereths < 3)
					World::setAction(static_cast<action::Action *>(new action::Engrave(this, ELBERETH "\n", HANDS, PRIORITY_HEALTH_REST_FOR_HP_HIGH, (eq.number_of_elbereths > 0))));
				else
					World::setAction(static_cast<action::Action *>(new action::Rest(this, PRIORITY_HEALTH_REST_FOR_HP_HIGH)));
			}
			/* TODO: handle digged/burned elbereth */
		}
	}
	if (Saiph::intrinsics & PROPERTY_LYCANTHROPY) {
		/* cure lycanthropy */
		/* TODO: eat sprig of wolfsbane */
		/* no? try praying instead */
		if (action::Pray::isSafeToPray())
			World::setAction(static_cast<action::Action *>(new action::Pray(this, PRIORITY_HEALTH_CURE_LYCANTHROPY)));
	}
	if (Saiph::polymorphed) {
		/* cure polymorph */
		if (action::Pray::isSafeToPray())
			World::setAction(static_cast<action::Action *>(new action::Pray(this, PRIORITY_HEALTH_CURE_POLYMORPH)));
	}
	if (prev_str < Saiph::strength || prev_dex < Saiph::dexterity || prev_con < Saiph::constitution || prev_int < Saiph::intelligence || prev_wis < Saiph::wisdom || prev_cha < Saiph::charisma) {
		/* TODO: we lost some stats. apply unihorn */
	}
	/* set previous stat values */
	prev_str = Saiph::strength;
	prev_dex = Saiph::dexterity;
	prev_con = Saiph::constitution;
	prev_int = Saiph::intelligence;
	prev_wis = Saiph::wisdom;
	prev_cha = Saiph::charisma;
}

void Health::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_SLOWING_DOWN) != string::npos || messages.find(MESSAGE_LIMBS_ARE_STIFFENING) != string::npos || messages.find(MESSAGE_LIMBS_TURNED_TO_STONE) != string::npos) {
		/* bloody *trice, this is bad */
		/* TODO: eat [partly eaten] lizard corpse */
		/* pray if all else fails, don't even bother checking if it's safe to pray, we're dead anyways */
		World::setAction(static_cast<action::Action *>(new action::Pray(this, PRIORITY_HEALTH_CURE_DEADLY)));
	}
}
