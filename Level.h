#ifndef LEVEL_H
#define LEVEL_H

#include <limits.h>
#include <map>
#include <string>
#include "Globals.h"
#include "Monster.h"
#include "Point.h"
#include "Stash.h"
#include "Tile.h"
#include "Events/ItemsOnGround.h"
#include "Events/ReceivedItems.h"

/* max amount of nodes in pathing_queue */
#define PATHING_QUEUE_SIZE 16384

class Item;

class Level {
public:
	Level(const int& level, const std::string &name, const int& branch = BRANCH_MAIN);

	static const bool& isPassable(const unsigned char& symbol);
	static const Tile& outsideMap();
	static void init();
	static void destroy();
	const int& branch() const;
	const int& branch(const int& branch);
	const int& depth() const;
	const std::string& name() const;
	Tile& tile();
	Tile& tile(const Point& point);
	std::map<Point, Monster>& monsters();
	std::map<Point, Stash>& stashes();
	std::map<Point, int>& symbols(const unsigned char& symbol);
	void analyze();
	void parseMessages(const std::string& messages);
	void setDungeonSymbol(const Point& point, const unsigned char& symbol);
	void increaseAdjacentSearchCount(const Point& point);

private:
	static Point _pathing_queue[PATHING_QUEUE_SIZE];
	static int _pathing_queue_size;
	static unsigned char _uniquemap[UCHAR_MAX + 1][CHAR_MAX + 1];
	static int _pathcost[UCHAR_MAX + 1];
	static bool _passable[UCHAR_MAX + 1];
	static bool _dungeon[UCHAR_MAX + 1];
	static bool _monster[UCHAR_MAX + 1];
	static bool _item[UCHAR_MAX + 1];
	static bool _got_pickup_menu;
	static bool _got_drop_menu;
	static event::ReceivedItems _received;
	static event::ItemsOnGround _on_ground;
	static Tile _outside_map;
	int _level;
	Tile _map[MAP_ROW_END + 1][MAP_COL_END + 1];
	std::map<Point, Monster> _monsters;
	std::map<Point, Stash> _stashes;
	std::map<Point, int> _symbols[UCHAR_MAX + 1];
	std::string _name;
	int _depth;
	int _branch;
	bool _walls_diggable;
	bool _floor_diggable;

	void updateMapPoint(const Point& point, unsigned char symbol, const int& color);
	void updateMonsters();
	void updatePathMap();
	unsigned int updatePathMapCalculateCost(const Point& to, const Point& from);
	void updatePathMapSetCost(const Point& to, const Point& from, const unsigned char& direction, const unsigned int& distance);
};
#endif
