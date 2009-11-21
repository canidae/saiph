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
		World::setAction(static_cast<action::Action*> (new action::Wield(this, _wield_weapon, WEAPON_WIELD_PRIORITY)));
}

void Weapon::onEvent(event::Event * const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		setBestWeapon();
	} else if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.beatitude() != BEATITUDE_UNKNOWN || data::Weapon::weapons().find(i->second.name()) == data::Weapon::weapons().end())
				continue; // known beatitude or not a weapon
			if (_melee_weapons.find(i->first) == _melee_weapons.end())
				continue; // it's not in our _melee_weapons list, not gonna wield it
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
			if (w->second->type() == WEAPON_PICKAXE)
				continue; // causing issues with shop, ignore them for now
			if ((w->second->properties() & PROPERTY_ARTIFACT) != 0) {
				/* ooh, artifact weapon, we'll want this */
				i->second.want(i->second.count());
				continue;
			}
			if (!e->dropping() && Saiph::encumbrance() >= BURDENED)
				continue; // don't loot weapons while burdened, but don't drop them either (unless we later figure out we don't want them)
			if (i->second.beatitude() == CURSED)
				continue; // ignore cursed weapons
			if (Saiph::alignment() == LAWFUL && i->second.name().find("poisoned") != string::npos)
				continue; // ignore poisoned weapons if we're lawful

			int score = calculateWeaponScore(i->second, w->second);
			if (score <= 0)
				continue; // don't want this weapon

			int melee_weapons = 2;
			int range_weapons = 5; // stacks, not count
			int better_weapon_count = 0;
			if (w->second->type() == WEAPON_DAGGER || w->second->type() == WEAPON_DART || w->second->type() == WEAPON_JAVELIN || w->second->type() == WEAPON_KNIFE || w->second->type() == WEAPON_SHURIKEN || w->second->type() == WEAPON_SPEAR) {
				for (map<unsigned char, int>::iterator m = _range_weapons.begin(); m != _range_weapons.end(); ++m) {
					if (m->second >= score)
						++better_weapon_count;
				}
				if (better_weapon_count > melee_weapons)
					continue; // got better ranged weapons already
				_range_weapons[i->first] = score;
			} else {
				for (map<unsigned char, int>::iterator m = _melee_weapons.begin(); m != _melee_weapons.end(); ++m) {
					if (m->second >= score)
						++better_weapon_count;
				}
				if (better_weapon_count > range_weapons)
					continue; // got better melee weapons already
				_melee_weapons[i->first] = score;
			}
			/* pick up every weapon for now */
			i->second.want(i->second.count());
		}
	}
}

/* private methods */
int Weapon::calculateWeaponScore(const Item& item, const data::Weapon* weapon) {
	if (!weapon->oneHanded() && Inventory::keyForSlot(SLOT_SHIELD) != ILLEGAL_ITEM)
		return 0; // for now, don't try to wield two-hander when we got a shield
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
	int skill = 0;
	switch (Saiph::role()) {
	case BARBARIAN:
		switch (weapon->type()) {
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
		switch (weapon->type()) {
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
	if (item.beatitude() == CURSED)
		score /= 4;
	else if (item.beatitude() == BEATITUDE_UNKNOWN)
		score /= 2;
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
		int score = calculateWeaponScore(i->second, w->second);
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
