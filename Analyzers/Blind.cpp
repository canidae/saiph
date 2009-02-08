#include "Blind.h"
#include "../Globals.h"
#include "../Item.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

Blind::Blind(Saiph *saiph) : Analyzer("Blind"), saiph(saiph), willful_blindness(false), blinding_tool(ILLEGAL_ITEM) {
}

void Blind::analyze() {
	if (saiph->inventory_changed)
		findBlindingTool();
	if (saiph->world->player.blind && !willful_blindness) {
		req.request = REQUEST_APPLY_UNIHORN;
		req.priority = PRIORITY_HEALTH_CURE_NON_DEADLY;
		saiph->request(req);
	}
}

bool Blind::request(const Request &request) {
	if (request.request == REQUEST_BECOME_BLIND) {
		/* TODO: put on blindfold/towel */
	} else if (request.request == REQUEST_UNBLIND) {
		/* TODO: remove blindfold/towel */
	}
	return false;
}

/* private methods */
void Blind::findBlindingTool() {
	map<unsigned char, Item>::iterator i = saiph->inventory.find(blinding_tool);
	if (i != saiph->inventory.end() && isBlindingTool(i->second))
		return;
	for (i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (isBlindingTool(i->second)) {
			blinding_tool = i->first;
			return;
		}
	}
	blinding_tool = ILLEGAL_ITEM;
}

bool Blind::isBlindingTool(const Item &item) {
	if (item.beatitude == CURSED || (item.name != "blindfold" && item.name != "towel"))
		return false;
	if (item.beatitude == BEATITUDE_UNKNOWN) {
		/* must beatify blinding tool first */
		req.request = REQUEST_BEATIFY_ITEMS;
		saiph->request(req);
		return false;
	}
	return true;
}
