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
void ItemTracker::begin() {
	/* reset certain things */
	changed.clear();
	on_ground = NULL;
}

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
		/* additionally, we'll assume we're picking up from the stash at this location.
		 * this will also trigger on wishes, but meh, probably not gonna be an issue */
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
				removeItemFromPickup(item);
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
	/* we'll need to set the on_ground pointer here */
	if (stashes[saiph->position.branch][saiph->position.level].find(saiph->position) != stashes[saiph->position.branch][saiph->position.level].end())
		on_ground = &stashes[saiph->position.branch][saiph->position.level][saiph->position];
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
		++s;
	}
}

void ItemTracker::updateStash(const Point &point) {
	unsigned char symbol = saiph->world->view[point.row][point.col];
	map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].find(point);
	if (s != stashes[saiph->position.branch][saiph->position.level].end()) {
		if (s->second.top_symbol == symbol)
			return;
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
	cerr << "[ITEMTRACKER] Adding " << item.count << " " << item.name << " to inventory slot " << key << endl;
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
	cerr << "[ITEMTRACKER] Adding " << item.count << " " << item.name << " to pickup slot " << key << endl;
	if (item.count < 0)
		return;
	pickup[key] = item;
}

void ItemTracker::addItemToStash(const Coordinate &coordinate, const Item &item) {
	cerr << "[ITEMTRACKER] Adding " << item.count << " " << item.name << " to stash at " << coordinate.branch << ", " << coordinate.level << ", " << coordinate.row << ", " << coordinate.col << endl;
	if (item.count < 0)
		return;
	map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].find(coordinate);
	if (s != stashes[saiph->position.branch][saiph->position.level].end()) {
		s->second.addItem(item);
		return;
	}
	/* new stash */
	cerr << "NEW STASH OMGWTFBBQ" << endl;
	Stash stash(coordinate);
	stash.items.push_back(item);
	stashes[coordinate.branch][coordinate.level][coordinate] = stash;
}

void ItemTracker::clearStash(const Coordinate &coordinate) {
	cerr << "[ITEMTRACKER] Clearing stash at " << coordinate.branch << ", " << coordinate.level << ", " << coordinate.row << ", " << coordinate.col << endl;
	/* clear the contents of a stash */
	map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].find(coordinate);
	if (s != stashes[saiph->position.branch][saiph->position.level].end())
		s->second.items.clear();
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
	cerr << "[ITEMTRACKER] Removing " << item.count << " " << item.name << " from inventory slot " << key << endl;
	if (item.count < 0)
		return;
	map<unsigned char, Item>::iterator i = inventory.find(key);
	if (i == inventory.end())
		return;
	if (i->second.count > item.count)
		i->second.count -= item.count; // we got more than we remove
	else
		inventory.erase(i); // removing all we got
}

void ItemTracker::removeItemFromPickup(const Item &item) {
	/* we currently don't have a way to tell exactly which item we removed.
	 * we'll have to search and remove the best match */
	cerr << "[ITEMTRACKER] Removing " << item.count << " " << item.name << " from pickup" << endl;
	if (item.count < 0)
		return;
	for (map<unsigned char, Item>::iterator p = pickup.begin(); p != pickup.end(); ++p) {
		if (p->second.name != item.name)
			continue;
		if (p->second.count > item.count) {
			/* we got more than we remove */
			p->second.count -= item.count;
		} else {
			/* removing all we got */
			pickup.erase(p);
		}
		return;
	}
}

void ItemTracker::removeItemFromStash(const Coordinate &coordinate, const Item &item) {
	cerr << "[ITEMTRACKER] Removing " << item.count << " " << item.name << " from stash at " << coordinate.branch << ", " << coordinate.level << ", " << coordinate.row << ", " << coordinate.col << endl;
	if (item.count < 0)
		return;
	map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].find(coordinate);
	if (s != stashes[saiph->position.branch][saiph->position.level].end())
		s->second.removeItem(item);
}
