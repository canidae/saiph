#ifndef MONSTER_H
#define MONSTER_H

#include "Globals.h"

namespace data {
	class Monster;
}

class Monster {
public:
	Monster(const unsigned char& symbol = ILLEGAL_MONSTER, const int& color = 0, const int& last_seen = -1);

	const unsigned char& symbol() const;
	const unsigned char& symbol(const unsigned char& symbol);
	const int& color() const;
	const int& color(const int& color);
	const bool& visible() const;
	const bool& visible(const bool& visible);
	const int& attitude() const;
	const int& attitude(const int& attitude);
	const int& lastSeen() const;
	const int& lastSeen(const int& last_seen);
	const bool& shopkeeper() const;
	const bool& shopkeeper(const bool& shopkeeper);
	const bool& priest() const;
	const bool& priest(const bool& priest);
	const data::Monster* data() const;
	const data::Monster* data(const data::Monster* data);

private:
	unsigned char _symbol;
	int _color;
	bool _visible;
	int _attitude;
	int _last_seen;
	bool _shopkeeper;
	bool _priest;
	const data::Monster* _data;
};
#endif
