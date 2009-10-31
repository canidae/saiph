#include "Health.h"

#include <stdlib.h>
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
Health::Health() : Analyzer("Health"), _resting(false), _prev_str(INT_MAX), _prev_dex(INT_MAX), _prev_con(INT_MAX), _prev_int(INT_MAX), _prev_wis(INT_MAX), _prev_cha(INT_MAX) {
}

/* methods */
void Health::analyze() {
	/* do something if our health is low */
	if (Saiph::hitpoints() > 0 && Saiph::hitpoints() < Saiph::hitpointsMax() * 4 / 7) {
		/* hp below 4/7 (about 57%), do something! */
		bool doing_something = false; // TODO: we probably don't need this variable later
		if (Saiph::hitpoints() < Saiph::hitpointsMax() * 2 / 7) {
			/* TODO: try quaffing healing potion */
			//req.priority = PRIORITY_HEALTH_QUAFF_FOR_HP;
			/* TODO: if we're quaffing, set doing_something = true */
			if (Saiph::hitpoints() < 6 || Saiph::hitpoints() < Saiph::hitpointsMax() / 7) {
				/* TODO: should not enter this block if we could quaff */
				/* quaffing won't work... how about pray? */
				if (action::Pray::isSafeToPray()) {
					World::setAction(static_cast<action::Action*> (new action::Pray(this, PRIORITY_HEALTH_PRAY_FOR_HP)));
					doing_something = true;
				}
			}
		}
		if (!doing_something) {
			/* we're not going to quaff/pray, set resting = true to make her elbereth/rest instead */
			_resting = true;
		}
	}
	if (Saiph::confused() || Saiph::hallucinating() || Saiph::foodpoisoned() || Saiph::ill() || Saiph::stunned()) {
		/* TODO: apply unihorn */
		//req.priority = (Saiph::foodpoisoned || Saiph::ill) ? PRIORITY_HEALTH_CURE_DEADLY : PRIORITY_HEALTH_CURE_NON_DEADLY;
		_resting = false; // this is to prevent us from trying to elbereth when we got some bad effect
		if (Saiph::foodpoisoned() || Saiph::ill()) {
			/* TODO: should not enter this block if we can use unihorn */
			/* TODO: eat eucalyptus leaf (if foodpoisoned or ill and no unihorn) */
			/* if we can't cure this in any other way, just pray even if it's not safe, because we'll die for sure if we don't */
			World::setAction(static_cast<action::Action*> (new action::Pray(this, PRIORITY_HEALTH_CURE_DEADLY)));
		}
	}
	if (_resting) {
		/* still resting */
		if (Saiph::hitpoints() > Saiph::hitpointsMax() * 4 / 7 && Saiph::hitpointsMax() > 42) {
			/* if we don't see any monsters, then we won't rest for so long */
			bool monster_nearby = false;
			for (map<Point, Monster>::iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
				if (m->second.visible() || (abs(Saiph::position().row() - m->first.row()) < 5 && abs(Saiph::position().col() - m->first.col()) < 5)) {
					monster_nearby = true;
					break;
				}
			}
			_resting = monster_nearby;
		}
		if (Saiph::hitpoints() > Saiph::hitpointsMax() * 6 / 7) {
			_resting = false; // enough hp (greater than about 86%) to continue our journey
		} else if (!Saiph::blind() && !Saiph::confused() && !Saiph::stunned() && !Saiph::hallucinating()) {
			ElberethQuery eq;
			EventBus::broadcast(static_cast<Event*> (&eq));
			if (eq.type() == ELBERETH_MUST_CHECK) {
				/* we don't know, we must look */
				World::setAction(static_cast<action::Action*> (new action::Look(this)));
			} else if (eq.type() == ELBERETH_DUSTED || eq.type() == ELBERETH_NONE) {
				/* no elbereth or dusted elbereth, engrave or rest, depending on amount of elbereths */
				if (eq.count() < 3)
					World::setAction(static_cast<action::Action*> (new action::Engrave(this, ELBERETH "\n", HANDS, PRIORITY_HEALTH_REST_FOR_HP_HIGH, (eq.count() > 0))));
				else
					World::setAction(static_cast<action::Action*> (new action::Rest(this, PRIORITY_HEALTH_REST_FOR_HP_HIGH)));
			}
			/* TODO: handle digged/burned elbereth */
		}
	}
	if (Saiph::intrinsics() & PROPERTY_LYCANTHROPY) {
		/* cure lycanthropy */
		/* TODO: eat sprig of wolfsbane */
		/* no? try praying instead */
		if (action::Pray::isSafeToPray())
			World::setAction(static_cast<action::Action*> (new action::Pray(this, PRIORITY_HEALTH_CURE_LYCANTHROPY)));
	}
	if (Saiph::polymorphed()) {
		/* cure polymorph */
		if (action::Pray::isSafeToPray())
			World::setAction(static_cast<action::Action*> (new action::Pray(this, PRIORITY_HEALTH_CURE_POLYMORPH)));
	}
	if (_prev_str < Saiph::strength() || _prev_dex < Saiph::dexterity() || _prev_con < Saiph::constitution() || _prev_int < Saiph::intelligence() || _prev_wis < Saiph::wisdom() || _prev_cha < Saiph::charisma()) {
		/* TODO: we lost some stats. apply unihorn */
	}

	/* set previous stat values */
	_prev_str = Saiph::strength();
	_prev_dex = Saiph::dexterity();
	_prev_con = Saiph::constitution();
	_prev_int = Saiph::intelligence();
	_prev_wis = Saiph::wisdom();
	_prev_cha = Saiph::charisma();
}

void Health::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_SLOWING_DOWN) != string::npos || messages.find(MESSAGE_LIMBS_ARE_STIFFENING) != string::npos || messages.find(MESSAGE_LIMBS_TURNED_TO_STONE) != string::npos) {
		/* bloody *trice, this is bad */
		/* TODO: eat [partly eaten] lizard corpse */
		/* pray if all else fails, don't even bother checking if it's safe to pray, we're dead anyways */
		World::setAction(static_cast<action::Action*> (new action::Pray(this, PRIORITY_HEALTH_CURE_DEADLY)));
	}
}
