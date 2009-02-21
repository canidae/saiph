#include "Dig.h"
#include "../Debug.h"
#include "../Globals.h"
#include "../Item.h"
#include "../Saiph.h"

using namespace std;

Dig::Dig(Saiph *saiph) : Analyzer("Dig"), saiph(saiph), digging_tool(ILLEGAL_ITEM), dig_direction(NOWHERE) {
}

void Dig::analyze() {
	if (priority >= PRIORITY_DIG_DOWN || digging_tool == ILLEGAL_ITEM)
		return;
	else if (saiph->levels[saiph->position.level].branch == BRANCH_SOKOBAN)
		return;
	else if (!freeWeaponHand())
		return;

	if (dig_locations.size() <= 0) {
		/* don't know where to dig, locate interesting dig locations */
		/* boulders */
		for (map<Point, int>::iterator b = saiph->levels[saiph->position.level].symbols[(unsigned char) BOULDER].begin(); b != saiph->levels[saiph->position.level].symbols[(unsigned char) BOULDER].end(); ++b)
			dig_locations.push_back(b->first);

		/* dig free UNPASSABLE FLOOR tiles */
		if (!saiph->levels[saiph->position.level].undiggable) {
			for (map<Point, int>::iterator b = saiph->levels[saiph->position.level].symbols[(unsigned char) FLOOR].begin(); b != saiph->levels[saiph->position.level].symbols[(unsigned char) FLOOR].end(); ++b) {
				const PathNode &node = saiph->shortestPath(b->first);
				if (node.cost != UNPASSABLE)
					continue; // can either walk on it or not reach it
				/* dig one of the N, W, E or S tile if we can reach it */
				const PathNode north = saiph->shortestPath(Point(b->first.row - 1, b->first.col));
				if (north.cost == UNPASSABLE) {
					dig_locations.push_back(Point(b->first.row - 1, b->first.col));
					continue;
				}
				const PathNode west = saiph->shortestPath(Point(b->first.row, b->first.col - 1));
				if (west.cost == UNPASSABLE) {
					dig_locations.push_back(Point(b->first.row, b->first.col - 1));
					continue;
				}
				const PathNode east = saiph->shortestPath(Point(b->first.row, b->first.col + 1));
				if (east.cost == UNPASSABLE) {
					dig_locations.push_back(Point(b->first.row, b->first.col + 1));
					continue;
				}
				const PathNode south = saiph->shortestPath(Point(b->first.row + 1, b->first.col));
				if (south.cost == UNPASSABLE) {
					dig_locations.push_back(Point(b->first.row + 1, b->first.col));
					continue;
				}
			}
		}
	}

	/* dig nearest dig_location */
	unsigned int least_moves = UNREACHABLE;
	for (list<Point>::iterator d = dig_locations.begin(); d != dig_locations.end(); ) {
		const PathNode &node = saiph->shortestPath(*d);
		if (node.cost == UNREACHABLE || node.moves > least_moves) {
			++d;
			continue;
		}
		if (node.moves == 1) {
			/* next to location we want to dig, but do we still want to dig here? */
			unsigned char symbol = saiph->getDungeonSymbol(*d);
			if (symbol == BOULDER || symbol == VERTICAL_WALL || symbol == HORIZONTAL_WALL || symbol == SOLID_ROCK) {
				/* yes, we do */
				dig_direction = node.dir;
				command = APPLY;
				last_dig_target = *d;
				last_dig_location = saiph->position;
			} else {
				/* no, we can no longer dig on this location */
				d = dig_locations.erase(d);
				continue;
			}
		} else {
			/* move towards location to dig */
			command = node.dir;
		}
		priority = PRIORITY_DIG_PATH;
		least_moves = node.moves;
		++d;
	}

	unsigned char symbol = saiph->getDungeonSymbol();
	if (!saiph->levels[saiph->position.level].undiggable && canDigDownTile(symbol) && least_moves == UNREACHABLE) {
		/* no place to dig, dig down instead */
		dig_direction = DOWN;
		last_dig_location = saiph->position;
		last_dig_target = (Point)saiph->position;
		command = APPLY;
		priority = PRIORITY_DIG_DOWN;
	}
}

inline bool Dig::canDigDownTile(unsigned char symbol) {
	return symbol != SHOP_TILE &&
		symbol != OPEN_DOOR &&
		symbol != STAIRS_DOWN &&
		symbol != STAIRS_UP &&
		symbol != ALTAR &&
		symbol != THRONE;
}

void Dig::parseMessages(const string &messages) {
	if (saiph->inventory_changed)
		findDiggingTool();
	if (priority >= PRIORITY_DIG_PATH || dig_direction == NOWHERE)
		return;
	if (messages.find(MESSAGE_WHAT_TO_APPLY) != string::npos) {
		command = digging_tool;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_DIG_DIRECTION) != string::npos) {
		command = dig_direction;
		priority = PRIORITY_CONTINUE_ACTION;
		dig_direction = NOWHERE;
		/* tell loot analyzer to check inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (last_dig_location == saiph->position &&
			last_dig_target != (Point)last_dig_location &&
 			(messages.find(DIG_TOO_HARD) != string::npos ||
			messages.find(DIG_NOT_ENOUGH_ROOM) != string::npos)) {
		/* our target is undiggable */
		saiph->setDungeonSymbol(last_dig_target, UNKNOWN_TILE_UNPASSABLE);
	}
}

/* private methods */
void Dig::findDiggingTool() {
	map<unsigned char, Item>::iterator i = saiph->inventory.find(digging_tool);
	if (i != saiph->inventory.end() && isDiggingTool(i->second))
		return;
	for (i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (isDiggingTool(i->second)) {
			digging_tool = i->first;
			return;
		}
	}
	digging_tool = ILLEGAL_ITEM;
}

bool Dig::freeWeaponHand() {
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.additional.find("weapon in ", 0) == 0 || i->second.additional == "wielded") {
			return !(i->second.beatitude == CURSED);
		}
	}
	return true; //not wielding anything
}

bool Dig::isDiggingTool(const Item &item) {
	if (item.beatitude == CURSED || item.name != "pick-axe")
		return false;
	if (item.beatitude == BEATITUDE_UNKNOWN) {
		/* must beatify digging_tool first */
		req.request = REQUEST_BEATIFY_ITEMS;
		saiph->request(req);
		return false;
	}
	return true;
}                                                                                                                                                       
