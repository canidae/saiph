#include "Analyzers/Health.h"

#include <stdlib.h>
#include "Saiph.h"
#include "World.h"
#include "Actions/Apply.h"
#include "Actions/Engrave.h"
#include "Actions/ListPlayerAttributes.h"
#include "Actions/Look.h"
#include "Actions/Pray.h"
#include "Actions/Eat.h"
#include "Actions/Rest.h"
#include "Data/UnicornHorn.h"
#include "EventBus.h"
#include "Events/Beatify.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ElberethQuery.h"
#include "Events/ReceivedItems.h"
#include "Events/WantItems.h"
#include "Data/Corpse.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Health::Health() : Analyzer("Health"), _resting(false), _was_polymorphed(false), _prev_str(INT_MAX), _prev_dex(INT_MAX), _prev_con(INT_MAX), _prev_int(INT_MAX), _prev_wis(INT_MAX), _prev_cha(INT_MAX), _lizard_key(ILLEGAL_ITEM), _unihorn_key(ILLEGAL_ITEM), _unihorn_use_turn(World::internalTurn()), _unihorn_priority(ILLEGAL_PRIORITY) {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
	World::queueAction(static_cast<action::Action*> (new action::ListPlayerAttributes(this)));
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
	if (Saiph::blind() || Saiph::confused() || Saiph::hallucinating() || Saiph::foodpoisoned() || Saiph::ill() || Saiph::stunned()) {
		if (canApplyUnihorn()) {
			/* apply unihorn */
			_unihorn_priority = (Saiph::foodpoisoned() || Saiph::ill()) ? PRIORITY_HEALTH_CURE_DEADLY : PRIORITY_HEALTH_CURE_NON_DEADLY;
		} else if (Saiph::foodpoisoned() || Saiph::ill()) {
			/* unable to use unihorn, need to try something else */
			/* TODO: eat eucalyptus leaf (if foodpoisoned or ill and no unihorn) */
			/* if we can't cure this in any other way, just pray even if it's not safe, because we'll die for sure if we don't */
			World::setAction(static_cast<action::Action*> (new action::Pray(this, PRIORITY_HEALTH_CURE_DEADLY)));
		}
	} else if (_resting) {
		/* [still] resting */
		if (Saiph::hitpoints() > Saiph::hitpointsMax() * 6 / 7) {
			/* enough hp (greater than about 86%) to continue our journey */
			_resting = false;
		} else if (Saiph::hitpoints() > Saiph::hitpointsMax() * 4 / 7 && Saiph::hitpointsMax() > 42) {
			/* if we don't see any monsters, then we won't rest for so long */
			bool monster_nearby = false;
			for (map<Point, Monster>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
				if (m->second.visible() || (abs(Saiph::position().row() - m->first.row()) < 5 && abs(Saiph::position().col() - m->first.col()) < 5)) {
					monster_nearby = true;
					break;
				}
			}
			_resting = monster_nearby;
		}
		if (_resting) {
			/* not enough hp and we want to rest, we should elbereth */
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
		if (action::Pray::isSafeToPrayMajorTrouble())
			World::setAction(static_cast<action::Action*> (new action::Pray(this, PRIORITY_HEALTH_CURE_LYCANTHROPY)));
	}
	if (Saiph::stoned()) {
		// cure stoning
		if (_lizard_key != ILLEGAL_ITEM) { // use lizard corpse if we have it
			World::setAction(static_cast<action::Action*> (new action::Eat(this, _lizard_key, PRIORITY_HEALTH_CURE_STONING)));
		} else {
			//we'll die if we don't pray, so let's pray
			World::setAction(static_cast<action::Action*> (new action::Pray(this, PRIORITY_HEALTH_CURE_STONING)));
		}
	}
	if (Saiph::polymorphed()) {
		/* cure polymorph */
		if (action::Pray::isSafeToPray())
			World::setAction(static_cast<action::Action*> (new action::Pray(this, PRIORITY_HEALTH_CURE_POLYMORPH)));
		/* also check player attributes */
		if (!_was_polymorphed)
			World::queueAction(static_cast<action::Action*> (new action::ListPlayerAttributes(this)));
		_was_polymorphed = true;
	}
	if (_was_polymorphed && !Saiph::polymorphed()) {
		/* returned to normal form, check player attributes again */
		World::queueAction(static_cast<action::Action*> (new action::ListPlayerAttributes(this)));
		_was_polymorphed = false;
	}
	if (_prev_str < Saiph::strength() || _prev_dex < Saiph::dexterity() || _prev_con < Saiph::constitution() || _prev_int < Saiph::intelligence() || _prev_wis < Saiph::wisdom() || _prev_cha < Saiph::charisma()) {
		/* we lost some stats, apply unihorn */
		if (canApplyUnihorn() && _unihorn_priority < PRIORITY_HEALTH_CURE_NON_DEADLY)
			_unihorn_priority = PRIORITY_HEALTH_CURE_NON_DEADLY;
	}

	/* apply unihorn until we've cured everything */
	if (_unihorn_priority >= 0 && canApplyUnihorn()) {
		/* unihorn failed last attempt, try again */
		World::setAction(static_cast<action::Action*> (new action::Apply(this, _unihorn_key, _unihorn_priority, false)));
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
	if (messages.find(UNIHORN_NOTHING_HAPPENS) != string::npos) {
		_unihorn_priority = -1;
		_unihorn_use_turn = World::internalTurn() + UNIHORN_UNIHORN_TIMEOUT;
	}
}

void Health::onEvent(event::Event * const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		for (set<unsigned char>::iterator k = e->keys().begin(); k != e->keys().end(); ++k) {
			map<unsigned char, Item>::iterator i = Inventory::items().find(*k);
			if (*k == _unihorn_key) {
				if (i == Inventory::items().end() || i->second.beatitude() == CURSED || data::UnicornHorn::unicornHorns().find(i->second.name()) == data::UnicornHorn::unicornHorns().end()) {
					_unihorn_key = ILLEGAL_ITEM;
					continue;
				}
			} else if (*k == _lizard_key) {
				map<const string, const data::Corpse*>::const_iterator c = data::Corpse::corpses().find(i->second.name());
				if (i == Inventory::items().end() || c == data::Corpse::corpses().end() || !(c->second->effects() & EAT_EFFECT_CURE_STONING)) {
					_lizard_key = ILLEGAL_ITEM;
					continue;
				}
			}
		}
	} else if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (data::UnicornHorn::unicornHorns().find(i->second.name()) != data::UnicornHorn::unicornHorns().end()) {
				if (i->second.beatitude() == BEATITUDE_UNKNOWN) {
					Beatify b(i->first, 175);
					EventBus::broadcast(&b);
				} else if (i->second.beatitude() != CURSED) {
					map<unsigned char, Item>::iterator u = Inventory::items().find(_unihorn_key);
					if (u == Inventory::items().end() || u->second.beatitude() == UNCURSED)
						_unihorn_key = i->first;
				}
				continue;
			}
			map<const string, const data::Corpse*>::const_iterator c = data::Corpse::corpses().find(i->second.name());
			if (c != data::Corpse::corpses().end() && (c->second->effects() & EAT_EFFECT_CURE_STONING)) {
				_lizard_key = i->first;
				continue;
			}
		}
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.beatitude() != CURSED && data::UnicornHorn::unicornHorns().find(i->second.name()) != data::UnicornHorn::unicornHorns().end()) {
				/* non-cursed unihorn */
				/* if we want to limit amount of unihorns, do it here, check inventory how many we got */
				int unihornNum = 0;
				for (map<unsigned char, Item>::iterator i2 = e->items().begin(); i2 != e->items().end(); ++i2) {
					if (i2->second.beatitude() != CURSED && data::UnicornHorn::unicornHorns().find(i->second.name()) != data::UnicornHorn::unicornHorns().end())
						unihornNum++;
				}
				/* keep at least 3 on hand */
				if (unihornNum < 3)
					i->second.want(i->second.count());
				continue;
			}
			map<const string, const data::Corpse*>::const_iterator c = data::Corpse::corpses().find(i->second.name());
			if (c != data::Corpse::corpses().end() && (c->second->effects() & EAT_EFFECT_CURE_STONING)) {
				/* lizard corpse */
				/* if we want to limit amount of lizard corpses, do it here, check inventory how many we got */
				int lizardNum = 0;
				for (map<unsigned char, Item>::iterator i2 = e->items().begin(); i2 != e->items().end(); ++i2) {
					map<const string, const data::Corpse*>::const_iterator c2 = data::Corpse::corpses().find(i2->second.name());
					if (c != data::Corpse::corpses().end() && (c->second->effects() & EAT_EFFECT_CURE_STONING))
						lizardNum++;
				}
				if (lizardNum < 3)
					i->second.want(i->second.count());
				continue;
			}
		}
	}
}

/* private methods */
bool Health::canApplyUnihorn() {
	return (_unihorn_key != ILLEGAL_ITEM && _unihorn_use_turn <= World::internalTurn() && Saiph::encumbrance() < OVERTAXED);
}
