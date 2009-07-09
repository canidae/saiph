#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "World.h"
#include "Data/Armor.h"
#include "Data/Amulet.h"

using namespace event;
using namespace std;

/* define static variables */
bool Inventory::updated = false;
map<unsigned char, Item> Inventory::items;
unsigned char Inventory::slots[] = {'\0'};

/* define private static variables */
ChangedInventoryItems Inventory::changed;
set<unsigned char> Inventory::lost;

/* methods */
void Inventory::analyze() {
}

void Inventory::parseMessages(const string &messages) {
	if (World::menu && messages.find(" - ") != string::npos && messages.find(" -  ") == string::npos) {
		/* listing a menu with " - " and it's not enhance menu (that got " -   "), probably listing inventory */
		string::size_type pos = 0;
		string::size_type pos2 = -1;
		if (World::cur_page == 1) {
			/* listing first page, clear changed and lost and add every inventory item to "lost" */
			changed.keys.clear();
			lost.clear();
			for (map<unsigned char, Item>::iterator i = items.begin(); i != items.end(); ++i)
				lost.insert(i->first);
		}
		while ((pos = messages.find(" - ")) != string::npos && pos > 2 && messages[pos - 3] == ' ' && messages[pos - 2] == ' ' && (pos2 = messages.find("  ", pos + 3)) != string::npos) {
			/* check that item match inventory item */
			Item item(messages.substr(pos + 3, pos2 - pos - 3));
			if (item.count <= 0) {
				Debug::notice() << INVENTORY_DEBUG_NAME << "Failed parsing \"" << messages.substr(pos - 2, pos2 - pos + 2) << "\" as an item" << endl;
				continue;
			}
			map<unsigned char, Item>::iterator i = items.find(messages[pos - 1]);
			if (i == items.end()) {
				/* item is not in our inventory */
				addItem(messages[pos - 1], item);
				changed.keys.insert(messages[pos - 1]);
			} else if (item != i->second) {
				/* item does not match item in inventory */
				removeItem(i->first, item);
				addItem(i->first, item);
				changed.keys.insert(i->first);
			}
			/* we (still) got this item, so it's not lost. remove it from lost */
			lost.erase(messages[pos - 1]);
		}
		if (World::cur_page == World::max_page) {
			/* listing last page, add lost items to changed and remove them from inventory */
			for (set<unsigned char>::iterator l = lost.begin(); l != lost.end(); ++l) {
				changed.keys.insert(*l);
				items.erase(*l);
			}
			/* mark inventory as updated */
			updated = true;
		}
		if (changed.keys.size() > 0) {
			/* broadcast ChangedInventoryItems */
			EventBus::broadcast(static_cast<Event *>(&changed));
		}
	} else if (messages.find(MESSAGE_NOT_CARRYING_ANYTHING) != string::npos || messages.find(MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD) != string::npos) {
		/* we're not carrying anything */
		items.clear();
		updated = true;
	} else if (messages.find(MESSAGE_STEALS) != string::npos || messages.find(MESSAGE_STOLE) != string::npos || messages.find(MESSAGE_DESTROY_POTION_FIRE, 0) != string::npos || messages.find(MESSAGE_DESTROY_POTION_FIRE2, 0) != string::npos || messages.find(MESSAGE_DESTROY_POTION_COLD, 0) != string::npos || messages.find(MESSAGE_DESTROY_POTION_COLD2, 0) != string::npos || messages.find(MESSAGE_DESTROY_RING, 0) != string::npos || messages.find(MESSAGE_DESTROY_RING2, 0) != string::npos || messages.find(MESSAGE_DESTROY_WAND, 0) != string::npos || messages.find(MESSAGE_DESTROY_WAND2, 0) != string::npos || messages.find(SAIPH_POLYMORPH, 0) != string::npos) {
		/* we got robbed, some of our stuff was destroyed or we polymorphed.
		 * mark inventory as not updated */
		updated = false;
	}
}

void Inventory::addItem(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	Debug::notice() << INVENTORY_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to inventory slot " << key << endl;
	if (items.find(key) != items.end()) {
		/* existing item, add amount */
		items[key].count += item.count;
	} else {
		/* new item */
		items[key] = item;
	}
	/* update slot */
	setSlot(key, item);
}

void Inventory::removeItem(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	map<unsigned char, Item>::iterator i = items.find(key);
	if (i == items.end())
		return;
	Debug::notice() << INVENTORY_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from inventory slot " << key << endl;
	if (i->second.count > item.count) {
		/* reduce stack */
		i->second.count -= item.count;
	} else {
		/* remove stack entirely */
		for (int a = 0; a < SLOTS; ++a) {
			if (slots[a] == key) {
				slots[a] = '\0';
				return;
			}
		}
		items.erase(i);
	}
}

/* private methods */
void Inventory::setSlot(unsigned char key, const Item &item) {
	if (item.additional == "being worn") {
		/* armor */
		map<string, data::Armor *>::iterator a = data::Armor::armors.find(item.name);
		if (a != data::Armor::armors.end()) {
			slots[a->second->slot] = key;
			return;
		}
		/* amulet */
		map<string, data::Amulet *>::iterator b = data::Amulet::amulets.find(item.name);
		if (b != data::Amulet::amulets.end()) {
			slots[SLOT_AMULET] = key;
			return;
		}
	} else if (item.additional == "wielded") {
		slots[SLOT_WEAPON] = key;
	} else if (item.additional.find("weapon in ") == 0) {
		slots[SLOT_WEAPON] = key;
	} else if (item.additional.find("wielded in other ") == 0) {
		slots[SLOT_OFFHAND_WEAPON] = key;
	} else if (item.additional.find("on left ") == 0) {
		slots[SLOT_LEFT_RING] = key;
	} else if (item.additional.find("on right ") == 0) {
		slots[SLOT_RIGHT_RING] = key;
	}
}
