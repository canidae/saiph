#ifndef MONSTER_H
#define MONSTER_H

#include "Globals.h"

namespace data {
	class Monster;
}

class Monster {
public:
	Monster(unsigned char symbol = ILLEGAL_MONSTER, int color = 0, int last_seen = -1);

	unsigned char symbol() const;
	unsigned char symbol(unsigned char symbol);
	int color() const;
	int color(int color);
	bool visible() const;
	bool visible(bool visible);
	int attitude() const;
	int attitude(int attitude);
	int lastSeen() const;
	int lastSeen(int last_seen);
	int lastMoved() const;
	int lastMoved(int last_moved);
	bool shopkeeper() const;
	bool shopkeeper(bool shopkeeper);
	bool priest() const;
	bool priest(bool priest);
	const data::Monster* data() const;
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
