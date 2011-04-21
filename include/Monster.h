#ifndef MONSTER_H
#define MONSTER_H

#include "Globals.h"

namespace data {
	class Monster;
}

class Monster {
public:
	Monster(unsigned char symbol = ILLEGAL_MONSTER, int color = 0, int last_seen = -1);

	unsigned char symbol() const { return _symbol; }
	unsigned char symbol(unsigned char symbol);
	int color() const { return _color; }
	int color(int color);
	bool visible() const { return _visible; }
	bool visible(bool visible);
	int attitude() const { return _attitude; }
	int attitude(int attitude);
	int lastSeen() const { return _last_seen; }
	int lastSeen(int last_seen);
	int lastMoved() const { return _last_moved; }
	int lastMoved(int last_moved);
	bool shopkeeper() const { return _shopkeeper; }
	bool shopkeeper(bool shopkeeper);
	bool priest() const { return _priest; }
	bool priest(bool priest);
	const data::Monster* data() const { return _data; }
	const data::Monster* data(const data::Monster* data);

private:
	unsigned char _symbol;
	int _color;
	bool _visible;
	int _attitude;
	int _last_seen;
	int _last_moved;
	bool _shopkeeper;
	bool _priest;
	const data::Monster* _data;
};
#endif
