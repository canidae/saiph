#include "Unihorn.h"
#include "../Request.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Unihorn::Unihorn(Saiph *saiph) : Analyzer("Unihorn"), saiph(saiph), unihorn_key(ILLEGAL_ITEM), apply_priority(-1), unihorn_use_turn(-1) {
}

/* methods */
void Unihorn::analyze(const string &messages) {
	parseMessages(messages);
	if (saiph->world->menu || saiph->world->question)
		return;
	if (apply_priority >= 0 && unihorn_key != ILLEGAL_ITEM && unihorn_use_turn <= saiph->world->player.turn && saiph->world->player.encumbrance < OVERTAXED) {
		/* unihorn failed last attempt, try again */
		command = APPLY;
		priority = apply_priority;
	}
}

void Unihorn::parseMessages(const string &messages) {
	if (saiph->inventory_changed)
		findUnihorn();
	if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_APPLY) != string::npos) {
		command = unihorn_key;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(UNIHORN_NOTHING_HAPPENS) != string::npos) {
		apply_priority = -1;
		unihorn_use_turn = saiph->world->player.turn + UNIHORN_UNIHORN_TIMEOUT;
	}
}

bool Unihorn::request(const Request &request) {
	if (request.request == REQUEST_APPLY_UNIHORN) {
		command = APPLY;
		priority = apply_priority;
		return true;
	}
	return false;
}

/* private methods */
void Unihorn::findUnihorn() {
	map<unsigned char, Item>::iterator u = saiph->inventory.find(unihorn_key);
	if (u != saiph->inventory.end() && isUnihorn(u->second))
		return;
	for (u = saiph->inventory.begin(); u != saiph->inventory.end(); ++u) {
		if (isUnihorn(u->second)) {
			unihorn_key = u->first;
			return;
		}
	}
	unihorn_key = ILLEGAL_ITEM;
}

bool Unihorn::isUnihorn(const Item &item) {
	if (item.beatitude == CURSED || item.name != "unicorn horn")
		return false;
	if (item.beatitude == BEATITUDE_UNKNOWN) {
		/* must beatify unihorn first */
		req.request = REQUEST_BEATIFY_ITEMS;
		saiph->request(req);
		return false;
	}
	return true;
}
