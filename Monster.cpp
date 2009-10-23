#include "Monster.h"
#include "Data/Monster.h"

/* constructors/destructor */
Monster::Monster(const unsigned char& symbol, const int& color, const int& last_seen) : _symbol(symbol), _color(color), _visible(false), _attitude(ATTITUDE_UNKNOWN), _last_seen(last_seen), _shopkeeper(false), _priest(false), _data(data::Monster::getMonsterData(symbol, color)) {
}

/* methods */
const unsigned char& Monster::symbol() const {
	return _symbol;
}

const unsigned char& Monster::symbol(const unsigned char& symbol) {
	_symbol = symbol;
	return this->symbol();
}

const int& Monster::color() const {
	return _color;
}

const int& Monster::color(const int& color) {
	_color = color;
	return this->color();
}

const bool& Monster::visible() const {
	return _visible;
}

const bool& Monster::visible(const bool& visible) {
	_visible = visible;
	return this->visible();
}

const int& Monster::attitude() const {
	return _attitude;
}

const int& Monster::attitude(const int& attitude) {
	_attitude = attitude;
	return this->attitude();
}

const int& Monster::lastSeen() const {
	return _last_seen;
}

const int& Monster::lastSeen(const int& last_seen) {
	_last_seen = last_seen;
	return this->lastSeen();
}

const bool& Monster::shopkeeper() const {
	return _shopkeeper;
}

const bool& Monster::shopkeeper(const bool& shopkeeper) {
	_shopkeeper = shopkeeper;
	return this->shopkeeper();
}

const bool& Monster::priest() const {
	return _priest;
}

const bool& Monster::priest(const bool& priest) {
	_priest = priest;
	return this->priest();
}

const data::Monster* Monster::data() const {
	return _data;
}

const data::Monster* Monster::data(const data::Monster* data) {
	_data = data;
	return this->data();
}