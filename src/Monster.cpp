#include "Monster.h"
#include "Data/Monster.h"

/* constructors/destructor */
Monster::Monster(unsigned char symbol, int color, int last_seen) : _symbol(symbol), _color(color), _visible(false), _attitude(ATTITUDE_UNKNOWN), _last_seen(last_seen), _last_moved(last_seen), _shopkeeper(false), _priest(false), _data(data::Monster::monster(symbol, color)) {
}

/* methods */
unsigned char Monster::symbol(unsigned char symbol) {
	_symbol = symbol;
	return this->symbol();
}

int Monster::color(int color) {
	_color = color;
	return this->color();
}

bool Monster::visible(bool visible) {
	_visible = visible;
	return this->visible();
}

int Monster::attitude(int attitude) {
	_attitude = attitude;
	return this->attitude();
}

int Monster::lastSeen(int last_seen) {
	_last_seen = last_seen;
	return this->lastSeen();
}

int Monster::lastMoved(int last_moved) {
	_last_moved = last_moved;
	return this->lastMoved();
}

bool Monster::shopkeeper(bool shopkeeper) {
	_shopkeeper = shopkeeper;
	return this->shopkeeper();
}

bool Monster::priest(bool priest) {
	_priest = priest;
	return this->priest();
}

const data::Monster* Monster::data(const data::Monster* data) {
	_data = data;
	return this->data();
}
