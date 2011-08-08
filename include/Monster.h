#ifndef MONSTER_H
#define MONSTER_H

#include "Globals.h"
#include "Coordinate.h"

#include <string>
#include <map>

namespace data {
	class Monster;
}

class Monster {
public:
	Monster(const std::string& id);

	const std::string& id() const;
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
	Coordinate lastSeenPos() const;
	void lastSeenPos(const Coordinate& in);
	bool called() const;
	bool called(bool called);
	bool shopkeeper() const;
	bool shopkeeper(bool shopkeeper);
	bool priest() const;
	bool priest(bool priest);
	const data::Monster* data() const;
	const data::Monster* data(const data::Monster* data);

	static std::map<std::string, Monster*>& byID();
	static std::multimap<int, Monster*>& byLastSeen();

private:
	// uniquely identifies this monster.  if G_UNIQ, will be the monster's own identity,
	// either as a typename or a shkname, otherwise will be an integer.
	std::string _id;
	unsigned char _symbol;
	int _color;
	bool _visible;
	int _attitude;
	int _last_seen;
	int _last_moved;
	Coordinate _last_seen_pos;
	// has the monster been 'C'alled with its _id?  always true for G_UNIQ
	bool _called;
	bool _shopkeeper;
	bool _priest;
	const data::Monster* _data;

	void index();
	void unindex();

	static int _next_id;
	// all Monster objects in the game are owned by this map; everyone else just holds pointers
	// XXX: Level probably shouldn't be holding pointers.  Probably code which uses Level's access should use our maps instead
	static std::map<std::string, Monster*> _by_id;
	static std::multimap<int, Monster*> _by_last_seen;
};
#endif
