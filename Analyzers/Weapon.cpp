#include "Weapon.h"

#include "../EventBus.h"
#include "../Inventory.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/Wield.h"
#include "../Data/Weapon.h"
#include "../Events/Beatify.h"
#include "../Events/ChangedInventoryItems.h"
#include "../Events/Event.h"
#include "../Events/ReceivedItems.h"
#include "../Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Weapon::Weapon() : Analyzer("Weapon"), _wield_weapon(ILLEGAL_ITEM), _melee_weapons(), _range_weapons() {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Weapon::analyze() {
	if (!Saiph::polymorphed() && _wield_weapon != ILLEGAL_ITEM && _wield_weapon != Inventory::keyForSlot(SLOT_WEAPON))
		World::setAction(static_cast<action::Action*> (new action::Wield(this, _wield_weapon, 200)));
}

void Weapon::onEvent(event::Event * const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		setBestWeapon();
	} else if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.beatitude() != BEATITUDE_UNKNOWN || data::Weapon::weapons().find(i->second.name()) == data::Weapon::weapons().end())
				continue; // known beatitude or not a weapon
			Beatify b(i->first, 100);
			EventBus::broadcast(&b);
		}
		setBestWeapon();
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (e->dropping() && (i->first == Inventory::keyForSlot(SLOT_WEAPON) || i->first == Inventory::keyForSlot(SLOT_ALTERNATE_WEAPON) || i->first == Inventory::keyForSlot(SLOT_OFFHAND_WEAPON))) {
				/* don't drop main/alternate/offhand weapon */
				i->second.want(i->second.count());
				continue;
			}
			map<const string, const data::Weapon*>::const_iterator w = data::Weapon::weapons().find(i->second.name());
			if (w == data::Weapon::weapons().end())
				continue; // not a weapon
			if ((w->second->properties() & PROPERTY_ARTIFACT) != 0) {
				/* ooh, artifact weapon, we'll want this */
				i->second.want(i->second.count());
				continue;
			}
			if (Saiph::encumbrance() >= BURDENED)
				continue; // only loot artifacts while burdened
			if (i->second.beatitude() == CURSED)
				continue; // ignore cursed weapons
			if (Saiph::alignment() == LAWFUL && i->second.name().find("poisoned") != string::npos)
				continue; // ignore poisoned weapons if we're lawful

			int melee_weapons = 2;
			int range_weapons = 5; // stacks, not count
			int better_weapon_count = 0;
			int weapon_score = calculateWeaponScore(i->second);
			if (weapon_score <= 0)
				continue; // don't want this weapon
			if (w->second->type() == WEAPON_DAGGER || w->second->type() == WEAPON_DART || w->second->type() == WEAPON_JAVELIN || w->second->type() == WEAPON_KNIFE || w->second->type() == WEAPON_SHURIKEN || w->second->type() == WEAPON_SPEAR) {
				for (map<unsigned char, int>::iterator m = _range_weapons.begin(); m != _range_weapons.end(); ++m) {
					if (m->second >= weapon_score)
						++better_weapon_count;
				}
				if (better_weapon_count > melee_weapons)
					continue; // got better ranged weapons already
			} else {
				for (map<unsigned char, int>::iterator m = _melee_weapons.begin(); m != _melee_weapons.end(); ++m) {
					if (m->second >= weapon_score)
						++better_weapon_count;
				}
				if (better_weapon_count > range_weapons)
					continue; // got better melee weapons already
			}
			/* pick up every weapon for now */
			i->second.want(i->second.count());
		}
	}
}

/* private methods */
int Weapon::calculateWeaponScore(const Item& item) {
	map<const string, const data::Weapon*>::const_iterator w = data::Weapon::weapons().find(item.name());
	if (w == data::Weapon::weapons().end())
		return 0; // not a weapon
	if (!w->second->oneHanded() && Inventory::keyForSlot(SLOT_SHIELD) != ILLEGAL_ITEM)
		return 0; // for now, don't try to wield two-hander when we got a shield
	int score = 0;
	for (vector<data::Attack>::const_iterator a = w->second->attackSmall().begin(); a != w->second->attackSmall().end(); ++a)
		score += a->avgDamage();
	for (vector<data::Attack>::const_iterator a = w->second->attackLarge().begin(); a != w->second->attackLarge().end(); ++a)
		score += a->avgDamage();
	score += item.enchantment() * 2;
	score -= item.damage() * 2;

	/* set role/skill specific score */
	/* TODO: replace with data from #enhance (big task, though) */
	int skill = 0;
	switch (Saiph::role()) {
	case BARBARIAN:
		switch (w->second->type()) {
		case WEAPON_DAGGER:
		case WEAPON_SABER:
		case WEAPON_FLAIL:
		case WEAPON_QUARTERSTAFF:
		case WEAPON_BOW:
			/* basic */
			skill = 1;
			break;

		case WEAPON_PICKAXE:
		case WEAPON_BROADSWORD:
		case WEAPON_LONGSWORD:
		case WEAPON_SCIMITAR:
		case WEAPON_CLUB:
		case WEAPON_MACE:
		case WEAPON_MORNINGSTAR:
		case WEAPON_SPEAR:
		case WEAPON_TRIDENT:
			/* skilled */
			skill = 2;
			break;

		case WEAPON_AXE:
		case WEAPON_SHORTSWORD:
		case WEAPON_TWOHANDEDSWORD:
		case WEAPON_HAMMER:
			/* expert */
			skill = 3;
			break;

		default:
			/* unskilled */
			skill = 0;
			break;
		}
		break;

	case VALKYRIE:
		switch (w->second->type()) {
		case WEAPON_SCIMITAR:
		case WEAPON_SABER:
		case WEAPON_QUARTERSTAFF:
		case WEAPON_JAVELIN:
		case WEAPON_TRIDENT:
		case WEAPON_SLING:
			/* basic */
			skill = 1;
			break;

		case WEAPON_PICKAXE:
		case WEAPON_SHORTSWORD:
		case WEAPON_BROADSWORD:
		case WEAPON_POLEARM:
		case WEAPON_SPEAR:
		case WEAPON_LANCE:
			/* skilled */
			skill = 2;
			break;

		case WEAPON_DAGGER:
		case WEAPON_AXE:
		case WEAPON_LONGSWORD:
		case WEAPON_TWOHANDEDSWORD:
		case WEAPON_HAMMER:
			/* expert */
			skill = 3;
			break;

		default:
			/* unskilled */
			skill = 0;
			break;
		}
		break;

	default:
		skill = 0;
		break;
	}
	/* score += tohit + 2 * damage */
	switch (skill) {
	case 1:
		/* basic */
		/* +hit = 0, +dam = 0 */
		break;

	case 2:
		/* skilled */
		/* +hit = 2, +dam = 1 */
		score += 2 + 1 * 2;
		break;

	case 3:
		/* expert */
		/* +hit = 3, +dam = 2 */
		score += 3 + 2 * 2;
		break;

	default:
		/* unskilled */
		/* +hit = -4, +dam = -2 */
		score += -4 + -2 * 2;
		break;
	}
	return score;
}

void Weapon::setBestWeapon() {
	/* wield the best weapon we got */
	int best_score = 0;
	_melee_weapons.clear();
	_range_weapons.clear();
	for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		map<const string, const data::Weapon*>::const_iterator w = data::Weapon::weapons().find(i->second.name());
		if (w == data::Weapon::weapons().end())
			continue; // not a weapon
		if (!w->second->oneHanded() && Inventory::keyForSlot(SLOT_SHIELD) != ILLEGAL_ITEM)
			continue; // for now, don't try to wield two-hander when we got a shield
		int score = calculateWeaponScore(i->second);
		if (score > best_score) {
			best_score = score;
			_wield_weapon = i->first;
		}
		if (w->second->type() == WEAPON_DAGGER || w->second->type() == WEAPON_DART || w->second->type() == WEAPON_JAVELIN || w->second->type() == WEAPON_KNIFE || w->second->type() == WEAPON_SHURIKEN || w->second->type() == WEAPON_SPEAR)
			_range_weapons[i->first] = score;
		else
			_melee_weapons[i->first] = score;
	}
}
