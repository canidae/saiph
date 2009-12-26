#ifndef TILE_H
#define	TILE_H

#include "Coordinate.h"

/* max searching on a point */
#define TILE_FULLY_SEARCHED 256

class Tile {
public:
	Tile(const Coordinate& coordinate = Coordinate());
	virtual ~Tile();

	const Coordinate& coordinate() const;
	unsigned int cost();
	unsigned char direction();
	unsigned char direction(unsigned char direction);
	unsigned int distance();
	unsigned char monster() const; // only character, most analyzers should use list of Monster from Level instead
	unsigned char monster(unsigned char monster);
	unsigned int search() const;
	unsigned int search(unsigned int search);
	unsigned int searchInc(int count = 1);
	unsigned char symbol() const;
	unsigned char symbol(unsigned char symbol);
	void updatePath(unsigned char direction, unsigned int distance, unsigned int cost);

private:
	static unsigned int _unreachable;
	static unsigned char _illegal_direction;
	Coordinate _coordinate; // where this tile is located
	unsigned char _symbol; // dungeon symbol on this tile
	unsigned char _monster; // monster symbol on this tile, used for pathing
	unsigned char _direction; // direction to move to get to this tile when on same level
	unsigned int _search; // how many times we've searched this tile
	unsigned int _distance; // distance to this tile when on same level
	unsigned int _cost; // cost to this tile when on same level
	unsigned int _updated; // last internal turn this node was updated, used for pathing
};

std::ostream & operator<<(std::ostream& os, Tile& t);
#endif
