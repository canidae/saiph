#include <stdlib.h>
#include "Donate.h"
#include "../World.h"
#include "../Player.h"

using namespace std;

Donate::Donate(Saiph* saiph) : Analyzer("Donate"), saiph(saiph), priest_dir(0), priest_loc() {
}

void Donate::parseMessages(const std::string& messages) {
	if (priest_dir == 0)
		return;
	if (messages.find(DONATE_TALK_TO_WHOM) != string::npos) {
		command = priest_dir;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(DONATE_HOW_MUCH_OFFER) != string::npos) {
		command = 400 * saiph->world->player.experience;
		command.append("\n");
		priority = PRIORITY_CONTINUE_ACTION;
	}
}

void Donate::analyze() {
	if (priority >= PRIORITY_DONATE_CHAT_TO_PRIEST)
		return;
	if (saiph->world->player.zorkmids < 400 * saiph->world->player.experience) {
		priest_loc.level = -1;
		priest_dir = 0;
		return;
	}
	if (priest_loc.level != -1)
		for (int lev = 0; lev < (int) saiph->levels.size(); lev++) {
			for (map<Point, Monster>::iterator i = saiph->levels[lev].monsters.begin(); i != saiph->levels[lev].monsters.end(); ++i)
				if (i->second.priest) {
					if (priest_loc.level != -1) {
						//check if this priest is closer
						if (abs(priest_loc.level - saiph->position.level) < abs(lev - saiph->position.level))
							continue; //it wasn't, but keep looking
					}
					priest_loc = Coordinate(lev, i->first);
				}
		}
	if (priest_loc.level != -1) {
		priority = PRIORITY_DONATE_CHAT_TO_PRIEST;
		int moves = -1;
		priest_dir = saiph->shortestPath(priest_loc, false, &moves);
		if (moves == 1)
			command = CHAT;
		else
			command = priest_dir;
	}
}
