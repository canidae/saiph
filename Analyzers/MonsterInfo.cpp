#include "MonsterInfo.h"

#include <stdlib.h>
#include "../Debug.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/FarLook.h"
#include "../Data/Monster.h"

using namespace analyzer;
using namespace std;

/* public constructors */
MonsterInfo::MonsterInfo() : Analyzer("MonsterInfo") {
}

/* methods */
void MonsterInfo::analyze() {
	if (Saiph::hallucinating())
		return; // if we're hallucinating, the output is garbage
	if (_updated == World::internalTurn())
		return; // already farlooked this turn
	_updated = World::internalTurn();
	for (_look_at = World::level().monsters().begin(); _look_at != World::level().monsters().end(); ++_look_at) {
		if (!_look_at->second.visible())
			continue; // don't farlook monsters we can't see
		else if (_look_at->second.symbol() == 'I' || _look_at->second.symbol() == 'm')
			continue; // don't farlook 'I' or 'm' monsters
		else if (_look_at->second.attitude() == HOSTILE)
			continue; // we don't expect hostile monsters to go friendly (XXX: scroll of taming, etc will need special handling)
		World::setAction(static_cast<action::Action*> (new action::FarLook(this, _look_at->first)));
		return;
	}
}

void MonsterInfo::parseMessages(const string& messages) {
	if (_look_at != World::level().monsters().end() && messages.size() > 5 && messages[2] != ' ' && messages[3] == ' ' && messages[4] == ' ' && messages[5] == ' ') {
		/* probably looked at a monster */
		string::size_type pos = string::npos;
		if ((pos = messages.find(" (peaceful ")) != string::npos) {
			/* it's friendly */
			_look_at->second.attitude(FRIENDLY);
			pos += sizeof (" (peaceful ") - 1;
		} else if ((pos = messages.find(" (")) != string::npos) {
			/* hostile */
			if (messages.find(" (Oracle", pos) == pos)
				_look_at->second.attitude(FRIENDLY); // never attack oracle
			else
				_look_at->second.attitude(HOSTILE);
			pos += sizeof (" (") - 1;
		}
		if (pos != string::npos && pos < messages.size() && _look_at->second.symbol() == '@' && _look_at->second.color() == BOLD_WHITE && messages[pos] >= 'A' && messages[pos] <= 'Z')
			_look_at->second.shopkeeper(true); // shopkeepers are always white @, and their names are capitalized
		else
			_look_at->second.shopkeeper(false);
		if (messages.find("priest of ", pos) != string::npos || messages.find("priestess of ", pos) != string::npos)
			_look_at->second.priest(true);
		else
			_look_at->second.priest(false);
		string::size_type pos2 = messages.find(" - ", pos);
		if (pos2 == string::npos)
			pos2 = messages.find(")", pos);
		if (pos2 != string::npos)
			_look_at->second.data(data::Monster::monster(messages.substr(pos, pos2 - pos)));
	}
}