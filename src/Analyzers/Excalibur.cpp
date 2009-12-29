#include "Analyzers/Excalibur.h"

#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Move.h"

using namespace analyzer;
using namespace std;

/* constructors/destructor */
Excalibur::Excalibur() : Analyzer("Excalibur") {
}

/* methods */
void Excalibur::analyze() {
	if (Saiph::experience() < 8)
		return;
	else if (Saiph::alignment() != LAWFUL)
		return;
	else if (Saiph::blind() || Saiph::confused() || Saiph::stunned() || Saiph::hallucinating())
		return; // don't move when blind/confused/stunned/hallucinating
	/* do we have a long sword? */
	unsigned char got_long_sword = ILLEGAL_ITEM;
	for(map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		if (i->second.name == EXCALIBUR_LONG_SWORD) {
			got_long_sword = i->first;
			break;
		}
	}
	if (got_long_sword == ILLEGAL_ITEM)
		return;
	/* path to nearest fountain */
	const Tile& tile = World::shortestPath(FOUNTAIN);
	if (tile.cost() >= UNPASSABLE)
		return; // can't get to any fountains
	if (tile.direction() == NOWHERE) {
		/* standing on (in?) fountain, dip */
		command = DIP;
		command2 = got_long_sword;
		priority = PRIORITY_EXCALIBUR_DIP;
	} else {
		/* move towards fountain */
		World::setAction(static_cast<action::Action*> (new action::Move(this, tile, PRIORITY_EXCALIBUR_DIP)));
	}
}

void Excalibur::parseMessages(const string& messages) {
	if (command != "" && command2 != "" && messages.find(MESSAGE_WHAT_TO_DIP, 0) != string::npos) {
		/* what to dip... the long sword ofcourse! */
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (command != "" && command2 != "" && messages.find(MESSAGE_DIP_IN_FOUNTAIN, 0) != string::npos) {
		/* if we want to dip in fountain? sure */
		command = YES;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_RECEIVED_EXCALIBUR, 0) != string::npos) {
		/* alright! */
		expired = true; // delete this analyzer from analyzers
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	}
}
