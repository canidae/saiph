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
	on_ground = NULL;
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
		inventory.clear();
	} else if ((pos = messages.find(".  ", 0)) != string::npos) {
		/* when we pick up stuff we only get "  f - a lichen corpse.  " and similar.
		 * we'll need to handle this too somehow.
		 * we're searching for ".  " as we won't get that when we're listing inventory.
		 * also, this won't detect gold, but we might not need to detect that,
		 * well, it's gonna be a bit buggy when picking up gold from stashes */
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
}

void ItemTracker::removeStashes() {
	/* remove stashes that seems to be gone. */
	for (map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].begin(); s != stashes[saiph->position.branch][saiph->position.level].end(); ) {
		if (saiph->world->view[s->second.row][s->second.col] == s->second.top_symbol) {
			++s;
			continue; // same top_symbol
		}
		if (saiph->world->view[s->second.row][s->second.col] == saiph->map[saiph->position.branch][saiph->position.level].dungeon[s->second.row][s->second.col]) {
			/* stash seems to be gone */
			changed.push_back(s->second);
			stashes[saiph->position.branch][saiph->position.level].erase(s++);
			continue;
		}
		/* if we're here, the stash is changed */
		changed.push_back(s->second);
		++s;
	}
}

void ItemTracker::updateStash(const Point &point) {
	unsigned char symbol = saiph->world->view[point.row][point.col];
	for (map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].begin(); s != stashes[saiph->position.branch][saiph->position.level].end(); ++s) {
		if (s->second.row != point.row || s->second.col != point.col)
			continue;
		if (s->second.top_symbol == symbol)
			continue;
		/* this stash has changed somehow */
		changed.push_back(point);
		return;
	}
	/* new stash */
	stashes[saiph->position.branch][saiph->position.level][point] = Stash(Coordinate(saiph->position.branch, saiph->position.level, point), symbol);
	changed.push_back(point);
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
	for (map<Point, Stash>::iterator s = stashes[coordinate.branch][coordinate.level].begin(); s != stashes[coordinate.branch][coordinate.level].end(); ++s) {
		if (s->second.row != coordinate.row || s->second.col != coordinate.col)
			continue;
		s->second.addItem(item);
		return;
	}
	/* new stash */
	Stash stash(coordinate);
	stash.items.push_back(item);
	stashes[coordinate.branch][coordinate.level][coordinate] = stash;
}

void ItemTracker::clearStash(const Coordinate &coordinate) {
	for (map<Point, Stash>::iterator s = stashes[coordinate.branch][coordinate.level].begin(); s != stashes[coordinate.branch][coordinate.level].end(); ++s) {
		if (s->second.row != coordinate.row || s->second.col != coordinate.col)
			continue;
		stashes[coordinate.branch][coordinate.level].erase(s);
		return;
	}
}

Item ItemTracker::parseItemText(const string &text) {
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
	for (map<Point, Stash>::iterator s = stashes[coordinate.branch][coordinate.level].begin(); s != stashes[coordinate.branch][coordinate.level].end(); ++s) {
		if (s->second.row != coordinate.row || s->second.col != coordinate.col)
			continue;
		s->second.removeItem(item);
		if (s->second.items.size() <= 0)
			stashes[coordinate.branch][coordinate.level].erase(s);
		return;
	}
}
