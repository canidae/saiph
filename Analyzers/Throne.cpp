#include "Throne.h"
#include "../Globals.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Level.h"

using namespace std;

Throne::Throne(Saiph* saiph) : Analyzer("Throne"), saiph(saiph) {
}

void Throne::parseMessages(const std::string& messages) {
}

void Throne::analyze() {
	if (priority >= PRIORITY_THRONE_SIT)
		return;
	bool hasUnihorn = false;
	//in case we get blinded
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i)
		if (i->second.name == "unicorn horn" && i->second.beatitude != CURSED)
			hasUnihorn = true;
	int moves = 0;
	unsigned char dir = saiph->shortestPath(THRONE, false, &moves);
	if (dir == ILLEGAL_DIRECTION)
		return;
	else if (dir == NOWHERE) {
		//don't get killed by an electric chair
		if (!saiph->world->player.shock_resistance && saiph->world->player.hitpoints < 31)
			return;
		if (saiph->world->player.shock_resistance && saiph->world->player.hitpoints < 7)
			return;
		//for the HP boost result
		if (saiph->world->player.hitpoints - saiph->world->player.hitpoints_max > 5)
			return;
		//don't lose our gold
		//TODO
		command = SIT;
	} else
		command = dir;
	priority = PRIORITY_THRONE_SIT;
}
