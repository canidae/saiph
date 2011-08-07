#include "Connection.h"
#include "Local.h"
#include "Telnet.h"
#include "Replay.h"
#include "Embed.h"

using namespace std;

/* constructors/destructor */
Connection::Connection() {
}

Connection::~Connection() {
}

/* static methods */
Connection* Connection::create(int interface) {
	switch (interface) {
	case CONNECTION_LOCAL:
		return new Local();
		break;

	case CONNECTION_TELNET:
		return new Telnet();
		break;

	case CONNECTION_REPLAY:
		return new Replay();
		break;

	case CONNECTION_EMBED:
		return new Embed();
		break;

	default:
		return NULL;
	}
	/* this never happens :) */
	return NULL;
}

/* methods */
int Connection::doRetrieve(char*, int) {
	return 0;
}

int Connection::retrieve(char* buffer, int size) {
	int amt = doRetrieve(buffer, size);
	Replay::recordFrame(buffer, amt);
	return amt;
}

int Connection::transmit(const string&) {
	return 0;
}

void Connection::start() {
}

void Connection::stop() {
}
