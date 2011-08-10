#include "Monster.h"
#include "World.h"
#include "Data/Monster.h"
#include "Debug.h"

#include <sstream>

/* static data */
int Monster::_next_id;
std::map<std::string, Monster*> Monster::_by_id;
std::multimap<int, Monster*> Monster::_by_last_seen;

/* constructors/destructor */
Monster::Monster(const std::string& id) : _id(id), _symbol(ILLEGAL_MONSTER), _color(BLACK), _visible(false), _attitude(ATTITUDE_UNKNOWN), _last_seen(0), _last_moved(0), _observed_turn(-2), _observed_subturn(0), _last_seen_pos(), _called(false), _shopkeeper(false), _priest(false), _data(0) {
	if (id.empty()) {
		int newid = _next_id++;
		std::ostringstream buf;
		buf << newid;
		_id = buf.str();
	}
	std::fill(_movehist, _movehist + MOVE_MEMORY, 0);
	index();
}

/* methods */
const std::string& Monster::id() const {
	return _id;
}

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

int Monster::maxMovesThisTurn() const {
	if (!data()) return 5; // hasted air E... gotta be pessimistic
	int speed = data()->moveRate();
	if (_observed_turn != World::turn())
		return (speed + 11) / 12; // why are you interested in whether an invisible monster can move?

	// we're interested in energy at the END of turns
	int max_en = 11;

	// note, _movehist values are lower limits, so we cannot calculate min energy
	for (int turns_ago = MOVE_MEMORY - 1; turns_ago >= 1; --turns_ago) {
		max_en = std::min(max_en, 11);
		max_en += speed;
		max_en -= _movehist[turns_ago] * 12;
		if (max_en < 0) {
			Debug::warning() << "Monster " << symbol() << id() << " is moving anomalously fast" << std::endl;
			return (((4 * speed + 2) / 3) + 11) / 12; // assume it's hasted
		}
	}

	max_en = std::min(max_en, 11);
	max_en += speed; // for the beginning of turn now-0
	return max_en / 12;
}

int Monster::observedTurn() const {
	return _observed_turn;
}

void Monster::observed(const Coordinate& point) {
	unindex();

	// maintain the invariant _movehist[N] corresponds to _observed_turn - N
	int shift = World::turn() - _observed_turn;
	if (shift < MOVE_MEMORY)
		std::copy_backward(_movehist, _movehist + MOVE_MEMORY - shift, _movehist + MOVE_MEMORY);
	std::fill(_movehist, _movehist + std::min(MOVE_MEMORY, shift), 0);

	// now add in observations of current moves (erring low)
	int mdist = _last_seen_pos.level() == point.level() ? Point::gridDistance(_last_seen_pos, point) : 0;
	if (World::turn() == (_observed_turn + 1)) {
		// Monster cannot have moved more than subturn() times this turn, so it must have moved prdist times between the last observation and the end of the last turn
		int prdist = std::max(0, mdist - World::subTurn());
		// All moves in prdist must belong to the previous turn, and they must come after the last observation; beware the 0 case
		if (prdist > 0)
			_movehist[1] = std::max(_movehist[1], _observed_subturn + prdist);
	} else if (World::turn() == _observed_turn) {
		// All moves must have occurred during the current turn
		if (mdist > 0)
			_movehist[0] = std::max(_movehist[0], _observed_subturn + mdist);
	} else {
		// We cannot safely assign the turns
	}
	_observed_turn = World::turn();
	_observed_subturn = World::subTurn();

	std::ostringstream buf;
	for (int i = 0; i < MOVE_MEMORY; ++i)
		buf << ' ' << _movehist[i];
	buf << " / predicted max for current turn = " << maxMovesThisTurn();
	Debug::info() << "Monster " << symbol() << id() << " move history:" << buf.str() << std::endl;

	if (point != _last_seen_pos) {
		_last_moved = World::internalTurn();
	}
	_last_seen_pos = point;
	_last_seen = World::internalTurn();

	index();
}

int Monster::lastSeen() const {
	return _last_seen;
}

int Monster::lastMoved() const {
	return _last_moved;
}

bool Monster::called() const {
	return _called;
}

bool Monster::called(bool called) {
	_called = called;
	return this->called();
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
