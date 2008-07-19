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
}

/* methods */
void Loot::command(string *command) {
	*command = action;
}

void Loot::finish() {
	/* figure out which stash to visit, if any */
	if (action == ":")
		return;
	int best_distance = INT_MAX;
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
		stashes[b][l][point.row][point.col].items.clear();
		for (list<Coordinate>::iterator sl = stash_locations.begin(); sl != stash_locations.end(); ++sl) {
			if (sl->branch == b && sl->level == l && sl->row == point.row && sl->col == point.col) {
				/* found the stash, erase it */
				stash_locations.erase(sl);
				return;
			}
		}
	}
}

void Loot::parseMessages(string *messages) {
	int b = saiph->current_branch;
	int l = saiph->current_level;
	int r = saiph->world->player.row;
	int c = saiph->world->player.col;
	vector<Item> *stash = &stashes[b][l][r][c].items;
	string::size_type pos = messages->find(MESSAGE_YOU_SEE_HERE, 0);
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
			parseMessageItem(messages->substr(pos, length), stash);
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
		while (pos != string::npos) {
			pos += 2;
			string::size_type length = messages->find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			parseMessageItem(messages->substr(pos, length), stash);
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
		for (vector<Item>::iterator i = stash->begin(); i != stash->end(); ++i) {
			announce.announce = ANNOUNCE_ITEM_ON_GROUND;
			announce.data = i->name;
			announce.value1 = i->count;
			saiph->announce(announce);
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
void Loot::parseMessageItem(const string &message, vector<Item> *stash) {
	char amount[8];
	char name[128];
	int matched = sscanf(message.c_str(), GET_SINGLE_ITEM, amount, name);
	if (matched != 2) {
		/* this may happen when:
		 * "Things that are here:  a rothe corpse  16 gold pieces  Something is written here in the dust." */
		return;
	}
	/* figure out amount of items */
	int count;
	if ((amount[0] == 'a' && (amount[1] == '\0' || (amount[1] == 'n' && amount[2] == '\0'))) || (amount[0] == 't' && amount[1] == 'h' && amount[2] == 'e' && amount[3] == '\0'))
		count = 1; // "a", "an" or "the" <item>
	else if (amount[0] >= '0' || amount[0] <= '9')
		count = atoi(amount);
	else
		return; // hmm, unable to parse this
	/* add item to stash */
	stash->push_back(Item(name, count));
}
