#include <stdlib.h>
#include "Donate.h"
#include "../Player.h"
#include "../World.h"

using namespace std;

Donate::Donate(Saiph* saiph) : Analyzer("Donate"), saiph(saiph), priest_dir(ILLEGAL_DIRECTION), priest_loc() {
}

void Donate::parseMessages(const std::string& messages) {
	if (priest_dir == ILLEGAL_DIRECTION)
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
	if (saiph->world->player.zorkmids < 400 * saiph->world->player.experience)
		return;
	if (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] == STAIRS_DOWN) {
		/* got enough money, standing on stairs down, find priest */
		priest_loc.level = -1;
		int least_moves = INT_MAX;
		for (int lev = 0; lev < (int) saiph->levels.size(); lev++) {
			for (map<Point, Monster>::iterator i = saiph->levels[lev].monsters.begin(); i != saiph->levels[lev].monsters.end(); ++i) {
				if (!i->second.priest)
					continue;
				int moves = 0;
				unsigned char dir = saiph->shortestPath(priest_loc, false, &moves);
				if (dir == ILLEGAL_DIRECTION)
					continue; // can't path to this priest
				if (moves < least_moves) {
					/* this priest is closer than the previous priest */
					priest_loc = Coordinate(lev, i->first);
					least_moves = moves;
				}
			}
		}
	}
	if (priest_loc.level != -1) {
		int moves = 0;
		priest_dir = saiph->shortestPath(priest_loc, false, &moves);
		if (priest_dir == ILLEGAL_DIRECTION)
			return;
		if (moves == 1)
			command = CHAT;
		else
			command = priest_dir;
		priority = PRIORITY_DONATE_CHAT_TO_PRIEST;
	}
}
