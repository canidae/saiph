#include "Analyzers/RandomWalk.h"

#include <stdlib.h>
#include "Level.h"
#include "Point.h"
#include "Tile.h"
#include "World.h"
#include "Actions/Move.h"
#include "Actions/Rest.h"

using namespace analyzer;

/* constructors/destructor */
RandomWalk::RandomWalk() : Analyzer("RandomWalk") {
}

/* methods */
void RandomWalk::analyze() {
	if (World::currentPriority() > PRIORITY_RANDOM_WALK)
		return;
	else if (Saiph::blind() || Saiph::confused() || Saiph::hallucinating() || Saiph::stunned())
		return; // don't walk when we're blind/confused/hallucinating/stunned

	Point p = Saiph::position();
	switch (rand() % 8) {
	case 0:
		p.moveNorthwest();
		break;

	case 1:
		p.moveNorth();
		break;

	case 2:
		p.moveNortheast();
		break;

	case 3:
		p.moveWest();
		break;

	case 4:
		p.moveEast();
		break;

	case 5:
		p.moveSouthwest();
		break;

	case 6:
		p.moveSouth();
		break;

	case 7:
		p.moveSoutheast();
		break;
	}

	Tile t = World::level().tile(p);
	if (t.cost() < UNPASSABLE)
		World::setAction(static_cast<action::Action*> (new action::Move(this, t, 1)));
	else
		World::setAction(static_cast<action::Action*> (new action::Rest(this, 1)));
}