#include "Blind.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Player.h"
#include "../Request.h"

using namespace std;

Blind::Blind(Saiph* saiph) : Analyzer("Blind"), saiph(saiph), willful_blindness(false), blinding_tool(0) {
}

void Blind::analyze() {
	if (saiph->inventory_changed) {
		if (blinding_tool != 0 && !isBlindingTool(blinding_tool))
			blinding_tool = 0;
		if (blinding_tool == 0) {
			for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i)
				if (isBlindingTool(i->second)) {
					blinding_tool == i->first;
					break;
				}
		}
	}
	if (saiph->world->player.blind && !willful_blindness) {
		Request req;
		req.request = REQUEST_APPLY_UNIHORN;
		req.priority = PRIORITY_HEALTH_CURE_NON_DEADLY;
		saiph->request(req);
	}
}

bool Blind::request(const Request &request) {
	if (request.request == REQUEST_BECOME_BLIND) {
		
	} else if (request.request == REQUEST_UNBLIND) {

	}
	return false;
}

bool Blind::isBlindingTool(const unsigned char& letter) {
	map<unsigned char, Item>::iterator iter = saiph->inventory.find(letter);
	return iter != saiph->inventory.end() && isBlindingTool(iter->second);
}

bool Blind::isBlindingTool(const Item& item) {
	return (item.name == "blindfold" || item.name == "towel") &&
			(item.beatitude == BLESSED || item.beatitude == UNCURSED);
}
