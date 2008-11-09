#include "Pray.h"
#include "../Request.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Pray::Pray(Saiph *saiph) : Analyzer("Pray"), saiph(saiph), last_pray_turn(0) {
}

/* methods */
void Pray::parseMessages(const string &messages) {
	if (messages.find(PRAY_FINISHED_PRAYING, 0) != string::npos)
		last_pray_turn = saiph->world->player.turn;
}

bool Pray::request(const Request &request) {
	if (request.request == REQUEST_PRAY) {
		if (last_pray_turn + PRAY_PRAYER_TIMEOUT <= saiph->world->player.turn) {
			/* it's (probably) safe to pray */
			setCommand(0, request.priority, PRAY);
			sequence = 0;
			return true;
		}
	}
	return false;
}
