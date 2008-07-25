#include "Pray.h"

/* constructors */
Pray::Pray(Saiph *saiph) : Analyzer("Pray"), saiph(saiph), last_pray_turn(0) {
}

/* methods */
void Pray::complete() {
	last_pray_turn = saiph->world->player.turn;
}

void Pray::parseMessages(const string &messages) {
}

bool Pray::request(const Request &request) {
	if (request.request == REQUEST_PRAY) {
		if (last_pray_turn + PRAY_PRAYER_TIMEOUT <= saiph->world->player.turn) {
			/* it's (probably) safe to pray */
			priority = request.priority;
			command = PRAY;
			return true;
		}
	}
	return false;
}
