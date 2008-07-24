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
	/* and on_ground */
	on_ground = NULL;
	string::size_type pos;
	if ((pos = messages.find(MESSAGE_YOU_SEE_HERE, 0)) != string::npos) {
		/* single item on ground */
		clearStash(saiph->position);
		pos += sizeof (MESSAGE_YOU_SEE_HERE) - 1;
		string::size_type length = messages.find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			Item item(messages.substr(pos, length));
			addItemToStash(saiph->position, item);
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
			Item item(messages.substr(pos, length));
			addItemToStash(saiph->position, item);
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
				Item item(messages.substr(pos, length));
				addItemToPickup(messages[pos - 4], item);
				addItemToStash(saiph->position, item);
			}
			pos += length;
		}
	} else if (messages.find(MESSAGE_NOT_CARRYING_ANYTHING, 0) != string::npos || messages.find(MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD, 0) != string::npos) {
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
				Item item(messages.substr(pos, length));
				addItemToInventory(key, item);
				removeItemFromPickup(item);
				removeItemFromStash(saiph->position, item);
				pos += length;
			} else {
				/* "Yak - dog food!" mess things up */
				++pos;
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
				Item item(messages.substr(pos, length));
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

void ItemTracker::removeItemFromInventory(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	map<unsigned char, Item>::iterator i = inventory.find(key);
	if (i == inventory.end())
		return;
	saiph->debugfile << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from inventory slot " << key << endl;
	if (i->second.count > item.count)
		i->second.count -= item.count; // we got more than we remove
	else
		inventory.erase(i); // removing all we got
}

void ItemTracker::removeStashes() {
	/* remove stashes that seems to be gone. */
	for (map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].begin(); s != stashes[saiph->position.branch][saiph->position.level].end(); ) {
		if (saiph->world->view[s->first.row][s->first.col] == saiph->map[saiph->position.branch][saiph->position.level].dungeon[s->first.row][s->first.col]) {
			/* stash seems to be gone */
			stashes[saiph->position.branch][saiph->position.level].erase(s++);
			continue;
		}
		++s;
	}
}

void ItemTracker::updateStash(const Point &point) {
	if (!item[(unsigned char) saiph->world->view[point.row][point.col]])
		return;
	map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].find(point);
	if (s != stashes[saiph->position.branch][saiph->position.level].end()) {
		if (s->second.top_symbol != saiph->world->view[point.row][point.col]) {
			/* top symbol changed, update */
			s->second.turn_changed = saiph->world->player.turn;
			s->second.top_symbol = saiph->world->view[point.row][point.col];
		}
		return; // know of this stash already
	}
	/* new stash */
	stashes[saiph->position.branch][saiph->position.level][point] = Stash(saiph->world->player.turn, saiph->world->view[point.row][point.col]);
}

/* private methods */
void ItemTracker::addItemToInventory(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	saiph->debugfile << ITEMTRACKER_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to inventory slot " << key << endl;
	if (inventory.find(key) != inventory.end()) {
		/* existing item, add amount */
		inventory[key].count += item.count;
	} else {
		/* new item */
		inventory[key] = item;
	}
}

void ItemTracker::addItemToPickup(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	saiph->debugfile << ITEMTRACKER_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to pickup slot " << key << endl;
	pickup[key] = item;
}

void ItemTracker::addItemToStash(const Point &point, const Item &item) {
	if (item.count <= 0)
		return;
	saiph->debugfile << ITEMTRACKER_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to stash at " << saiph->position.branch << ", " << saiph->position.level << ", " << point.row << ", " << point.col << endl;
	map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].find(point);
	if (s != stashes[saiph->position.branch][saiph->position.level].end()) {
		s->second.addItem(item);
		return;
	}
	/* new stash */
	Stash stash(saiph->world->player.turn);
	stash.items.push_back(item);
	stashes[saiph->position.branch][saiph->position.level][point] = stash;
}

void ItemTracker::clearStash(const Point &point) {
	/* clear the contents of a stash */
	saiph->debugfile << ITEMTRACKER_DEBUG_NAME << "Clearing stash at " << saiph->position.branch << ", " << saiph->position.level << ", " << point.row << ", " << point.col << endl;
	map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].find(point);
	if (s != stashes[saiph->position.branch][saiph->position.level].end())
		s->second.items.clear();
}

void ItemTracker::removeItemFromPickup(const Item &item) {
	/* we currently don't have a way to tell exactly which item we removed.
	 * we'll have to search and remove the best match */
	if (item.count <= 0)
		return;
	saiph->debugfile << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from pickup" << endl;
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

void ItemTracker::removeItemFromStash(const Point &point, const Item &item) {
	if (item.count <= 0)
		return;
	saiph->debugfile << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from stash at " << saiph->position.branch << ", " << saiph->position.level << ", " << point.row << ", " << point.col << endl;
	map<Point, Stash>::iterator s = stashes[saiph->position.branch][saiph->position.level].find(point);
	if (s != stashes[saiph->position.branch][saiph->position.level].end())
		s->second.removeItem(item);
}
