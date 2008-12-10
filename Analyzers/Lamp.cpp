#include "Lamp.h"
#include "../Request.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Lamp::Lamp(Saiph *saiph) : Analyzer("Lamp"), saiph(saiph), lamp_key(0) {
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
	if (saiph->world->player.blind && l->second.additional == "lit") {
		/* we should turn lamp/lantern off when we're blind */
		command = APPLY;
		priority = LAMP_PRIORITY_TOGGLE;
		return;
	}
	bool dark_area = (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] == CORRIDOR || isAreaDark());
	if (dark_area && l->second.additional != "lit") {
		/* should turn this lamp/lantern on */
		command = APPLY;
		priority = LAMP_PRIORITY_TOGGLE;
	} else if (!dark_area && l->second.additional == "lit") {
		/* should turn this lamp/lantern off */
		command = APPLY;
		priority = LAMP_PRIORITY_TOGGLE;
	}
}

void Lamp::parseMessages(const string &messages) {
	if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_APPLY, 0) != string::npos) {
		/* turn on/off lamp/lantern */
		command = lamp_key;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (lamp_key != 0 && (messages.find(LAMP_LAMP_OUT, 0) != string::npos || messages.find(LAMP_OIL_LAMP_OUT, 0) != string::npos || messages.find(LAMP_LANTERN_OUT, 0) != string::npos || messages.find(LAMP_NO_OIL, 0) != string::npos)) {
		/* this lamp/lantern is depleted. discard it */
		req.request = REQUEST_NAME_ITEM;
		req.data = DISCARD;
		req.key = lamp_key;
		saiph->request(req);
		lamp_key = 0;
	} else if (saiph->inventory_changed && lamp_key == 0) {
		/* inventory changed, find lamp/lantern */
		findLamp(); // inventory is changed and we got no lamp/lantern
	}
	if (messages.find(LAMP_TURNED_ON, 0) != string::npos) {
		/* lamp/lantern turned on, set "additional" */
		map<unsigned char, Item>::iterator l = saiph->inventory.find(lamp_key);
		if (l == saiph->inventory.end()) {
			/* uh, what? */
			req.request = REQUEST_DIRTY_INVENTORY;
			saiph->request(req);
		} else {
			/* set "additional" */
			l->second.additional = "lit";
		}
	} else if (messages.find(LAMP_TURNED_OFF, 0) != string::npos) {
		/* lamp/lantern turned off, unset "additional" */
		map<unsigned char, Item>::iterator l = saiph->inventory.find(lamp_key);
		if (l == saiph->inventory.end()) {
			/* uh, what? */
			req.request = REQUEST_DIRTY_INVENTORY;
			saiph->request(req);
		} else {
			/* unset "additional" */
			l->second.additional = "";
		}
	}
}

/* private methods */
void Lamp::findLamp() {
	map<unsigned char, Item>::iterator l = saiph->inventory.find(lamp_key);
	if (l != saiph->inventory.end() && l->second.named != DISCARD && (l->second.name == "lamp" || l->second.name == "oil lamp" || l->second.name == "brass lantern" || l->second.name == "magic lamp"))
		return;
	for (l = saiph->inventory.begin(); l != saiph->inventory.end(); ++l) {
		if (l->second.name != "lamp" && l->second.name != "oil lamp" && l->second.name != "brass lantern" && l->second.name != "magic lamp")
			continue; // not a lamp/lantern
		else if (l->second.named == DISCARD)
			continue; // probably depleted lamp/lantern
		/* this should be a lamp/lantern */
		lamp_key = l->first;
		return;
	}
	/* no lamp/lantern */
	lamp_key = 0;
}

bool Lamp::isAreaDark() {
	/* check if the area appears to be dark */
	int lit_areas = 0;
	if (saiph->position.col - 4 >= MAP_COL_BEGIN) {
		for (int r = saiph->position.row - 4; r < saiph->position.row + 4; ++r) {
			if (r < MAP_ROW_BEGIN || r > MAP_ROW_END || saiph->world->view[r][saiph->position.col - 4] == FLOOR || (saiph->world->view[r][saiph->position.col - 4] == CORRIDOR && saiph->world->color[r][saiph->position.col - 4] == BOLD_WHITE))
				++lit_areas;
		}
	}
	if (lit_areas > LAMP_LIT_AREAS_THRESHOLD)
		return false;
	if (saiph->position.col + 4 <= MAP_COL_END) {
		for (int r = saiph->position.row - 4; r < saiph->position.row + 4; ++r) {
			if (r < MAP_ROW_BEGIN || r > MAP_ROW_END || saiph->world->view[r][saiph->position.col + 4] == FLOOR || (saiph->world->view[r][saiph->position.col + 4] == CORRIDOR && saiph->world->color[r][saiph->position.col + 4] == BOLD_WHITE))
				++lit_areas;
		}
	}
	if (lit_areas > LAMP_LIT_AREAS_THRESHOLD)
		return false;
	if (saiph->position.row - 4 >= MAP_ROW_BEGIN) {
		for (int c = saiph->position.col - 4; c < saiph->position.col + 4; ++c) {
			if (c < MAP_COL_BEGIN || c > MAP_COL_END || saiph->world->view[saiph->position.row - 4][c] == FLOOR || (saiph->world->view[saiph->position.row - 4][c] == CORRIDOR && saiph->world->color[saiph->position.row - 4][c] == BOLD_WHITE))
				++lit_areas;
		}
	}
	if (lit_areas > LAMP_LIT_AREAS_THRESHOLD)
		return false;
	if (saiph->position.row + 4 <= MAP_ROW_END) {
		for (int c = saiph->position.col - 4; c < saiph->position.col + 4; ++c) {
			if (c < MAP_COL_BEGIN || c > MAP_COL_END || saiph->world->view[saiph->position.row + 4][c] == FLOOR || (saiph->world->view[saiph->position.row + 4][c] == CORRIDOR && saiph->world->color[saiph->position.row + 4][c] == BOLD_WHITE))
				++lit_areas;
		}
	}
	if (lit_areas > LAMP_LIT_AREAS_THRESHOLD)
		return false;
	/* seems like area is dark */
	return true;
}
