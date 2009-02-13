#include <stdlib.h>
#include "Donate.h"
#include "../Monster.h"
#include "../Player.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

Donate::Donate(Saiph *saiph) : Analyzer("Donate"), saiph(saiph), priest_dir(ILLEGAL_DIRECTION), priest_loc() {
}

void Donate::parseMessages(const std::string &messages) {
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
	if (saiph->getDungeonSymbol() == STAIRS_DOWN) {
		/* got enough money, standing on stairs down, find priest */
		priest_loc.level = -1;
		unsigned int least_moves = UNREACHABLE;
		for (int lev = 0; lev < (int) saiph->levels.size(); lev++) {
			for (map<Point, Monster>::iterator i = saiph->levels[lev].monsters.begin(); i != saiph->levels[lev].monsters.end(); ++i) {
				if (!i->second.priest)
					continue;
				const PathNode &node = saiph->shortestPath(priest_loc);
				if (node.dir == ILLEGAL_DIRECTION)
					continue; // can't path to this priest
				if (node.moves < least_moves) {
					/* this priest is closer than the previous priest */
					priest_loc = Coordinate(lev, i->first);
					least_moves = node.moves;
				}
			}
		}
	}
	if (priest_loc.level != -1) {
		int moves = 0;
		const PathNode &node = saiph->shortestPath(priest_loc);
		priest_dir = node.dir;
		if (priest_dir == ILLEGAL_DIRECTION)
			return;
		if (moves == 1)
			command = CHAT;
		else
			command = priest_dir;
		priority = PRIORITY_DONATE_CHAT_TO_PRIEST;
	}
}
