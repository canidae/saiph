#include "Ring.h"
#include "../Saiph.h"
#include "../World.h"

using namespace analyzer;
using namespace std;

/* constructors/destructor */
Ring::Ring(Saiph *saiph) : Analyzer("Ring"), saiph(saiph), command2(""), wear_ring(false) {
}

/* methods */
void Ring::analyze() {
	if (saiph->inventory_changed || wear_ring)
		wearRing();
}

void Ring::parseMessages(const string &messages) {
	if (saiph->world->question && (messages.find(MESSAGE_WHAT_TO_PUT_ON, 0) != string::npos || messages.find(MESSAGE_WHAT_TO_REMOVE, 0) != string::npos)) {
		/* put on or remove a ring */
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
		wear_ring = false;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (saiph->world->question && messages.find(MESSAGE_WHICH_RING_FINGER, 0) != string::npos) {
		/* need to specify which finger the ring should go on */
		command = "l";
		priority = PRIORITY_CONTINUE_ACTION;
		wear_ring = false;
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	}
}

bool Ring::request(const Request &request) {
	if (request.request == REQUEST_RING_WEAR) {
		/* player wish to wear this ring */
		WearRing wr;
		wr.beatitude = request.beatitude;
		wr.name = request.data;
		rings.push_back(wr);
		return true;
	}
	return false;
}

/* private methods */
void Ring::wearRing() {
	/* find out which rings we should wear */
	unsigned char ring_on_right = 0;
	int ring_on_right_pri = INT_MAX;
	bool ring_on_right_cursed = false;
	unsigned char ring_on_left = 0;
	int ring_on_left_pri = INT_MAX;
	bool ring_on_left_cursed = false;
	unsigned char best_key = 0;
	int best_ring = INT_MAX;
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.additional.find("on right hand", 0) == 0) {
			ring_on_right = i->first;
			if (i->second.beatitude == CURSED)
				ring_on_right_cursed = true;
			continue;
		} else if (i->second.additional.find("on left hand", 0) == 0) {
			ring_on_left = i->first;
			if (i->second.beatitude == CURSED)
				ring_on_left_cursed = true;
			continue;
		}
		for (vector<WearRing>::size_type w = 0; w < rings.size(); ++w) {
			if (ring_on_right != 0 && ring_on_right_pri == INT_MAX && rings[w].name == i->second.name)
				ring_on_right_pri = w;
			if (ring_on_left != 0 && ring_on_left_pri == INT_MAX && rings[w].name == i->second.name)
				ring_on_left_pri = w;
			if (rings[w].name != i->second.name || (int) w >= best_ring)
				continue;
			else if ((rings[w].beatitude & i->second.beatitude) == 0)
				continue;
			best_key = i->first;
			best_ring = w;
		}
	}
	if (ring_on_right_cursed && ring_on_left_cursed)
		return; // both rings cursed, return for now
	else if (best_key == 0)
		return; // we got no ring to put on
	else if (ring_on_right_pri <= best_ring && ring_on_left_pri <= best_ring)
		return; // already wearing just as good or better rings
	wear_ring = true;
	if (ring_on_right != 0 && ring_on_left != 0) {
		/* must remove one ring before we can put on the new ring */
		command = REMOVE;
		priority = PRIORITY_RING_WEAR;
		/* in case we get question about which ring to remove */
		if (ring_on_right_pri < ring_on_left_pri)
			command2 = ring_on_left;
		else
			command2 = ring_on_right;
	} else {
		/* no need to remove any rings, just put on the new ring */
		priority = PRIORITY_RING_WEAR;
		command =  PUT_ON;
		command2 = best_key;
	}
}
