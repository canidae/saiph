#ifndef TILE_H
#define	TILE_H

#include "Point.h"

/* max searching on a point */
#define TILE_FULLY_SEARCHED 256

class Tile {
public:
	Tile();
	virtual ~Tile();

	unsigned int cost() const;
	unsigned char direction() const;
	unsigned int distance() const;
	unsigned char monster() const; // only character, most analyzers should use list of Monster from Level instead
	unsigned int search() const;
	unsigned int searchInc();
	unsigned char symbol() const;
	/* maybe private and friends of Level & World? */
	unsigned char direction(unsigned char direction);
	unsigned char monster(unsigned char monster);
	unsigned int search(unsigned int search);
	unsigned char symbol(unsigned char symbol);
	void updatePath(const Point& next, unsigned char direction, unsigned int distance, unsigned int cost);

private:
	unsigned char _symbol; // dungeon symbol on this tile
	unsigned char _monster; // monster symbol on this tile, used for pathing
	unsigned char _direction; // direction to move to get to this tile when on same level
	unsigned int _search; // how many times we've searched this tile
	unsigned int _distance; // distance to this tile when on same level
	unsigned int _cost; // cost to this tile when on same level
	unsigned int _updated; // last internal turn this node was updated, used for pathing
	Point _next; // the location of the next tile in the path towards the player
};
#endif
