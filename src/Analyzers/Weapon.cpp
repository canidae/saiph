#include "Analyzers/Weapon.h"

#include <map>
#include <string>

#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/TwoWeapon.h"
#include "Actions/SwapWeapon.h"
#include "Actions/Wield.h"
#include "Data/Weapon.h"
#include "Events/Beatify.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/Event.h"
#include "Events/ReceivedItems.h"
#include "Events/WantItems.h"
#include "Events/ChangedSkills.h"

#define WEAPON_USE_PRIMARY   0
#define WEAPON_USE_ALTERNATE 1
#define WEAPON_USE_MISSILE   2

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Weapon::Weapon() : Analyzer("Weapon"), _best_weapon(ILLEGAL_ITEM), _alt_weapon(ILLEGAL_ITEM), _potentially_best_weapon(ILLEGAL_ITEM), _best_missiles() {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
	EventBus::registerEvent(ChangedSkills::ID, this);
}

/* methods */
void Weapon::analyze() {
	if (Saiph::polymorphed() || _best_weapon == ILLEGAL_ITEM || Inventory::itemInSlot(SLOT_WEAPON).beatitude() == CURSED)
		return;

	map<string, const data::Weapon*>::const_iterator idata = data::Weapon::weapons().find(Inventory::itemAtKey(_best_weapon).name());

	bool two_weapons =
		_alt_weapon != ILLEGAL_ITEM &&  // need two weapons
		Saiph::maxSkill(P_TWO_WEAPON_COMBAT) >= P_BASIC && // and some possible skill
		idata != data::Weapon::weapons().end() && // and a proper weapon
		idata->second->oneHanded() && // one-handed weapon :)
		!Saiph::likesShields() && // not conflicting with Armor
		Saiph::skill(idata->second->type()) == Saiph::maxSkill(idata->second->type()) && // and maxxed skill
		Inventory::itemAtKey(_alt_weapon).beatitude() != BEATITUDE_UNKNOWN; // and BUCed secondary

	if (two_weapons) {
		unsigned char cur_weap = Inventory::keyForSlot(SLOT_WEAPON);
		unsigned char cur_a_weap = Inventory::keyForSlot(SLOT_ALTERNATE_WEAPON);
		unsigned char cur_o_weap = Inventory::keyForSlot(SLOT_OFFHAND_WEAPON);

		if (_best_weapon == cur_weap && _alt_weapon == cur_o_weap)
			;
		else if (_best_weapon == cur_weap && _alt_weapon == cur_a_weap)
			World::setAction(static_cast<action::Action*> (new action::TwoWeapon(this, WEAPON_WIELD_PRIORITY)));
		else if (_alt_weapon == cur_a_weap)
			World::setAction(static_cast<action::Action*> (new action::Wield(this, _best_weapon, WEAPON_WIELD_PRIORITY)));
		else if (_alt_weapon == cur_weap)
			World::setAction(static_cast<action::Action*> (new action::SwapWeapon(this, WEAPON_WIELD_PRIORITY)));
		else
			World::setAction(static_cast<action::Action*> (new action::Wield(this, _alt_weapon, WEAPON_WIELD_PRIORITY)));
	} else {
		if (_best_weapon != Inventory::keyForSlot(SLOT_WEAPON))
			World::setAction(static_cast<action::Action*> (new action::Wield(this, _best_weapon, WEAPON_WIELD_PRIORITY)));
		else if (Inventory::keyForSlot(SLOT_OFFHAND_WEAPON) != ILLEGAL_ITEM)
			World::setAction(static_cast<action::Action*> (new action::TwoWeapon(this, WEAPON_WIELD_PRIORITY)));
	}
}

void Weapon::onEvent(event::Event* const event) {
	if (event->id() == ChangedInventoryItems::ID || event->id() == ChangedSkills::ID || event->id() == ReceivedItems::ID) {
		setBestWeapons();
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (e->dropping()) {
				if (_best_missiles.find(i->first) != _best_missiles.end() || _best_weapon == i->first || _alt_weapon == i->first || _potentially_best_weapon == i->first) {
					/* don't drop weapons in one of our lists */
					i->second.want(i->second.count());
				}
			} else {
				/* looting, is this better than what we got? */
				if (betterThanWhatWeGot(i->second))
					i->second.want(i->second.count());
			}
		}
	}
}

/* private methods */
bool Weapon::betterThanWhatWeGot(const Item& item) {
	int pscore = calculateWeaponScore(item, WEAPON_USE_PRIMARY, true);
	int ascore = calculateWeaponScore(item, WEAPON_USE_ALTERNATE, true);
	int mscore = calculateWeaponScore(item, WEAPON_USE_MISSILE);

	if (pscore > 0 && (_best_weapon == ILLEGAL_ITEM || pscore > calculateWeaponScore(Inventory::itemAtKey(_best_weapon), WEAPON_USE_PRIMARY, true)))
		return true;
	if (ascore > 0 && (_alt_weapon == ILLEGAL_ITEM || ascore > calculateWeaponScore(Inventory::itemAtKey(_alt_weapon), WEAPON_USE_ALTERNATE, true)))
		return true;
	if (mscore > 0) {
		if (int(_best_missiles.size()) < WEAPON_COUNT_RANGE)
			return true;
		for (map<unsigned char, int>::iterator m = _best_missiles.begin(); m != _best_missiles.end(); ++m) {
			if (mscore > m->second)
				return true;
		}
	}

	return false;
}

int Weapon::calculateWeaponScore(const Item& item, int use, bool potential) {
	if (item.beatitude() == CURSED && use != WEAPON_USE_MISSILE)
		return 0;
	if (Saiph::alignment() == LAWFUL && item.name().find("poisoned") != string::npos)
		return 0; // ignore poisoned weapons if we're lawful
	map<const string, const data::Weapon*>::const_iterator w = data::Weapon::weapons().find(item.name());
	if (w == data::Weapon::weapons().end())
		return 0; // not a weapon
	const data::Weapon* weapon = w->second;
	// if we want a shield, ignore two-hander, also alternate weapons should be small (for #tw) or light (for backup)
	if (!weapon->oneHanded() && (use == WEAPON_USE_ALTERNATE || Saiph::likesShields()))
		return 0;
	// offhand cannot be an artifact
	if (use == WEAPON_USE_ALTERNATE && (weapon->properties() & PROPERTY_ARTIFACT))
		return 0;
	int type = weapon->type();
	// only some weapons throw well
	if (use == WEAPON_USE_MISSILE && !(type == P_DAGGER || type == -P_DART || type == P_JAVELIN || type == P_KNIFE || type == -P_SHURIKEN || type == P_SPEAR))
		return 0;
	// some weapons are only for throwing
	if (use != WEAPON_USE_MISSILE && type < 0)
		return 0;
	int score = 0;
	for (vector<data::Attack>::const_iterator a = weapon->attackSmall().begin(); a != weapon->attackSmall().end(); ++a)
		score += a->avgDamage();
	for (vector<data::Attack>::const_iterator a = weapon->attackLarge().begin(); a != weapon->attackLarge().end(); ++a)
		score += a->avgDamage();
	if (!weapon->oneHanded())
		score /= 2;
	score += item.enchantment() * 2;
	score -= item.damage() * 2;

	/* set role/skill specific score */
	/* an infrequently used weapon or a #twoweapon candidate won't advance, so use current skill */
	int skill = (use == WEAPON_USE_ALTERNATE) ? Saiph::skill(abs(weapon->type())) : Saiph::maxSkill(abs(weapon->type()));

	/* score += tohit + 2 * damage */
	switch (skill) {
	case P_BASIC:
		/* +hit = 0, +dam = 0 */
		break;

	case P_SKILLED:
		/* +hit = 2, +dam = 1 */
		score += 2 + 1 * 2;
		break;

	case P_EXPERT:
		/* +hit = 3, +dam = 2 */
		score += 3 + 2 * 2;
		break;

	default:
		/* unskilled */
		/* +hit = -4, +dam = -2 */
		score += -4 + -2 * 2;
		break;
	}

	// beatitude doesn't matter so much for missiles
	if (!potential && use != WEAPON_USE_MISSILE && item.beatitude() == BEATITUDE_UNKNOWN)
		score /= 2;
	if (use == WEAPON_USE_MISSILE && item.beatitude() == CURSED)
		score = score * 6 / 7;

	return score;
}

void Weapon::setBestWeapons() {
	/* scan inventory and cache the best weapons we have; WantItems and wield handling will use these */
	_potentially_best_weapon = _best_weapon = _alt_weapon = ILLEGAL_ITEM;
	int best_score = 0;
	int alt_score = 0;
	int potentially_best_score = 0;
	_best_missiles.clear();

	for (map<unsigned char, Item>::const_iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		int pscore = calculateWeaponScore(i->second, WEAPON_USE_PRIMARY);
		if (pscore > best_score) {
			best_score = pscore;
			_best_weapon = i->first;
		}
	}

	for (map<unsigned char, Item>::const_iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		if (i->first == _best_weapon)
			continue;
		int ascore = calculateWeaponScore(i->second, WEAPON_USE_ALTERNATE);
		if (ascore > alt_score) {
			alt_score = ascore;
			_alt_weapon = i->first;
		}
	}

	// carry up to 1 weapon which could be better than our primary or alternate, if it's not cursed
	if (_best_weapon != ILLEGAL_ITEM && _alt_weapon != ILLEGAL_ITEM)
		potentially_best_score = min(calculateWeaponScore(Inventory::itemAtKey(_best_weapon), WEAPON_USE_PRIMARY, true),
				calculateWeaponScore(Inventory::itemAtKey(_alt_weapon), WEAPON_USE_PRIMARY, true));

	for (map<unsigned char, Item>::const_iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		if (i->second.beatitude() != BEATITUDE_UNKNOWN)
			continue;
		int ptscore = calculateWeaponScore(i->second, WEAPON_USE_PRIMARY, true);
		if (ptscore > potentially_best_score) {
			potentially_best_score = ptscore;
			_potentially_best_weapon = i->first;
		}
	}

	multimap<int, unsigned char> mq;

	for (map<unsigned char, Item>::const_iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		if (i->first == _best_weapon || i->first == _alt_weapon)
			continue;
		int mscore = calculateWeaponScore(i->second, WEAPON_USE_MISSILE);
		if (mscore > 0)
			mq.insert(pair<int, unsigned char>(-mscore, i->first));
	}

	multimap<int, unsigned char>::iterator mqi = mq.begin();
	while (_best_missiles.size() < WEAPON_COUNT_RANGE && mqi != mq.end()) {
		_best_missiles[mqi->second] = -mqi->first;
		++mqi;
	}

	if (_potentially_best_weapon != ILLEGAL_ITEM) {
		Beatify b(_potentially_best_weapon, 175);
		EventBus::broadcast(&b);
	}

	Debug::custom(name()) << "Best melee weapon (" << best_score << ") is: " << Inventory::itemAtKey(_best_weapon) << endl;
	Debug::custom(name()) << "Best alternate weapon (" << alt_score << ") is: " << Inventory::itemAtKey(_alt_weapon) << endl;
	Debug::custom(name()) << "Best candidate weapon (" << potentially_best_score << ") is: " << Inventory::itemAtKey(_potentially_best_weapon) << endl;
	for (map<unsigned char, int>::const_iterator i = _best_missiles.begin(); i != _best_missiles.end(); ++i)
		Debug::custom(name()) << "High quality missile (" << i->second << ") is: " << Inventory::itemAtKey(i->first) << endl;
}
