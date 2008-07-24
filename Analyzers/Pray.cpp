#include "Pray.h"

/* constructors */
Pray::Pray(Saiph *saiph) : Analyzer("Pray"), saiph(saiph) {
	action = "";
	last_pray_turn = PRAY_PRAYER_TIMEOUT;
	priority = ILLEGAL_PRIORITY;
}

/* methods */
void Pray::command(string *command) {
	*command = action;
	last_pray_turn = saiph->world->player.turn;
}

void Pray::parseMessages(const string &messages) {
}

bool Pray::request(const Request &request) {
	if (request.request == REQUEST_PRAY) {
		if (last_pray_turn + PRAY_PRAYER_TIMEOUT <= saiph->world->player.turn) {
			/* it's (probably) safe to pray */
			priority = request.priority;
			action = PRAY;
			return true;
		}
	}
	return false;
}
