#include "Analyzers/MonsterInfo.h"

#include <stdlib.h>
#include "Debug.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/FarLook.h"
#include "Data/Monster.h"

using namespace analyzer;
using namespace std;

/* public constructors */
MonsterInfo::MonsterInfo() : Analyzer("MonsterInfo") {
}

/* methods */
void MonsterInfo::analyze() {
	if (Saiph::hallucinating())
		return; // if we're hallucinating, the output is garbage
	_requests.clear();
	for (map<Point, Monster>::const_iterator look_at = World::level().monsters().begin(); look_at != World::level().monsters().end(); ++look_at) {
		if (!look_at->second.visible())
			continue; // don't farlook monsters we can't see
		if (look_at->second.symbol() == 'I' || (look_at->second.symbol() == 'm' && look_at->second.color() == BLUE))
			continue; // don't farlook 'I' or 'm' monsters
		map<Point, unsigned int>::iterator c = _checked.find(look_at->first);
		if (c != _checked.end() && c->second == World::internalTurn())
			continue; // already checked this monster this turn
		_requests.push_back(action::FarLook::Request(look_at->first));
	}
	if (!_requests.empty())
		World::setAction(new action::FarLook(this, _requests));
}

void MonsterInfo::actionCompleted(const string&) {
	for (vector<action::FarLook::Request>::iterator looked_at = _requests.begin(); looked_at != _requests.end(); ++looked_at) {
		_checked[looked_at->where] = World::internalTurn();
		string messages = looked_at->result;

		map<Point, Monster>::const_iterator pmonster = World::level().monsters().find(looked_at->where);

		if (!(pmonster != World::level().monsters().end() && messages[2] != ' ' && messages[3] == ' ' && messages[4] == ' ' && messages[5] == ' ')) {
			Debug::warning() << "Bogus farlook result " << messages << endl;
			continue;
		}

		string::size_type pos = string::npos;
		Monster monster = pmonster->second;
		if ((pos = messages.find(" (peaceful ")) != string::npos) {
			/* it's friendly */
			monster.attitude(FRIENDLY);
			pos += sizeof (" (peaceful ") - 1;
		} else if ((pos = messages.find(" (")) != string::npos) {
			/* hostile */
			if (messages.find(" (Oracle", pos) == pos)
				monster.attitude(FRIENDLY); // never attack oracle
			else
				monster.attitude(HOSTILE);
			pos += sizeof (" (") - 1;
		}
		if (pos != string::npos && pos < messages.size() && monster.symbol() == '@' && monster.color() == BOLD_WHITE && messages[pos] >= 'A' && messages[pos] <= 'Z')
			monster.shopkeeper(true); // shopkeepers are always white @, and their names are capitalized
		else
			monster.shopkeeper(false);
		if (messages.find("priest of ", pos) != string::npos || messages.find("priestess of ", pos) != string::npos)
			monster.priest(true);
		else
			monster.priest(false);
		string::size_type pos2 = messages.find(" - ", pos);
		if (pos2 == string::npos)
			pos2 = messages.find(")", pos);
		if (pos2 != string::npos)
			monster.data(data::Monster::monster(messages.substr(pos, pos2 - pos)));
		/* update level */
		World::level().setMonster(looked_at->where, monster);
	}
}
