#include "Weapon.h"


#include "../EventBus.h"
#include "../Inventory.h"
#include "../Saiph.h"
#include "../World.h"
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
Weapon::Weapon() : Analyzer("Weapon"), _wield_weapon(false) {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Weapon::parseMessages(const string& messages) {
	if (last_polymorphed != Saiph::polymorphed()) {
		/* we polymorphed; assume we can wield unless determined otherwise */
		can_wield_weapon = true;
		last_polymorphed = Saiph::polymorphed();
	}

	if (!command2.empty() && messages.find(MESSAGE_DONT_BE_RIDICULOUS, 0) != string::npos) {
		/* we're polymorphed to a creature that can't wield weapons */
		can_wield_weapon = false;
		return;
	}

	if (World::question() && !command2.empty() && messages.find(WEAPON_WHAT_TO_WIELD, 0) != string::npos) {
		/* wield a weapon */
		command = command2;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
		_wield_weapon = false;
	} else if (!saiph->world->question && !saiph->world->menu && can_wield_weapon && (saiph->inventory_changed || _wield_weapon)) {
		wieldWeapon();
	}
}

void Weapon::onEvent(event::Event * const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		// TODO: check if we should change armor
	} else if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.beatitude() != BEATITUDE_UNKNOWN || data::Weapon::weapons().find(i->second.name()) != data::Weapon::weapons().end())
				continue; // known beatitude or not a weapon
			Beatify b(i->first, 100);
			EventBus::broadcast(&b);
		}
		// TODO: check if we should switch weapon
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (e->dropping() && (i->first == Inventory::keyForSlot(SLOT_WEAPON) || i->first == Inventory::keyForSlot(SLOT_ALTERNATE_WEAPON) || i->first == Inventory::keyForSlot(SLOT_OFFHAND_WEAPON)))
				i->second.want(i->second.count()); // main/alternate/offhand weapon, don't drop
			else
				wantWeapon(i->second);
		}
	}
}

/* private methods */
void Weapon::wantWeapon(const Item& item) {
	map<const string, const Weapon*>::iterator w = data::Weapon::weapons().find(item.name());
	if (w == data::Weapon::weapons().end())
		return; // not a weapon
}

void Weapon::wieldWeapon() {
	/* check that we're (still) wielding our preferred weapon */
	unsigned char wielded = 0;
	unsigned char best_key = 0;
	int best_priority = INT_MIN;
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.additional.find("weapon in ", 0) == 0 || i->second.additional == "wielded") {
			wielded = i->first;
			if (i->second.beatitude == CURSED) {
				/* weapon is cursed, we can't wield another weapon */
				best_key = 0;
				break;
			}
		}
		for (vector<WeaponData>::iterator w = weapons.begin(); w != weapons.end(); ++w) {
			if (w->name != i->second.name) {
				continue;
			} else if (i->second.beatitude == BEATITUDE_UNKNOWN &&
				wielded != i->first) {
				/* weapon with unknown beatitude, request it beatified */
				req.request = REQUEST_BEATIFY_ITEMS;
				saiph->request(req);
			}
			if ((w->beatitude & i->second.beatitude) == 0)
				continue;
			else if (w->priority + i->second.enchantment - i->second.damage <= best_priority)
				continue;
			best_key = i->first;
			best_priority = w->priority + i->second.enchantment - i->second.damage;
		}
	}
	if (best_key == 0 || (wielded == best_key)) {
		/* wielding best weapon or got no weapon to wield */
		command.clear();
		command2.clear();
		priority = ILLEGAL_PRIORITY;
		_wield_weapon = false;
		return;
	}
	_wield_weapon = true;
	command = WIELD;
	command2 = best_key;
	priority = PRIORITY_WEAPON_WIELD;

	/* tell Loot to drop unwanted weapons */
	req.request = REQUEST_ITEM_PICKUP;
	req.only_unknown_enchantment = false;
	for (vector<WeaponData>::iterator w = weapons.begin(); w != weapons.end(); ++w) {
		req.beatitude = w->beatitude | BEATITUDE_UNKNOWN;
		req.data = w->name;
		if (w->keep || w->priority + 7 >= best_priority) {
			/* we [still] want this weapon.
			 * in case we lost a good weapon and now wield a less good
			 * weapon we'll need to tell Loot to pick up this weapon */
			req.value = w->amount;
		} else {
			/* we don't want to keep this weapon and it'll never
			 * be better than the weapon we currently got */
			req.value = 0;
		}
		saiph->request(req);
	}
}
