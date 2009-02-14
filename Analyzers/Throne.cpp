#include "Throne.h"
#include "../Globals.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

Throne::Throne(Saiph *saiph) : Analyzer("Throne"), saiph(saiph) {
}

void Throne::analyze() {
	if (priority >= PRIORITY_THRONE_SIT)
		return;
	else if (saiph->world->player.hitpoints < 7 || (!saiph->world->player.shock_resistance && saiph->world->player.hitpoints < 31))
		return; // don't get killed by an electric chair
	else if (saiph->world->player.hitpoints - saiph->world->player.hitpoints_max > 5)
		return; // for the HP boost result
	const PathNode &node = saiph->shortestPath(THRONE);
	if (node.cost >= UNPASSABLE)
		return;
	else if (node.dir == NOWHERE)
		command = SIT;
	else
		command = node.dir;
	priority = PRIORITY_THRONE_SIT;
}
