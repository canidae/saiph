#include "Blind.h"
#include "../Globals.h"
#include "../Item.h"
#include "../Player.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Debug.h"

using namespace std;

Blind::Blind(Saiph *saiph) : Analyzer("Blind"), saiph(saiph), willful_blindness(false), blinding_tool(ILLEGAL_ITEM), blind_priority(ILLEGAL_PRIORITY), unblind_priority(ILLEGAL_PRIORITY) {
}

void Blind::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_WHAT_TO_PUT_ON) != string::npos ||
			messages.find(MESSAGE_WHAT_TO_REMOVE) != string::npos) {
		command = blinding_tool;
		priority = PRIORITY_CONTINUE_ACTION;
		willful_blindness = !willful_blindness;
	}
}

void Blind::analyze() {
	if (saiph->inventory_changed)
		findBlindingTool();
	if (saiph->world->player.blind && !willful_blindness) {
		req.request = REQUEST_APPLY_UNIHORN;
		req.priority = PRIORITY_HEALTH_CURE_NON_DEADLY;
		saiph->request(req);
		return;
	}
	if (blind_priority) {
		if (willful_blindness) {
			blind_priority = 0;
			return;
		}
		command = PUT_ON;
		priority = blind_priority;
	} else if (unblind_priority) {
		if (!willful_blindness) {
			unblind_priority = 0;
			return;
		}
		command = REMOVE;
		priority = blind_priority;
	}
}

void Blind::complete() {
	blind_priority = ILLEGAL_PRIORITY;
	unblind_priority = ILLEGAL_PRIORITY;
}

bool Blind::request(const Request &request) {
	if (request.request == REQUEST_BECOME_BLIND) {
		if (willful_blindness) {
			Debug::warning(saiph->last_turn) << "Recieved REQUEST_BECOME_BLIND while already willfully blind" << endl;
			return true;
		}
		if (blinding_tool == ILLEGAL_ITEM)
			return false;
		blind_priority = request.priority;
	} else if (request.request == REQUEST_UNBLIND) {
		if (!saiph->world->player.blind) {
			Debug::warning(saiph->last_turn) << "Recieved REQUEST_UNBLIND while already unblind" << endl;
			return true;
		}
		unblind_priority = request.priority;
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
