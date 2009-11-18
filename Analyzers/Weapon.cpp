#include "Weapon.h"

#include "../EventBus.h"
#include "../Inventory.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/Wield.h"
#include "../Data/Weapon.h"
#include "../Events/Beatify.h"
#include "../Events/Event.h"
#include "../Events/ChangedInventoryItems.h"
#include "../Events/ReceivedItems.h"
#include "../Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Weapon::Weapon() : Analyzer("Weapon"), _wield_weapon(ILLEGAL_ITEM) {
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

			switch (Saiph::role()) {
			default:
				/* pick up every weapon for now */
				i->second.want(i->second.count());
				break;
			}
		}
	}
}

/* private methods */
void Weapon::setBestWeapon() {
	/* wield the best weapon we got */
	int best_damage = 0;
	for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		map<const string, const data::Weapon*>::const_iterator w = data::Weapon::weapons().find(i->second.name());
		if (w == data::Weapon::weapons().end())
			continue; // not a weapon
		if (!w->second->oneHanded() && Inventory::keyForSlot(SLOT_SHIELD) != ILLEGAL_ITEM)
			continue; // for now, don't try to wield two-hander when we got a shield
		int damage = 0;
		for (vector<data::Attack>::const_iterator a = w->second->attackSmall().begin(); a != w->second->attackSmall().end(); ++a)
			damage += a->avgDamage();
		for (vector<data::Attack>::const_iterator a = w->second->attackLarge().begin(); a != w->second->attackLarge().end(); ++a)
			damage += a->avgDamage();
		damage += i->second.enchantment() * 2;
		damage -= i->second.damage() * 2;
		if (damage > best_damage) {
			best_damage = damage;
			_wield_weapon = i->first;
		}
	}
}
