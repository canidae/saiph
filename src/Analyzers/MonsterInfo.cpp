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
	for (_look_at = World::level().monsters().begin(); _look_at != World::level().monsters().end(); ++_look_at) {
		if (!_look_at->second.visible())
			continue; // don't farlook monsters we can't see
		if (_look_at->second.symbol() == 'I' || _look_at->second.symbol() == 'm')
			continue; // don't farlook 'I' or 'm' monsters
		if (_look_at->second.attitude() == HOSTILE)
			continue; // we don't expect hostile monsters to go friendly (XXX: scroll of taming, etc will need special handling)
		map<Point, unsigned int>::iterator c = _checked.find(_look_at->first);
		if (c != _checked.end() && c->second == World::internalTurn())
			continue; // already checked this monster this turn
		World::setAction(static_cast<action::Action*> (new action::FarLook(this, _look_at->first)));
		return;
	}
}

void MonsterInfo::parseMessages(const string& messages) {
	if (_look_at != World::level().monsters().end() && messages.size() > 5 && messages[2] != ' ' && messages[3] == ' ' && messages[4] == ' ' && messages[5] == ' ') {
		/* probably looked at a monster */
		string::size_type pos = string::npos;
		Monster monster = _look_at->second;
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
		World::level().setMonster(_look_at->first, monster);
	}
}

void MonsterInfo::actionCompleted() {
	_checked[_look_at->first] = World::internalTurn();
}
