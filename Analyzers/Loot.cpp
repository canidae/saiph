#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph) {
	memset(check_item, false, sizeof (check_item));
	check_item[WEAPON] = true;
	check_item[ARMOR] = true;
	check_item[RING] = true;
	check_item[AMULET] = true;
	check_item[TOOL] = true;
	check_item[FOOD] = true;
	check_item[POTION] = true;
	check_item[SCROLL] = true;
	check_item[SPELLBOOK] = true;
	check_item[WAND] = true;
	check_item[GOLD] = true;
	check_item[GEM] = true;
	check_item[STATUE] = true;
	// skipping boulder as that's a special item
	check_item[IRON_BALL] = true;
	check_item[CHAINS] = true;
	check_item[VENOM] = true;

	/* set top_item to ILLEGAL_ITEM */
	for (int b = 0; b < MAX_BRANCHES; ++b) {
		for (int l = 0; l < MAX_DUNGEON_DEPTH; ++l) {
			for (int r = 0; r <= MAP_ROW_END; ++r) {
				for (int c = 0; c <= MAP_COL_END; ++c)
					stashes[b][l][r][c].top_item = ILLEGAL_ITEM;
			}
		}
	}

	last_turn_inventory_check = INT_MIN;
}

/* methods */
void Loot::command(string *command) {
	*command = action;
	if (action == "i") {
		/* checking inventory, set last_turn_inventory_check */
		last_turn_inventory_check = saiph->world->player.turn;
	}
	if (action.size() > 0 && action[0] == PICKUP) {
		/* picking up items */
		int b = saiph->current_branch;
		int l = saiph->current_level;
		int r = saiph->world->player.row;
		int c = saiph->world->player.col;
		if (stashes[b][l][r][c].items.size() == 1) {
			/* only 1 item here which we'll pick up.
			 * erase stash & pickup orders for this stash */
			removeStash(b, l, r, c);
		}
	}
}

void Loot::finish() {
	/* figure out which stash to visit, if any */
	if (action == ":")
		return;
	if (saiph->world->player.turn > last_turn_inventory_check + LOOT_CHECK_INVENTORY_INTERVAL) {
		action = "i";
		priority = PRIORITY_LOOK;
		return;
	}
	int best_distance = INT_MAX;
	int best_priority = -1;
	action = "";
	/* pickup items */
	for (list<Request>::iterator p = pickup.begin(); p != pickup.end(); ++p) {
		if (p->coordinate.branch != saiph->current_branch || p->coordinate.level != saiph->current_level)
			continue;
		if (p->priority < best_priority)
			continue;
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(p->coordinate, false, &distance, &straight_line);
		if (move != ILLEGAL_MOVE && distance < best_distance) {
			best_distance = distance;
			best_priority = p->priority;
			action = move;
		}
	}
	if (best_priority >= LOOT_VISIT_STASH_PRIORITY && action.size() > 0) {
		/* we should pick up something */
		if (action[0] == REST) {
			/* infact, we should pick up something _here_ */
			action = PICKUP;
		}
		priority = best_priority;
		return;
	}
	best_distance = INT_MAX;
	action = "";
	for (list<Point>::iterator v = visit.begin(); v != visit.end(); ) {
		if (v->row == saiph->world->player.row && v->col == saiph->world->player.col) {
			/* we're on the stash, remove it from visit list */
			v = visit.erase(v);
			continue;
		}
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(*v, false, &distance, &straight_line);
		if (move != ILLEGAL_MOVE && distance < best_distance) {
			best_distance = distance;
			action = move;
		}
		++v;
	}
	if (best_distance < INT_MAX)
		priority = LOOT_VISIT_STASH_PRIORITY;
}

void Loot::inspect(const Point &point) {
	unsigned char s = saiph->world->view[point.row][point.col];
	int b = saiph->current_branch;
	int l = saiph->current_level;
	if (check_item[s]) {
		/* we're interested in this symbol */
		if (stashes[b][l][point.row][point.col].items.size() > 0) {
			/* we know of a stash here already */
			if (stashes[b][l][point.row][point.col].top_item == s) {
				/* top item is the same as last time we checked. */
				return;
			} else {
				/* top item changed, we should check this stash again */
			}
		} else {
			/* we've not seen a stash here before, we should check */
		}
		/* visit stash (again) */
		stashes[b][l][point.row][point.col].top_item = s;
		visit.push_back(point);
	} else if (s == saiph->map[b][l].dungeon[point.row][point.col] && stashes[b][l][point.row][point.col].items.size() > 0) {
		/* there used to be a stash here, but now it's gone */
		removeStash(b, l, point.row, point.col);
	}
}

void Loot::parseMessages(string *messages) {
	if (last_turn_inventory_check == saiph->world->player.turn && saiph->world->menu) {
		/* we requested the inventory to be listed, and it was.
		 * now parse it */
		inventory.clear();
		string::size_type pos = messages->find("  ");
		announce.announce = ANNOUNCE_ITEM_IN_INVENTORY;
		while (pos != string::npos && messages->size() > pos + 6) {
			pos += 6;
			string::size_type length = messages->find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if ((*messages)[pos - 2] == '-') {
				unsigned char key = (*messages)[pos - 4];
				Item item = parseMessageItem(messages->substr(pos, length));
				if (item.count > 0)
					inventory[key] = item;
				announce.data = item.name;
				announce.key = key;
				announce.value1 = item.count;
				saiph->announce(announce);
			}
			pos += length;
		}
		priority = PRIORITY_CONTINUE_ACTION;
		action = " ";
		return;
	}
	int b = saiph->current_branch;
	int l = saiph->current_level;
	int r = saiph->world->player.row;
	int c = saiph->world->player.col;
	vector<Item> *stash = &stashes[b][l][r][c].items;
	string::size_type pos = messages->find(MESSAGE_PICK_UP_WHAT, 0);
	if (pos != string::npos) {
		/* we're trying to pick up something */
		if (stash->size() > 0) {
			/* we know about this stash already.
			 * ditch the previous content of this stash */
			stash->clear();
		} else {
			/* new stash, add it to stash_locations */
			stash_locations.push_back(Coordinate(b, l, r, c));
		}
		action = "";
		pos = messages->find("  ", pos + 1);
		while (pos != string::npos && messages->size() > pos + 6) {
			pos += 6;
			string::size_type length = messages->find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if ((*messages)[pos - 2] == '-') {
				Item item = parseMessageItem(messages->substr(pos, length));
				if (item.count > 0) {
					for (list<Request>::iterator p = pickup.begin(); p != pickup.end(); ) {
						if (p->coordinate.branch != b || p->coordinate.level != l || p->coordinate.row != r || p->coordinate.col != c || p->data != item.name) {
							++p;
							continue;
						}
						/* pick up */
						if (p->value1 < item.count) {
							/* only some of them */
							action.append(1, (unsigned char) p->value1);
							item.count = item.count - p->value1;
							/* only add to the stash what we're not picking up */
							stash->push_back(item);
						}
						action.append(1, (*messages)[pos - 4]); // the key before the item
						/* remove from pickup list */
						p = pickup.erase(p);

					}
				}
			}
			pos += length;
		}
		action.append(" ");
		priority = PRIORITY_CONTINUE_ACTION;
		return;
	}
	pos = messages->find(MESSAGE_YOU_SEE_HERE, 0);
	if (pos != string::npos) {
		/* one item on the floor */
		pos += sizeof (MESSAGE_YOU_SEE_HERE) - 1;
		string::size_type length = messages->find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			if (stash->size() > 0) {
				/* we know about this stash already.
				 * ditch the previous content of this stash */
				stash->clear();
			} else {
				/* new stash, add it to stash_locations */
				stash_locations.push_back(Coordinate(b, l, r, c));
			}
			Item item = parseMessageItem(messages->substr(pos, length));
			if (item.count > 0)
				stash->push_back(item);
		}
	}
	pos = messages->find(MESSAGE_THINGS_THAT_ARE_HERE, 0);
	if (pos != string::npos) {
		/* multiple items on the floor */
		if (stash->size() > 0) {
			/* we know about this stash already.
			 * ditch the previous content of this stash */
			stash->clear();
		} else {
			/* new stash, add it to stash_locations */
			stash_locations.push_back(Coordinate(b, l, r, c));
		}
		pos = messages->find("  ", pos + 1);
		while (pos != string::npos && messages->size() > pos + 2) {
			pos += 2;
			string::size_type length = messages->find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			Item item = parseMessageItem(messages->substr(pos, length));
			if (item.count > 0)
				stash->push_back(item);
			pos += length;
		}
	}
	/* if we get "there are several/many objects here" we need to ":".
	 * if we don't, we'll get an endless loop.
	 * it's a zero turn action anyways, so we're cool */
	if (messages->find(MESSAGE_MANY_OBJECTS_HERE, 0) != string::npos || messages->find(MESSAGE_SEVERAL_OBJECTS_HERE, 0) != string::npos) {
		action = ":";
		priority = PRIORITY_LOOK;
		return;
	}
	if (stash->size() > 0) {
		/* announce what's on the ground to the other analyzers */
		announce.coordinate.branch = b;
		announce.coordinate.level = l;
		announce.coordinate.row = r;
		announce.coordinate.col = c;
		announce.announce = ANNOUNCE_ITEM_ON_GROUND;
		for (vector<Item>::iterator i = stash->begin(); i != stash->end(); ++i) {
			announce.data = i->name;
			announce.value1 = i->count;
			saiph->announce(announce);
		}
	}
	/* when we've picked up stuff we get eg. "f - a lichen corpse".
	 * we need to parse this too... meh */
	pos = 0;
	announce.announce = ANNOUNCE_ITEM_IN_INVENTORY;
	while ((pos = messages->find(" - ", pos)) != string::npos) {
		if (pos > 2 && (*messages)[pos - 3] == ' ' && (*messages)[pos - 2] == ' ') {
			unsigned char key = (*messages)[pos - 1];
			pos += 3;
			string::size_type length = messages->find(".  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			Item item = parseMessageItem(messages->substr(pos, length));
			if (item.count > 0) {
				map<unsigned char, Item>::iterator existing = inventory.find(key);
				if (existing != inventory.end()) {
					existing->second.count += item.count;
					item.count = existing->second.count;
				} else {
					inventory[key] = item;
				}
				announce.data = item.name;
				announce.key = key;
				announce.value1 = item.count;
				saiph->announce(announce);
			}
			pos += length;
		}
	}
}

bool Loot::request(const Request &request) {
	if (request.request == REQUEST_PICK_UP_ITEM) {
		/* add it to pickup list */
		pickup.push_back(request);
		return true;
	}
	return false;
}

/* private methods */
Item Loot::parseMessageItem(const string &message) {
	Item item("", -1);
	char amount[8];
	char name[128];
	int matched = sscanf(message.c_str(), GET_SINGLE_ITEM, amount, name);
	if (matched != 2) {
		/* this may happen when:
		 * "Things that are here:  a rothe corpse  16 gold pieces  Something is written here in the dust." */
		return item;
	}
	/* figure out amount of items */
	int count;
	if ((amount[0] == 'a' && (amount[1] == '\0' || (amount[1] == 'n' && amount[2] == '\0'))) || (amount[0] == 't' && amount[1] == 'h' && amount[2] == 'e' && amount[3] == '\0'))
		count = 1; // "a", "an" or "the" <item>
	else if (amount[0] >= '0' || amount[0] <= '9')
		count = atoi(amount);
	else
		return item; // hmm, unable to parse this
	item.name = name;
	item.count = count;
	return item;
}

void Loot::removeStash(int branch, int level, int row, int col) {
	stashes[branch][level][row][col].items.clear();
	stashes[branch][level][row][col].top_item = ILLEGAL_ITEM;
	for (list<Request>::iterator p = pickup.begin(); p != pickup.end(); ) {
		if (p->coordinate.branch == branch && p->coordinate.level == level && p->coordinate.row == row && p->coordinate.col == col) {
			p = pickup.erase(p);
			continue;
		}
		++p;
	}
	for (list<Coordinate>::iterator sl = stash_locations.begin(); sl != stash_locations.end(); ++sl) {
		if (sl->branch == branch && sl->level == level && sl->row == row && sl->col == col) {
			/* found the stash, erase it */
			stash_locations.erase(sl);
			return;
		}
	}
}
