#include "Monster.h"
#include "World.h"
#include "Data/Monster.h"
#include "Data/Polearm.h"
#include "Debug.h"

#include <sstream>
#include <cctype>

using namespace std;

/* static data */
int Monster::_next_id;
std::map<std::string, Monster*> Monster::_by_id;
std::multimap<int, Monster*> Monster::_by_last_seen;

/* constructors/destructor */
Monster::Monster(const std::string& id) : _id(id), _symbol(ILLEGAL_MONSTER), _color(BLACK), _visible(false), _attitude(ATTITUDE_UNKNOWN), _last_seen(0), _last_moved(0), _observed_turn(-2), _observed_subturn(0), _last_seen_pos(), _ranged(0), _called(false), _shopkeeper(false), _priest(false), _data(0) {
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
	if (!data())
		return 5; // hasted air E... gotta be pessimistic
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

int Monster::ranged() const {
	return _ranged;
}

void Monster::addRanged(int what) {
	Debug::info() << id() << ": adding ranged type " << (what == RANGED_WAND ? "wand" : what == RANGED_POLEARM ? "polearm" : "missile") << endl;
	_ranged |= what;
}

bool Monster::removeAdj(string& from, const string& adj) {
	string cap_adj = adj;
	cap_adj[0] = toupper(cap_adj[0]);

	if (from.size() <= adj.size() || from[adj.size()] != ' ')
		return false;
	if (from[0] != adj[0] && from[0] != toupper(adj[0]))
		return false;
	if (from.compare(1, adj.size() - 1, adj, 1, adj.size() - 1))
		return false;

	from.erase(0, adj.size() + 1);
	return true;
}

// handles called=false strings
Monster* Monster::parseMonster(const string& str) {
	string head = str;
	if (head == "It")
		return 0;
	if (Saiph::hallucinating())
		return 0;
	// these are actually handled at the end of x_monnam
	removeAdj(head, "the");
	removeAdj(head, "your");
	removeAdj(head, "a");
	removeAdj(head, "an");
	// TODO: priests/minions special code
	// TODO: !hallu shopkeepers special code
	// I wonder if it would be better to combine these
	removeAdj(head, "falling");
	removeAdj(head, "poor");
	removeAdj(head, "angry");
	removeAdj(head, "plain");
	removeAdj(head, "beautiful");
	removeAdj(head, "blind");
	removeAdj(head, "bite-covered");

	removeAdj(head, "invisible");
	removeAdj(head, "saddled");
	// we assume all monsters that can be renamed, have been.  so no dealing with funky mplayer cases
	string id;
	if (head.size() > 8 && head.substr(head.size() - 8) == "'s ghost")
		id = head.substr(0, head.size() - 8);
	else
		id = head; // is either a call string, or the name of a unique mob
	map<string, Monster*>::const_iterator mi = _by_id.find(id);
	if (mi != _by_id.end())
		return mi->second;
	else
		return 0;
}

// Hack - we don't (can't?) completely parse monster items in messages, so just look for polearm names
bool Monster::checkPolearm(const string& what) {
	for (map<const string, const data::Polearm*>::const_iterator i = data::Polearm::polearms().begin(); i != data::Polearm::polearms().end(); ++i) {
		if (what.find(i->first) != string::npos)
			return true;
	}
	return false;
}

void Monster::parseMessages(const string& messages) {
	string::size_type nextpos;
	for (string::size_type pos = messages.find("  "); pos != string::npos; pos = nextpos) {
		nextpos = messages.find("  ", pos + 2);
		string message = messages.substr(pos + 2, nextpos - (pos + 2));

		Monster* mob;
		string::size_type breakpt;

		if ((breakpt = message.find(" thrusts ")) != string::npos && checkPolearm(message.substr(breakpt + sizeof(" thrusts ") - 1)) && (mob = parseMonster(message.substr(0, breakpt)))) {
			mob->addRanged(RANGED_POLEARM);
			continue;
		}

		if ((breakpt = min(message.find(" throws "), message.find(" shoots "))) != string::npos && (mob = parseMonster(message.substr(0, breakpt)))) {
			mob->addRanged(RANGED_MISSILES);
			continue;
		}

		// ignore %s zaps %sself with %s!
		if ((breakpt = message.find(" zaps a")) != string::npos && (mob = parseMonster(message.substr(0, breakpt)))) {
			mob->addRanged(RANGED_WAND);
			continue;
		}
	}
}
