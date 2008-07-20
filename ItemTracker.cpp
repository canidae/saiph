#include "ItemTracker.h"

/* constructors */
ItemTracker::ItemTracker(Saiph *saiph) : saiph(saiph) {
	/* set which items we track */
	for (int a = 0; a <= UCHAR_MAX; ++a)
		item[a] = false;
	item[(unsigned char) WEAPON] = true;
	item[(unsigned char) ARMOR] = true;
	item[(unsigned char) RING] = true;
	item[(unsigned char) AMULET] = true;
	item[(unsigned char) TOOL] = true;
	item[(unsigned char) FOOD] = true;
	item[(unsigned char) POTION] = true;
	item[(unsigned char) SCROLL] = true;
	item[(unsigned char) SPELLBOOK] = true;
	item[(unsigned char) WAND] = true;
	item[(unsigned char) GOLD] = true;
	item[(unsigned char) GEM] = true;
	item[(unsigned char) STATUE] = true;
	// skipping boulder as that's a special item
	item[(unsigned char) IRON_BALL] = true;
	item[(unsigned char) CHAINS] = true;
	item[(unsigned char) VENOM] = true;
}

/* methods */
void ItemTracker::parseMessages(const string &messages) {
	/* figure out if there's something on the ground or if we're picking up something */
	/* always clear pickup list */
	pickup.clear();
	string::size_type pos;
	if ((pos = messages.find(MESSAGE_YOU_SEE_HERE, 0)) != string::npos) {
		/* single item on ground */
		clearStash(saiph->position);
		pos += sizeof (MESSAGE_YOU_SEE_HERE) - 1;
		string::size_type length = messages.find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			addItemToStash(saiph->position, parseItemText(messages.substr(pos, length)));
		}
	} else if ((pos = messages.find(MESSAGE_THINGS_THAT_ARE_HERE, 0)) != string::npos) {
		/* multiple items on ground */
		clearStash(saiph->position);
		pos = messages.find("  ", pos + 1);
		while (pos != string::npos && messages.size() > pos + 2) {
			pos += 2;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			addItemToStash(saiph->position, parseItemText(messages.substr(pos, length)));
			pos += length;
		}
	} else if ((pos = messages.find(MESSAGE_YOU_SEE_NO_OBJECTS, 0)) != string::npos) {
		/* no items on ground */
		clearStash(saiph->position);
	} else if ((pos = messages.find(MESSAGE_PICK_UP_WHAT, 0)) != string::npos) {
		/* picking up stuff.
		 * we should clear the stash here too and update it */
		cerr << "[ITEMTRACKER] picking up stuff" << endl;
		clearStash(saiph->position);
		pos = messages.find("  ", pos + 1);
		while (pos != string::npos && messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (messages[pos - 2] == '-') {
				Item item = parseItemText(messages.substr(pos, length));
				addItemToPickup(messages[pos - 4], item);
				addItemToStash(saiph->position, item);
			}
			pos += length;
		}
	} else if ((pos = messages.find(MESSAGE_NOT_CARRYING_ANYTHING, 0)) != string::npos) {
		/* our inventory is empty. how did that happen? */
		cerr << "[ITEMTRACKER] checked inventory and we're not carrying anything" << endl;
		inventory.clear();
	} else if ((pos = messages.find(".  ", 0)) != string::npos) {
		/* when we pick up stuff we only get "  f - a lichen corpse.  " and similar.
		 * we'll need to handle this too somehow.
		 * we're searching for ".  " as we won't get that when we're listing inventory.
		 * also, this won't detect gold, but we might not need to detect that,
		 * well, it's gonna be a bit buggy when picking up gold from stashes */
		cerr << "[ITEMTRACKER] possibly picked up stuff, adding to inventory" << endl;
		pos = 0;
		while ((pos = messages.find(" - ", pos)) != string::npos) {
			if (pos > 2 && messages[pos - 3] == ' ' && messages[pos - 2] == ' ') {
				unsigned char key = messages[pos - 1];
				pos += 3;
				string::size_type length = messages.find(".  ", pos);
				if (length == string::npos)
					break;
				length = length - pos;
				Item item = parseItemText(messages.substr(pos, length));
				addItemToInventory(key, item);
				removeItemFromStash(saiph->position, item);
				pos += length;
			}
		}
	} else if ((pos = messages.find(" - ", 0)) != string::npos) {
		/* we probably listed our inventory */
		cerr << "[ITEMTRACKER] possibly listing inventory" << endl;
		inventory.clear();
		while ((pos = messages.find(" - ", pos)) != string::npos) {
			if (pos > 2 && messages[pos - 3] == ' ' && messages[pos - 2] == ' ') {
				unsigned char key = messages[pos - 1];
				pos += 3;
				string::size_type length = messages.find("  ", pos);
				if (length == string::npos)
					break;
				length = length - pos;
				Item item = parseItemText(messages.substr(pos, length));
				addItemToInventory(key, item);
				removeItemFromStash(saiph->position, item);
				pos += length;
			}
		}
	}
	cerr << "[INVENTORY] ";
	for (map<unsigned char, Item>::iterator i = inventory.begin(); i != inventory.end(); ++i)
		cerr << i->first << " - " << i->second.count << " " << i->second.name << endl;
	cerr << endl;
	cerr << "[ON GROUND] ";
	for (list<Stash>::iterator s = stashes.begin(); s != stashes.end(); ++s) {
		if (s->branch != saiph->position.branch || s->level != saiph->position.level || s->row != saiph->position.row || s->col != saiph->position.col)
			continue;
		for (list<Item>::iterator i = s->items.begin(); i != s->items.end(); ++i)
			cerr << i->count << " " << i->name;
		break;
	}
	cerr << endl;
	cerr << "[PICKUP] ";
	for (map<unsigned char, Item>::iterator p = pickup.begin(); p != pickup.end(); ++p)
		cerr << p->first << " - " << p->second.count << " " << p->second.name << endl;
	cerr << endl;
}

void ItemTracker::removeStashes() {
	for (list<Stash>::iterator s = stashes.begin(); s != stashes.end(); ) {
		if (s->branch != saiph->position.branch || s->level != saiph->position.level) {
			++s;
			continue;
		}
		if (saiph->world->view[s->row][s->col] == s->top_symbol) {
			++s;
			continue; // same top_symbol
		}
		if (saiph->world->view[s->row][s->col] == saiph->map[s->branch][s->level].dungeon[s->row][s->col]) {
			/* stash seems to be gone */
			changed.push_back(*s);
			s = stashes.erase(s);
			continue;
		}
		/* if we're here, the stash is changed */
		changed.push_back(*s);
		++s;
	}
}

void ItemTracker::updateStash(const Point &point) {
	Coordinate coordinate = Coordinate(saiph->position.branch, saiph->position.level, point);
	unsigned char symbol = saiph->world->view[point.row][point.col];
	for (list<Stash>::iterator s = stashes.begin(); s != stashes.end(); ++s) {
		if (s->branch != coordinate.branch || s->level != coordinate.level || s->row != coordinate.row || s->col != coordinate.col)
			continue;
		if (s->top_symbol != symbol) {
			/* this stash has changed somehow */
		}
		return;
	}
	/* new stash */
	stashes.push_back(Stash(coordinate, symbol));
}

/* private methods */
void ItemTracker::addItemToInventory(unsigned char key, const Item &item) {
	if (item.count < 0)
		return;
	if (inventory.find(key) != inventory.end()) {
		/* existing item, add amount */
		inventory[key].count += item.count;
	} else {
		/* new item */
		inventory[key] = item;
	}
}

void ItemTracker::addItemToPickup(unsigned char key, const Item &item) {
	if (item.count < 0)
		return;
	inventory[key] = item;
}

void ItemTracker::addItemToStash(const Coordinate &coordinate, const Item &item) {
	if (item.count < 0)
		return;
	for (list<Stash>::iterator s = stashes.begin(); s != stashes.end(); ++s) {
		if (s->branch != coordinate.branch || s->level != coordinate.level || s->row != coordinate.row || s->col != coordinate.col)
			continue;
		s->addItem(item);
		return;
	}
	/* new stash */
	Stash stash(coordinate);
	stash.items.push_back(item);
	stashes.push_back(stash);
}

void ItemTracker::clearStash(const Coordinate &coordinate) {
	for (list<Stash>::iterator s = stashes.begin(); s != stashes.end(); ++s) {
		if (s->branch != coordinate.branch || s->level != coordinate.level || s->row != coordinate.row || s->col != coordinate.col)
			continue;
		stashes.erase(s);
		return;
	}
}

Item ItemTracker::parseItemText(const string &text) {
	cerr << "[PARSING ITEM] " << text << endl;
	Item item("", -1);
	char amount[8];
	char name[128];
	int matched = sscanf(text.c_str(), GET_SINGLE_ITEM, amount, name);
	if (matched != 2)
		return item; // unable to parse text as item
	/* figure out amount of items */
	if ((amount[0] == 'a' && (amount[1] == '\0' || (amount[1] == 'n' && amount[2] == '\0'))) || (amount[0] == 't' && amount[1] == 'h' && amount[2] == 'e' && amount[3] == '\0'))     
		item.count = 1; // "a", "an" or "the" <item>
	else if (amount[0] >= '0' || amount[0] <= '9')
		item.count = atoi(amount); // n <items>
	else    
		return item; // unable to parse text as item
	item.name = name;
	cerr << "[ITEM PARSED] " << item.count << " " << item.name << endl;
	return item;
}

void ItemTracker::removeItemFromInventory(unsigned char key, const Item &item) {
	if (item.count < 0)
		return;
	if (inventory.find(key) != inventory.end()) {
		Item &inv = inventory[key];
		if (inv.count > item.count) {
			/* we got more than we remove */
			inv.count -= item.count;
		} else {
			/* removing all we got */
			inventory.erase(key);
		}
	}
}

void ItemTracker::removeItemFromPickup(unsigned char key, const Item &item) {
	if (item.count < 0)
		return;
	if (pickup.find(key) != pickup.end()) {
		Item &pick = pickup[key];
		if (pick.count > item.count) {
			/* we got more than we remove */
			pick.count -= item.count;
		} else {
			/* removing all we got */
			pickup.erase(key);
		}
	}
}

void ItemTracker::removeItemFromStash(const Coordinate &coordinate, const Item &item) {
	if (item.count < 0)
		return;
	for (list<Stash>::iterator s = stashes.begin(); s != stashes.end(); ++s) {
		if (s->branch != coordinate.branch || s->level != coordinate.level || s->row != coordinate.row || s->col != coordinate.col)
			continue;
		s->removeItem(item);
		if (s->items.size() <= 0)
			stashes.erase(s);
		return;
	}
}
