#ifndef LEVEL_H
#define LEVEL_H

#include <limits.h>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "Globals.h"
#include "Point.h"
#include "Stash.h"
#include "Tile.h"

/* max amount of nodes in pathing_queue */
#define PATHING_QUEUE_SIZE 16384
/* Fight needs to be able to subtract this */
#define COST_MONSTER 256

namespace data {
	class Monster;
}

class Item;
class Monster;

class Level {
public:
	Level(int level, const std::string &name, int branch = BRANCH_MAIN);

	static bool isPassable(unsigned char symbol);
	static const Tile& outsideMap();
	static void init();
	static void destroy();
	int branch() const;
	int branch(int branch);
	int depth() const;
	const std::string& name() const;
	Tile& tile();
	Tile& tile(const Point& point);
	const std::map<Point, Monster*>& monsters() const;
	const std::map<Point, Stash>& stashes() const;
	const std::map<Point, int>& symbols(unsigned char symbol) const;
	bool isCompletelyOpen() const;
	void analyze();
	int identifier() const;
	void parseMessages(const std::string& messages);
	void setDirtyStash(const Point& point);
	void setDungeonSymbol(const Point& point, unsigned char symbol);
	void setDungeonSymbolValue(const Point& point, int value);
	static void setFarlookResults(const std::map<Point, std::string>& farlooks);
	static void clearFarlookData();
	std::vector<Point> farlooksNeeded();
	void increaseAdjacentSearchCount(const Point& point, int count = 1);

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
	static Tile _outside_map;
	static std::map<Point,std::string> _turn_farlooks;
	static unsigned _farlooked_turn;
	static Coordinate _prev_position;
	int _level;
	Tile _map[MAP_ROW_END + 1][MAP_COL_END + 1];
	std::map<Point, Monster*> _monsters;
	std::set<Point> _monster_points;
	std::map<Point, Stash> _stashes;
	std::map<Point, int> _symbols[UCHAR_MAX + 1];
	std::string _name;
	int _depth;
	int _branch;
	bool _walls_diggable;
	bool _floor_diggable;
	bool _new_level;

	void updateMapPoint(const Point& point, unsigned char symbol, int color);
	void updateLight(const Point& point);
	void updateMonsters();
	bool parseFarlook(Point c, bool& shopkeeper, bool& priest, int& attitude, std::string& name, const data::Monster*& data);
	void updatePathMap();
	void updatePathMapSetCost(const Point& to, const Tile& prev);
	unsigned int updatePathMapCalculateCost(const Tile& next, const Tile& prev);
};
#endif
