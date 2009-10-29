#ifndef TILE_H
#define	TILE_H

#include "Coordinate.h"
#include "Point.h"

/* max searching on a point */
#define TILE_FULLY_SEARCHED 256

class Tile {
public:
	Tile(const Coordinate& coordinate = Coordinate());
	virtual ~Tile();

	const Coordinate& coordinate() const;
	const unsigned int& cost();
	const unsigned char& direction();
	const unsigned char& direction(const unsigned char& direction);
	const unsigned int& distance();
	const unsigned char& monster() const; // only character, most analyzers should use list of Monster from Level instead
	const unsigned char& monster(const unsigned char& monster);
	const unsigned int& search() const;
	const unsigned int& search(const unsigned int& search);
	const unsigned int& searchInc();
	const unsigned char& symbol() const;
	const unsigned char& symbol(const unsigned char& symbol);
	void updatePath(const Point& next, const unsigned char& direction, const unsigned int& distance, const unsigned int& cost);

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
	Point _next; // the location of the next tile in the path towards the player
};

std::ostream & operator<<(std::ostream& os, Tile& t);
#endif
