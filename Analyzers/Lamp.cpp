#include "Lamp.h"
#include "../Request.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Lamp::Lamp(Saiph *saiph) : Analyzer("Lamp"), saiph(saiph), lamp_key(0), remove_lamp(false) {
}

/* methods */
void Lamp::analyze() {
	if (priority > LAMP_PRIORITY_TOGGLE || lamp_key == 0)
		return; // no lamp/lantern or got something more important to do
	map<unsigned char, Item>::iterator l = saiph->inventory.find(lamp_key);
	if (l == saiph->inventory.end()) {
		/* hmm, this shouldn't happen */
		lamp_key = 0;
		return;
	}
	/* should turn this lamp/lantern on */
	command = APPLY;
	priority = LAMP_PRIORITY_TOGGLE;
}

void Lamp::parseMessages(const string &messages) {
	if (messages.find(LAMP_TURNED_ON, 0) != string::npos) {
		/* lamp/lantern turned on, set "additional" */
		map<unsigned char, Item>::iterator l = saiph->inventory.find(lamp_key);
		if (l == saiph->inventory.end()) {
			/* uh, what? */
			req.request = REQUEST_DIRTY_INVENTORY;
			saiph->request(req);
		} else {
			/* set "additional" */
			l->second.additional = LAMP_LIT;
		}
		/* figure out if we got more unlit lamps */
		findLamp();
	} else if (messages.find(LAMP_LAMP_GOES_OUT, 0) != string::npos || messages.find(LAMP_LANTERN_GOES_OUT, 0) != string::npos) {
		/* probably got engulfed or something, lamp/lantern went out */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	}

	if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_APPLY, 0) != string::npos) {
		/* turn on lamp/lantern */
		command = lamp_key;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(LAMP_LAMP_OUT, 0) != string::npos || messages.find(LAMP_OIL_LAMP_OUT, 0) != string::npos || messages.find(LAMP_LANTERN_OUT, 0) != string::npos || messages.find(LAMP_LAMP_OUT_OF_POWER, 0) != string::npos || messages.find(LAMP_NO_OIL, 0) != string::npos) {
		/* a lamp/lantern is depleted. discard it.
		 * problem is that we don't know which lamp/lantern it is
		 * as we may have multiple lamps/lanterns on.
		 * request inventory update, set a flag and remove the first
		 * unlit lamp we find */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
		remove_lamp = true;
	} else if (saiph->inventory_changed) {
		/* inventory changed, find lamp/lantern */
		findLamp();
		if (remove_lamp && lamp_key != 0) {
			req.request = REQUEST_NAME_ITEM;
			req.data = DISCARD;
			req.key = lamp_key;
			saiph->request(req);
			remove_lamp = false;
			lamp_key = 0;
		}
	}
}

/* private methods */
void Lamp::findLamp() {
	map<unsigned char, Item>::iterator l = saiph->inventory.find(lamp_key);
	if (l != saiph->inventory.end() && l->second.named != DISCARD && l->second.additional != LAMP_LIT && (l->second.name == "lamp" || l->second.name == "oil lamp" || l->second.name == "brass lantern" || l->second.name == "magic lamp"))
		return;
	for (l = saiph->inventory.begin(); l != saiph->inventory.end(); ++l) {
		if (l->second.name != "lamp" && l->second.name != "oil lamp" && l->second.name != "brass lantern" && l->second.name != "magic lamp")
			continue; // not a lamp/lantern
		else if (l->second.named == DISCARD)
			continue; // probably depleted lamp/lantern
		else if (l->second.additional == LAMP_LIT)
			continue; // already lit, we turn on every lamp/lantern so skip this
		/* this should be a lamp/lantern */
		lamp_key = l->first;
		return;
	}
	/* no lamp/lantern */
	lamp_key = 0;
}
