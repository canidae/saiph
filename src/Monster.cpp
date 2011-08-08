#include "Monster.h"
#include "Data/Monster.h"

#include <sstream>

/* static data */
int Monster::_next_id;
std::map<std::string, Monster*> Monster::_by_id;
std::multimap<int, Monster*> Monster::_by_last_seen;

/* constructors/destructor */
Monster::Monster(const std::string& id) : _id(id), _symbol(ILLEGAL_MONSTER), _color(BLACK), _visible(false), _attitude(ATTITUDE_UNKNOWN), _last_seen(0), _last_moved(0), _last_seen_pos(), _shopkeeper(false), _priest(false), _data(0) {
	if (id.empty()) {
		int newid = _next_id++;
		std::ostringstream buf;
		buf << newid;
		_id = buf.str();
	}
	index();
}

/* methods */
unsigned char Monster::symbol() const {
	return _symbol;
}

unsigned char Monster::symbol(unsigned char symbol) {
	_symbol = symbol;
	return this->symbol();
}

int Monster::color() const {
	return _color;
}

int Monster::color(int color) {
	_color = color;
	return this->color();
}

bool Monster::visible() const {
	return _visible;
}

bool Monster::visible(bool visible) {
	_visible = visible;
	return this->visible();
}

int Monster::attitude() const {
	return _attitude;
}

int Monster::attitude(int attitude) {
	_attitude = attitude;
	return this->attitude();
}

Coordinate Monster::lastSeenPos() const {
	return _last_seen_pos;
}

void Monster::lastSeenPos(const Coordinate& last_seen_pos) {
	_last_seen_pos = last_seen_pos;
}

int Monster::lastSeen() const {
	return _last_seen;
}

int Monster::lastSeen(int last_seen) {
	unindex();
	_last_seen = last_seen;
	index();
	return this->lastSeen();
}

int Monster::lastMoved() const {
	return _last_moved;
}

int Monster::lastMoved(int last_moved) {
	_last_moved = last_moved;
	return this->lastMoved();
}

bool Monster::shopkeeper() const {
	return _shopkeeper;
}

bool Monster::shopkeeper(bool shopkeeper) {
	_shopkeeper = shopkeeper;
	return this->shopkeeper();
}

bool Monster::priest() const {
	return _priest;
}

bool Monster::priest(bool priest) {
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

std::map<std::string, Monster*>& Monster::byID() {
	return _by_id;
}

std::multimap<int, Monster*>& Monster::byLastSeen() {
	return _by_last_seen;
}

void Monster::index() {
	_by_id.insert(std::make_pair(_id, this));
	_by_last_seen.insert(std::make_pair(_last_seen, this));
}

void Monster::unindex() {
	_by_id.erase(_id);
	std::multimap<int, Monster*>::iterator i = _by_last_seen.find(_last_seen);
	while (i != _by_last_seen.end() && i->second != this) ++i;
	if (i != _by_last_seen.end())
		_by_last_seen.erase(i);
}
