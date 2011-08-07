#include "Analyzers/Boulder.h"

#include "EventBus.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/ApplyInDirection.h"
#include "Actions/Move.h"
#include "Events/GotDiggingTool.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Boulder::Boulder() : Analyzer("Boulder"), _pick_key(ILLEGAL_ITEM) {
	/* register events */
	EventBus::registerEvent(GotDiggingTool::ID, this);
}

/* methods */
void Boulder::analyze() {
	/* destroy boulders */
	if (Saiph::blind() || Saiph::confused() || Saiph::stunned() || Saiph::hallucinating())
		return; // don't move to, or destroy boulders when blind/confused/stunned/hallucinating
	if (World::level().branch() == BRANCH_SOKOBAN)
		return; // these boulders stay!
	if (_pick_key == ILLEGAL_ITEM)
		return; // we don't have any way ...

	/* go to nearest boulder and destroy it somehow */
	for (map<Point, int>::const_iterator d = World::level().symbols((unsigned char) BOULDER).begin(); d != World::level().symbols((unsigned char) BOULDER).end(); ++d) {
		Tile& tile = World::shortestPath(d->first);
		if (tile.cost() == UNREACHABLE)
			continue; // can't reach this boulder
		if (Saiph::inAPit() && tile.distance() == 1)
			continue; // in a pit next to the boulder
		if (tile.distance() == 1) {
			/* smash it! */
			World::setAction(new action::ApplyInDirection(this, _pick_key, tile.direction(), PRIORITY_BOULDER_DESTROY));
		} else {
			/* go to door */
			World::setAction(new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_BOULDER_DESTROY, tile.distance())));
		}
	}
}

void Boulder::onEvent(Event* const event) {
	if (event->id() == GotDiggingTool::ID)
		_pick_key = (static_cast<GotDiggingTool*> (event))->key();
}
