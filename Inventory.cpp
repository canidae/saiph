#include "EventBus.h"
#include "Inventory.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ReceivedItems.h"

using namespace event;
using namespace std;

/* define static variables */
bool Inventory::updated = false;
map<unsigned char, Item> Inventory::items;

/* define private static variables */
vector<unsigned char> Inventory::changed_items;

/* methods */
void Inventory::analyze() {
}

void Inventory::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_NOT_CARRYING_ANYTHING) != string::npos || messages.find(MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD) != string::npos) {
		/* we're not carrying anything */
		Inventory::items.clear();
	} else if (World::menu && messages.find(" -  ") == string::npos) {
		/* listing a menu with " - " and it's not enhance menu (that got " -   "), probably listing inventory */
		string::size_type pos = 0;
		string::size_type pos2 = -1;
		if (World::cur_page == 1) {
			/* listing first page, set item count for every item to 0 */
			for (map<unsigned char, Item>::iterator i = items.begin(); i != items.end(); ++i)
				i->second.count = 0;
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
				changed_items.push_back(messages[pos - 1]);
			} else if (item != i->second) {
				/* item does not match item in inventory */
				removeItem(i->first, item);
				addItem(i->first, item);
				changed_items.push_back(i->first);
			}
		}
		if (World::cur_page == World::max_page) {
			/* listing last page, add items with count 0 to changed_items and remove them from inventory */
			for (map<unsigned char, Item>::iterator i = items.begin(); i != items.end(); ) {
				if (i->second.count == 0) {
					changed_items.push_back(i->first);
					items.erase(i++);
					continue;
				}
				++i;
			}
			/* also mark inventory as updated */
			updated = true;
		}
		if (changed_items.size() > 0) {
			/* send event ChangedInventoryItems */
			ChangedInventoryItems cii(changed_items);
			EventBus::broadcast((Event *) &cii);
			changed_items.clear();
		}
	} else if (!World::menu) {
		/* check if we received items */
		string::size_type pos = 0;
		string::size_type pos2 = -1;
		while ((pos = messages.find(" - ", pos2 + 4)) != string::npos && pos > 1 && messages[pos - 2] == ' ' && (pos2 = messages.find_first_of('.', pos + 3)) != string::npos && pos2 < messages.size() - 2 && messages[pos2 + 1] == ' ' && messages[pos2 + 2] == ' ') {
			/* add item to inventory */
			Item item(messages.substr(pos + 3, pos2 - pos - 3));
			if (item.count <= 0) {
				Debug::notice() << INVENTORY_DEBUG_NAME << "Failed parsing \"" << messages.substr(pos - 2, pos2 - pos + 2) << "\" as an item" << endl;
				continue;
			}
			addItem(messages[pos - 1], item);
			/* add item to changed_items */
			changed_items.push_back(messages[pos - 1]);
		}
		if (changed_items.size() > 0) {
			/* if we're standing on a stash, mark it as changed */
			map<Point, Stash>::iterator s = World::levels[Saiph::position.level].stashes.find(Saiph::position);
			if (s != World::levels[Saiph::position.level].stashes.end())
				s->second.turn_changed = World::turn;
			/* send event "ReceivedItems" */
			ReceivedItems ri(changed_items);
			EventBus::broadcast((Event *) &ri);
			changed_items.clear();
		}
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
}

void Inventory::removeItem(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	map<unsigned char, Item>::iterator i = items.find(key);
	if (i == items.end())
		return;
	Debug::notice() << INVENTORY_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from inventory slot " << key << endl;
	if (i->second.count > item.count)
		i->second.count -= item.count; // we got more than we remove
	else    
		items.erase(i); // removing all we got
}
