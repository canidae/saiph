#include "Saiph.h"

/* constructors */
Saiph::Saiph(int interface) {
	debugfile.open("debug.log", ios::trunc);
	connection = Connection::create(interface, &debugfile);
	if (connection == NULL) {
		cout << "ERROR: Don't know what interface this is: " << interface << endl;
		exit(1);
	}
	world = new World(connection, &debugfile);

	/* engulfed */
	engulfed = false;

	/* null out maps */
	memset(dungeonmap, SOLID_ROCK, sizeof (dungeonmap));
	memset(monstermap, ILLEGAL_MONSTER, sizeof (monstermap));

	/* set on_ground to NULL */
	on_ground = NULL;

	/* tracking, pathing & maps */
	for (int a = 0; a <= UCHAR_MAX; ++a) {
		/* monstertracking */
		if ((a >= '@' && a <= 'Z') || (a >= 'a' && a <= 'z') || (a >= '1' && a <= '5')  || a == '&' || a == '\'' || a == ':' || a == ';' || a == '~' || a == PET)
			monster[a] = true;
		else
			monster[a] = false;
		/* itemtracking */
		item[a] = false;
		/* pathing & maps */
		passable[a] = false;
		dungeon[a] = false;
		pathcost[a] = 0;
	}
	/* items */
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
	/* pathing & maps */
	passable[(unsigned char) FLOOR] = true;
	passable[(unsigned char) OPEN_DOOR] = true;
	passable[(unsigned char) CORRIDOR] = true;
	passable[(unsigned char) STAIRS_UP] = true;
	passable[(unsigned char) STAIRS_DOWN] = true;
	passable[(unsigned char) ALTAR] = true;
	passable[(unsigned char) GRAVE] = true;
	passable[(unsigned char) THRONE] = true;
	passable[(unsigned char) SINK] = true;
	passable[(unsigned char) FOUNTAIN] = true;
	passable[(unsigned char) WATER] = true;
	passable[(unsigned char) ICE] = true;
	passable[(unsigned char) LAVA] = true;
	passable[(unsigned char) LOWERED_DRAWBRIDGE] = true;
	passable[(unsigned char) TRAP] = true;
	passable[(unsigned char) UNKNOWN_TILE] = true;
	passable[(unsigned char) UNKNOWN_TILE_DIAGONALLY_PASSABLE] = true;
	passable[(unsigned char) WEAPON] = true;
	passable[(unsigned char) ARMOR] = true;
	passable[(unsigned char) RING] = true;
	passable[(unsigned char) AMULET] = true;
	passable[(unsigned char) TOOL] = true;
	passable[(unsigned char) FOOD] = true;
	passable[(unsigned char) POTION] = true;
	passable[(unsigned char) SCROLL] = true;
	passable[(unsigned char) SPELLBOOK] = true;
	passable[(unsigned char) WAND] = true;
	passable[(unsigned char) GOLD] = true;
	passable[(unsigned char) GEM] = true;
	passable[(unsigned char) STATUE] = true;
	passable[(unsigned char) IRON_BALL] = true;
	passable[(unsigned char) CHAINS] = true;
	passable[(unsigned char) VENOM] = true;
	passable[(unsigned char) PET] = true;
	dungeon[(unsigned char) VERTICAL_WALL] = true;
	dungeon[(unsigned char) HORIZONTAL_WALL] = true;
	dungeon[(unsigned char) FLOOR] = true;
	dungeon[(unsigned char) OPEN_DOOR] = true;
	dungeon[(unsigned char) CLOSED_DOOR] = true;
	dungeon[(unsigned char) IRON_BARS] = true;
	dungeon[(unsigned char) TREE] = true;
	dungeon[(unsigned char) CORRIDOR] = true;
	dungeon[(unsigned char) STAIRS_UP] = true;
	dungeon[(unsigned char) STAIRS_DOWN] = true;
	dungeon[(unsigned char) ALTAR] = true;
	dungeon[(unsigned char) GRAVE] = true;
	dungeon[(unsigned char) THRONE] = true;
	dungeon[(unsigned char) SINK] = true;
	dungeon[(unsigned char) FOUNTAIN] = true;
	dungeon[(unsigned char) WATER] = true;
	dungeon[(unsigned char) ICE] = true;
	dungeon[(unsigned char) LAVA] = true;
	dungeon[(unsigned char) LOWERED_DRAWBRIDGE] = true;
	dungeon[(unsigned char) RAISED_DRAWBRIDGE] = true;
	dungeon[(unsigned char) TRAP] = true;
	dungeon[(unsigned char) BOULDER] = true; // hardly static, but we won't allow moving on to one
	pathcost[ICE] = COST_ICE;
	pathcost[LAVA] = COST_LAVA;
	pathcost[PET] = COST_PET;
	pathcost[TRAP] = COST_TRAP;
	pathcost[WATER] = COST_WATER;

	/* Analyzers */
	analyzers.push_back(new Door(this));
	analyzers.push_back(new Elbereth(this));
	analyzers.push_back(new Excalibur(this));
	analyzers.push_back(new Explore(this));
	analyzers.push_back(new Fight(this));
	analyzers.push_back(new Food(this));
	analyzers.push_back(new Fountain(this));
	analyzers.push_back(new Health(this));
	analyzers.push_back(new Level(this));
	analyzers.push_back(new Loot(this));
	analyzers.push_back(new Pray(this));
	analyzers.push_back(new Zorkmid(this));
}

/* destructor */
Saiph::~Saiph() {
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		delete *a;
	delete world;
	delete connection;
	debugfile.close();
}

/* methods */
void Saiph::farlook(const Point &target) {
	/* look at something, eg. monster */
	command.push_back(';');
	Point cursor(world->player.row, world->player.col);
	while (cursor.row != target.row && cursor.col != target.col) {
		unsigned char move;
		if (cursor.row < target.row && cursor.col < target.col) {
			move = MOVE_SE;
			++cursor.row;
			++cursor.col;
		} else if (cursor.row < target.row && cursor.col > target.col) {
			move = MOVE_SW;
			++cursor.row;
			--cursor.col;
		} else if (cursor.row > target.row && cursor.col < target.col) {
			move = MOVE_NE;
			--cursor.row;
			++cursor.col;
		} else if (cursor.row > target.row && cursor.col > target.col) {
			move = MOVE_NW;
			--cursor.row;
			--cursor.col;
		} else if (cursor.row < target.row) {
			move = MOVE_S;
			++cursor.row;
		} else if (cursor.row > target.row) {
			move = MOVE_N;
			--cursor.row;
		} else if (cursor.col < target.col) {
			move = MOVE_E;
			++cursor.col;
		} else {
			move = MOVE_W;
			--cursor.col;
		}
		command.push_back(move);
	}
	command.push_back(',');
	world->executeCommand(command);
}

void Saiph::removeItemFromInventory(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	map<unsigned char, Item>::iterator i = inventory.find(key);
	if (i == inventory.end())
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from inventory slot " << key << endl;
	if (i->second.count > item.count)
		i->second.count -= item.count; // we got more than we remove
	else
		inventory.erase(i); // removing all we got
}

bool Saiph::request(const Request &request) {
	/* request an action from any analyzer */
	debugfile << REQUEST_DEBUG_NAME << request.request << ", " << request.priority << ", " << request.value << ", " << request.data << ", (" << request.coordinate.branch << ", " << request.coordinate.level << ", " << request.coordinate.row << ", " << request.coordinate.col << ")" << endl;
	bool status = false;
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
		if ((*a)->request(request) && !status)
			status = true;
	}
	return status;
}

bool Saiph::run() {
	/* clear pickup list */
	pickup.clear();
	/* and on_ground */
	on_ground = NULL;

	/* clear analyzers priority */
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		(*a)->priority = ILLEGAL_PRIORITY;

	/* check if we're engulfed */
	if (position.row > MAP_ROW_BEGIN && position.row < MAP_ROW_END && position.col > MAP_COL_BEGIN && position.col < MAP_COL_END && world->view[position.row - 1][position.col - 1] == '/' && world->view[position.row - 1][position.col + 1] == '\\' && world->view[position.row + 1][position.col - 1] == '\\' && world->view[position.row + 1][position.col + 1] == '/')
		engulfed = true;
	else
		engulfed = false;

	/* figure out which map to use.
	 * TODO: we need some branch detection & stuff here */
	position.branch = 0;
	position.level = world->player.dungeon;
	position.row = world->player.row;
	position.col = world->player.col;

	/* deal with messages */
	debugfile << MESSAGES_DEBUG_NAME << "'" << world->messages << "'" << endl;
	/* global parsing */
	parseMessages();

	/* update maps */
	if (!world->question && !world->menu && !engulfed)
		updateMaps();
	/* print maps so we see what we're doing */
	dumpMaps();

	/* then analyzer message parsing */
	for (vector<Analyzer *>::size_type a = 0; a < analyzers.size(); ++a)
		analyzers[a]->parseMessages(world->messages);

	/* call start() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer *>::size_type a = 0; a < analyzers.size(); ++a)
			analyzers[a]->start();
	}

	/* inspect the dungeon */
	if (!world->question && !world->menu)
		inspect();

	/* call finish() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer *>::size_type a = 0; a < analyzers.size(); ++a)
			analyzers[a]->finish();
	}

	/* check if we got a command */
	int best_analyzer = -1;
	int best_priority = ILLEGAL_PRIORITY;
	for (vector<Analyzer *>::size_type a = 0; a < analyzers.size(); ++a) {
		if (analyzers[a]->priority > best_priority) {
			best_priority = analyzers[a]->priority;
			best_analyzer = a;
		}
	}

	if (world->question && best_analyzer == -1) {
		debugfile << SAIPH_DEBUG_NAME << "Unhandled question: " << world->messages << endl;
		return false;
	}

	if (world->menu && best_analyzer == -1) {
		debugfile << SAIPH_DEBUG_NAME << "Unhandled menu: " << world->messages << endl;
		return false;
	}

	if (best_analyzer == -1)
		return false;

	/* let an analyzer do its command */
	command.clear(); // just in case some analyzer messed with this string
	analyzers[best_analyzer]->command(&command);
	debugfile << COMMAND_DEBUG_NAME << "'" << command << "' from analyzer " << analyzers[best_analyzer]->name << " with priority " << best_priority << endl;
	world->executeCommand(command);
	return true;
}

unsigned char Saiph::shortestPath(const Point &target, bool allow_illegal_last_move, int *distance, bool *straight_line) {
	/* returns next move in shortest path from player to target.
	 * also sets "distance" and "straight_line" to target */
	if (target.row < MAP_ROW_BEGIN || target.row > MAP_ROW_END || target.col < MAP_COL_BEGIN || target.col > MAP_COL_END)
		return ILLEGAL_MOVE; // outside the map
	PathNode *node = &pathmap[target.row][target.col];
	*distance = 0;
	*straight_line = true;
	if (node->cost == 0)
		return REST; // pathing to player?
	++*distance;
	unsigned char move = ILLEGAL_MOVE;
	unsigned char previous_move = ILLEGAL_MOVE; // used to determine straight_line
	if (allow_illegal_last_move && node->nextnode == NULL) {
		/* sometimes we wish to move somewhere we really can't move to.
		 * for example: fighting a monster in a wall or through "corner".
		 * solution: find adjacent squares with lowest cost and backtrack from there */
		/* note:
		 * since we're moving from target to player,
		 * we're moving the opposite direction of the node we're checking */
		/* northwest node */
		int row = target.row - 1;
		int col = target.col - 1;
		move = MOVE_SE;
		node = &pathmap[row][col];
		unsigned int lowest_cost = node->cost;
		/* north node */
		++col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_S;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* northeast node */
		++col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_SW;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* east node */
		++row;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_W;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* southeast node */
		++row;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_NW;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* south node */
		--col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_N;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* southwest node */
		--col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_NE;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* west node */
		--row;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_E;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		previous_move = move;
		if (lowest_cost == 0)
			return move; // found the player
		++*distance;
	}
	if (node->nextnode == NULL)
		return ILLEGAL_MOVE; // couldn't find path

	while (node->nextnode != NULL) {
		previous_move = move;
		move = node->move;
		if (*distance > 1 && previous_move != move)
			*straight_line = false;
		++*distance;
		node = node->nextnode;
	}
	return move;
}

/* private methods */
void Saiph::addItemToInventory(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to inventory slot " << key << endl;
	if (inventory.find(key) != inventory.end()) {
		/* existing item, add amount */
		inventory[key].count += item.count;
	} else {
		/* new item */
		inventory[key] = item;
	}
}

void Saiph::addItemToPickup(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to pickup slot " << key << endl;
	pickup[key] = item;
}

void Saiph::addItemToStash(const Point &point, const Item &item) {
	if (item.count <= 0)
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to stash at " << position.branch << ", " << position.level << ", " << point.row << ", " << point.col << endl;
	map<Point, Stash>::iterator s = stashes[position.branch][position.level].find(point);
	if (s != stashes[position.branch][position.level].end()) {
		s->second.addItem(item);
		return;
	}
	/* new stash */
	Stash stash(world->player.turn);
	stash.items.push_back(item);
	stashes[position.branch][position.level][point] = stash;
}

void Saiph::clearStash(const Point &point) {
	/* clear the contents of a stash */
	debugfile << ITEMTRACKER_DEBUG_NAME << "Clearing stash at " << position.branch << ", " << position.level << ", " << point.row << ", " << point.col << endl;
	map<Point, Stash>::iterator s = stashes[position.branch][position.level].find(point);
	if (s != stashes[position.branch][position.level].end())
		s->second.items.clear();
}

void Saiph::dumpMaps() {
	/* monsters */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";2H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (monstermap[position.branch][position.level][r][c] != ILLEGAL_MONSTER)
				cout << (unsigned char) (monstermap[position.branch][position.level][r][c]);
			else
				cout << (unsigned char) (dungeonmap[position.branch][position.level][r][c]);
		}
	}
	/* world map as the bot sees it */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 1 << ";82H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m";
			cout << (unsigned char) (dungeonmap[position.branch][position.level][r][c]);
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[m";
		}
	}
	/* path map */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";82H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (pathmap[r][c].move >= 'a' && pathmap[r][c].move <= 'z')
				cout << (unsigned char) pathmap[r][c].move;
			else
				cout << (unsigned char) (dungeonmap[position.branch][position.level][r][c]);
		}
	}
	/* return cursor back to where it was */
	cout << (unsigned char) 27 << "[" << world->cursor.row + 1 << ";" << world->cursor.col + 1 << "H";
	/* and flush cout. if we don't do this our output looks like garbage */
	cout.flush();
}

void Saiph::inspect() {
	/* notify the analyzers about changes */
	for (vector<Point>::iterator c = world->changes.begin(); c != world->changes.end(); ++c) {
		for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
			(*a)->inspect(*c);
	}
}

void Saiph::parseMessages() {
	/* parse messages that can help us find doors/staircases/etc. */
	if (world->messages.find(MESSAGE_STAIRCASE_UP_HERE, 0) != string::npos)
		dungeonmap[position.branch][position.level][world->player.row][world->player.col] = STAIRS_UP;
	else if (world->messages.find(MESSAGE_STAIRCASE_DOWN_HERE, 0) != string::npos)
		dungeonmap[position.branch][position.level][world->player.row][world->player.col] = STAIRS_DOWN;
	else if (world->messages.find(MESSAGE_OPEN_DOOR_HERE, 0) != string::npos)
		dungeonmap[position.branch][position.level][world->player.row][world->player.col] = OPEN_DOOR;
	else if (world->messages.find(MESSAGE_FOUNTAIN_HERE, 0) != string::npos)
		dungeonmap[position.branch][position.level][world->player.row][world->player.col] = FOUNTAIN;
	else if (world->messages.find(MESSAGE_FOUNTAIN_DRIES_UP, 0) != string::npos || world->messages.find(MESSAGE_FOUNTAIN_DRIES_UP2, 0) != string::npos)
		dungeonmap[position.branch][position.level][world->player.row][world->player.col] = FLOOR;
	/* when we've checked messages for static dungeon features and not found anything,
	 * then we can set the tile to UNKNOWN_TILE_DIAGONALLY_PASSABLE if the tile is UNKNOWN_TILE */
	else if (dungeonmap[position.branch][position.level][world->player.row][world->player.col] == UNKNOWN_TILE)
		dungeonmap[position.branch][position.level][world->player.row][world->player.col] = UNKNOWN_TILE_DIAGONALLY_PASSABLE;

	/* item parsing */
	/* figure out if there's something on the ground or if we're picking up something */
	string::size_type pos;
	if ((pos = world->messages.find(MESSAGE_YOU_SEE_HERE, 0)) != string::npos || (pos = world->messages.find(MESSAGE_YOU_FEEL_HERE, 0)) != string::npos) {
		/* single item on ground */
		clearStash(position);
		pos += sizeof (MESSAGE_YOU_SEE_HERE) - 1;
		string::size_type length = world->messages.find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			Item item(world->messages.substr(pos, length));
			addItemToStash(position, item);
		}
	} else if ((pos = world->messages.find(MESSAGE_THINGS_THAT_ARE_HERE, 0)) != string::npos) {
		/* multiple items on ground */
		clearStash(position);
		pos = world->messages.find("  ", pos + 1);
		while (pos != string::npos && world->messages.size() > pos + 2) {
			pos += 2;
			string::size_type length = world->messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			Item item(world->messages.substr(pos, length));
			addItemToStash(position, item);
			pos += length;
		}
	} else if (world->messages.find(MESSAGE_YOU_SEE_NO_OBJECTS, 0) != string::npos || world->messages.find(MESSAGE_THERE_IS_NOTHING_HERE, 0) != string::npos) {
		/* no items on ground */
		clearStash(position);
	} else if ((pos = world->messages.find(MESSAGE_PICK_UP_WHAT, 0)) != string::npos) {
		/* picking up stuff.
		 * we should clear the stash here too and update it */
		clearStash(position);
		pos = world->messages.find("  ", pos + 1);
		while (pos != string::npos && world->messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = world->messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (world->messages[pos - 2] == '-') {
				Item item(world->messages.substr(pos, length));
				addItemToPickup(world->messages[pos - 4], item);
				addItemToStash(position, item);
			}
			pos += length;
		}
	} else if (world->messages.find(MESSAGE_NOT_CARRYING_ANYTHING, 0) != string::npos || world->messages.find(MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD, 0) != string::npos) {
		/* our inventory is empty. how did that happen? */
		inventory.clear();
	} else if ((pos = world->messages.find(".  ", 0)) != string::npos) {
		/* when we pick up stuff we only get "  f - a lichen corpse.  " and similar.
		 * we'll need to handle this too somehow.
		 * we're searching for ".  " as we won't get that when we're listing inventory.
		 * also, this won't detect gold, but we might not need to detect that,
		 * well, it's gonna be a bit buggy when picking up gold from stashes */
		/* additionally, we'll assume we're picking up from the stash at this location.
		 * this will also trigger on wishes, but meh, probably not gonna be an issue */
		pos = 0;
		while ((pos = world->messages.find(" - ", pos)) != string::npos) {
			if (pos > 2 && world->messages[pos - 3] == ' ' && world->messages[pos - 2] == ' ') {
				unsigned char key = world->messages[pos - 1];
				pos += 3;
				string::size_type length = world->messages.find(".  ", pos);
				if (length == string::npos)
					break;
				length = length - pos;
				Item item(world->messages.substr(pos, length));
				addItemToInventory(key, item);
				removeItemFromPickup(item);
				removeItemFromStash(position, item);
				pos += length;
			} else {
				/* "Yak - dog food!" mess things up */
				++pos;
			}
		}
	} else if ((pos = world->messages.find(" - ", 0)) != string::npos) {
		/* we probably listed our inventory or the pickup list */
		inventory.clear();
		while ((pos = world->messages.find(" - ", pos)) != string::npos) {
			if (pos > 2 && world->messages[pos - 3] == ' ' && world->messages[pos - 2] == ' ') {
				unsigned char key = world->messages[pos - 1];
				pos += 3;
				string::size_type length = world->messages.find("  ", pos);
				if (length == string::npos)
					break;
				length = length - pos;
				Item item(world->messages.substr(pos, length));
				addItemToInventory(key, item);
				removeItemFromStash(position, item);
				pos += length;
			}
		}
	}
	/* we'll need to set the on_ground pointer here */
	if (stashes[position.branch][position.level].find(position) != stashes[position.branch][position.level].end())
		on_ground = &stashes[position.branch][position.level][position];
}

void Saiph::removeItemFromPickup(const Item &item) {
	/* we currently don't have a way to tell exactly which item we removed.
	 * we'll have to search and remove the best match */
	if (item.count <= 0)
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from pickup" << endl;
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

void Saiph::removeItemFromStash(const Point &point, const Item &item) {
	if (item.count <= 0)
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from stash at " << position.branch << ", " << position.level << ", " << point.row << ", " << point.col << endl;
	map<Point, Stash>::iterator s = stashes[position.branch][position.level].find(point);
	if (s != stashes[position.branch][position.level].end())
		s->second.removeItem(item);
}

void Saiph::updateMaps() {
	/* update the various maps */
	for (vector<Point>::iterator c = world->changes.begin(); c != world->changes.end(); ++c) {
		if (world->view[c->row][c->col] == SOLID_ROCK)
			continue; // not interesting (also mess up unlit rooms)
		if (dungeon[(unsigned char) world->view[c->row][c->col]]) {
			/* update the map showing static stuff */
			dungeonmap[position.branch][position.level][c->row][c->col] = world->view[c->row][c->col];
		} else if (!passable[dungeonmap[position.branch][position.level][c->row][c->col]]) {
			/* we can't see the floor here, but we believe we can pass this tile.
			 * place an UNKNOWN_TILE here.
			 * the reason we check if stored tile is !passable is because if we don't,
			 * then every tile a monster steps on or drops an item on will become UNKNOWN_TILE,
			 * even if we already know what's beneath the monster/item. */
			dungeonmap[position.branch][position.level][c->row][c->col] = UNKNOWN_TILE;
		}
		/* update items */
		if (!world->player.hallucinating) {
			if (item[(unsigned char) world->view[c->row][c->col]]) {
				map<Point, Stash>::iterator s = stashes[position.branch][position.level].find(*c);
				if (s != stashes[position.branch][position.level].end()) {
					if (s->second.top_symbol != world->view[c->row][c->col]) {
						/* top symbol changed, update */
						/* TODO: check color too */
						s->second.turn_changed = world->player.turn;
						s->second.top_symbol = world->view[c->row][c->col];
					}
				} else {
					/* new stash */
					stashes[position.branch][position.level][*c] = Stash(world->player.turn, world->view[c->row][c->col]);
				}
			} else if (world->view[c->row][c->col] == dungeonmap[position.branch][position.level][c->row][c->col]) {
				/* if there ever was a stash here, it's gone now */
				stashes[position.branch][position.level].erase(*c);
			}
		}

		/* update monsters */
		if (monster[(unsigned char) world->view[c->row][c->col]]) {
			/* add a monster, or update position of an existing monster */
			unsigned char symbol;
			if (world->color[c->row][c->col] == INVERSE)
				symbol = PET;
			else
				symbol = world->view[c->row][c->col];
			/* set monster on monstermap */
			monstermap[position.branch][position.level][c->row][c->col] = symbol;
			/* find nearest monster */
			int min_distance = INT_MAX;
			map<Point, Monster>::iterator nearest = monsters[position.branch][position.level].end();
			for (map<Point, Monster>::iterator m = monsters[position.branch][position.level].begin(); m != monsters[position.branch][position.level].end(); ++m) {
				if (m->second.symbol != symbol || m->second.color != world->color[c->row][c->col])
					continue; // not the same monster
				unsigned char old_symbol;
				if (world->color[m->first.row][m->first.col] == INVERSE)
					old_symbol = PET;
				else
					old_symbol = world->view[c->row][c->col];
				if (m->second.symbol == old_symbol && m->second.color == world->color[m->first.row][m->first.col])
					continue; // this monster already is on its square
				/* see if this monster is closer than the last found monster */
				int distance = max(abs(m->first.row - c->row), abs(m->first.col - c->col));
				if (distance >= min_distance)
					continue;
				/* it is */
				min_distance = distance;
				nearest = m;
			}
			if (nearest != monsters[position.branch][position.level].end()) {
				/* we know of this monster, move it to new location */
				/* remove monster from monstermap */
				monstermap[position.branch][position.level][nearest->first.row][nearest->first.col] = ILLEGAL_MONSTER;
				/* update monster */
				monsters[position.branch][position.level][*c] = nearest->second;
				monsters[position.branch][position.level].erase(nearest);
			} else {
				/* add monster */
				monsters[position.branch][position.level][*c] = Monster(symbol, world->color[c->row][c->col]);
			}
		}
	}
	/* remove monsters that seems to be gone
	 * and make monsters we can't see !visible */
	for (map<Point, Monster>::iterator m = monsters[position.branch][position.level].begin(); m != monsters[position.branch][position.level].end(); ) {
		unsigned char symbol;
		if (world->color[m->first.row][m->first.col] == INVERSE)
			symbol = PET;
		else
			symbol = world->view[m->first.row][m->first.col];
		/* if we don't see the monster on world->view then it's not visible */
		m->second.visible = (symbol == m->second.symbol && world->color[m->first.row][m->first.col] == m->second.color);
		if (abs(position.row - m->first.row) > 1 || abs(position.col - m->first.col) > 1) {
			/* player is not next to where we last saw the monster */
			++m;
			continue;
		}
		if (symbol == m->second.symbol && world->color[m->first.row][m->first.col] == m->second.color) {
			/* we can still see the monster */
			++m;
			continue;
		}
		/* remove monster from monstermap */
		monstermap[position.branch][position.level][m->first.row][m->first.col] = ILLEGAL_MONSTER;
		/* remove monster from list */
		monsters[position.branch][position.level].erase(m++);
	}
	/* update map used for pathing */
	updatePathMap();
}

void Saiph::updatePathMap() {
	/* first reset nextnode pointer, cost & move */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			pathmap[r][c].nextnode = NULL;
			pathmap[r][c].cost = UINT_MAX;
			pathmap[r][c].move = ILLEGAL_MOVE;
		}
	}
	Point from(world->player.row, world->player.col);
	pathing_queue[0] = from;
	pathmap[from.row][from.col].cost = 0;
	pathmap[from.row][from.col].move = REST;
	int curnode = 0;
	int nodes = 1;
	while (curnode < nodes) {
		from = pathing_queue[curnode++];
		/* check northwest node */
		Point to(from.row - 1, from.col - 1);
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_NW;
			pathing_queue[nodes++] = to;
		}
		/* check north node */
		++to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_N;
			pathing_queue[nodes++] = to;
		}
		/* check northeast node */
		++to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_NE;
			pathing_queue[nodes++] = to;
		}
		/* check east node */
		++to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_E;
			pathing_queue[nodes++] = to;
		}
		/* check southeast node */
		++to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_SE;
			pathing_queue[nodes++] = to;
		}
		/* check south node */
		--to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_S;
			pathing_queue[nodes++] = to;
		}
		/* check southwest node */
		--to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_SW;
			pathing_queue[nodes++] = to;
		}
		/* check west node */
		--to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_W;
			pathing_queue[nodes++] = to;
		}
	}
}

bool Saiph::updatePathMapHelper(const Point &to, const Point &from) {
	/* helper method for updatePathMap()
	 * return true if the move is legal and we should path further from this node */
	if (to.row < MAP_ROW_BEGIN || to.row > MAP_ROW_END || to.col < MAP_COL_BEGIN || to.col > MAP_COL_END)
		return false; // outside map
	unsigned char s = dungeonmap[position.branch][position.level][to.row][to.col];
	if (!passable[s])
		return false;
	unsigned char m = monstermap[position.branch][position.level][to.row][to.col];
	if (monster[m] && m != PET)
		return false; // can't path through monsters (except pets)
	bool cardinal_move = (to.row == from.row || to.col == from.col);
	if (!cardinal_move) {
		if (s == OPEN_DOOR || dungeonmap[position.branch][position.level][from.row][from.col] == OPEN_DOOR)
			return false; // diagonally in/out of door
		if (s == UNKNOWN_TILE || dungeonmap[position.branch][position.level][from.row][from.col] == UNKNOWN_TILE)
			return false; // don't know what tile this is, it may be a door. no diagonal movement
		unsigned char sc1 = dungeonmap[position.branch][position.level][to.row][from.col];
		unsigned char sc2 = dungeonmap[position.branch][position.level][from.row][to.col];
		if (!passable[sc1] && !passable[sc2]) {
			/* moving past two corners
			 * while we may pass two corners if we're not carrying too much we'll just ignore this.
			 * it's bound to cause issues */
			if (sc1 != BOULDER && sc2 != BOULDER)
				return false; // neither corner is a boulder, we may not pass
			else if (position.branch == BRANCH_SOKOBAN)
				return false; // in sokoban we can't pass by boulders diagonally
		}
		//if (polymorphed_to_grid_bug)
		//	return false;
		//if (rogue_level)
		//	return false; // level that is hard to parse. only allowing cardinal moves makes it easier
	}
	//if (blacklisted_move)
	//	return false;
	//if (s == LAVA && !levitating)
	//	return false;
	//if (s == WATER && (!levitating || !waterwalk))
	//	return false;
	unsigned int newcost = pathmap[from.row][from.col].cost + (cardinal_move ? COST_CARDINAL : COST_DIAGONAL);
	newcost += pathcost[s];
	newcost += pathcost[m];
	if (newcost < pathmap[to.row][to.col].cost) {
		pathmap[to.row][to.col].nextnode = &pathmap[from.row][from.col];
		pathmap[to.row][to.col].cost = newcost;
		return true;
	}
	return false;
}

/* main */
int main() {
	Saiph *saiph = new Saiph(CONNECTION_TELNET);
	//for (int a = 0; a < 200 && saiph->run(); ++a)
	//	;
	while (saiph->run())
		;
	saiph->debugfile << SAIPH_DEBUG_NAME << "Quitting gracefully" << endl;
	delete saiph;
}
