#include <stdlib.h>
#include "MonsterInfo.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Data/MonsterData.h"

using namespace std;

/* constructors/destructor */
MonsterInfo::MonsterInfo(Saiph *saiph) : Analyzer("MonsterInfo"), saiph(saiph) {
}

/* methods */
void MonsterInfo::analyze() {
	if (saiph->world->player.hallucinating)
		return; // if we're hallucinating, the output is garbage
	for (look_at = saiph->levels[saiph->position.level].monsters.begin(); look_at != saiph->levels[saiph->position.level].monsters.end(); ++look_at) {
		if (!look_at->second.visible)
			continue; // monster not visible
		else if (look_at->second.data != NULL && look_at->second.symbol != '@' && look_at->second.symbol != 'A')
			continue; // not an interesting monster
		else if (look_at->second.attitude != ATTITUDE_UNKNOWN)
			continue; // known attitude
		/* farlook this monster */
		command = saiph->farlook(look_at->first);
		priority = PRIORITY_LOOK;
		return;
	}
}

void MonsterInfo::parseMessages(const string &messages) {
	if (look_at != saiph->levels[saiph->position.level].monsters.end() && messages.size() > 5 && messages[2] != ' ' && messages[3] == ' ' && messages[4] == ' ' && messages[5] == ' ') {
		/* probably looked at a monster */
		string::size_type pos = string::npos;
		if ((pos = messages.find(" (peaceful ", 0)) != string::npos) {
			/* it's friendly */
			look_at->second.attitude = FRIENDLY;
			pos += sizeof (" (peaceful ") - 1;
		} else if ((pos = messages.find(" (", 0)) != string::npos) {
			/* hostile */
			if (messages.find(" (Oracle", pos) == pos)
				look_at->second.attitude = FRIENDLY; // never attack oracle
			else
				look_at->second.attitude = HOSTILE;
			pos += sizeof (" (") - 1;
		}
		if (pos != string::npos && pos < messages.size() && look_at->second.symbol == '@' && look_at->second.color == BOLD_WHITE && messages[pos] >= 'A' && messages[pos] <= 'Z')
			look_at->second.shopkeeper = true; // shopkeepers are always white @, and their names are capitalized
		else
			look_at->second.shopkeeper = false;
		if (messages.find("priest of ", pos) != string::npos || messages.find("priestess of ", pos) != string::npos)
			look_at->second.priest = true;
		else
			look_at->second.priest = false;
		if (look_at->second.data == NULL)
			look_at->second.data = MonsterData::getMonsterData(messages.substr(pos, messages.find(")", pos) - pos));
		if (look_at->second.data == NULL)
			look_at->second.data = MonsterData::getMonsterData(messages.substr(pos, messages.find(" - ", pos) - pos));
	} else if (messages.find(" gets angry!", 0) != string::npos) {
		/* uh oh, we pissed someone off, make everyone's attitude unknown */
		for (map<Point, Monster>::iterator look_at = saiph->levels[saiph->position.level].monsters.begin(); look_at != saiph->levels[saiph->position.level].monsters.end(); ++look_at)
			look_at->second.attitude = ATTITUDE_UNKNOWN;
	}
}
