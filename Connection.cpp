#include "Connection.h"
#include "Local.h"
#include "Telnet.h"

using namespace std;

/* constructors/destructor */
Connection::Connection(ofstream *debugfile) : debugfile(debugfile) {
}

Connection::~Connection() {
}

/* static methods */
Connection *Connection::create(int interface, ofstream *debugfile) {
	switch (interface) {
		case CONNECTION_LOCAL:
			return new Local(debugfile);
			break;

		case CONNECTION_TELNET:
			return new Telnet(debugfile);
			break;

		default:
			return NULL;
	}
	/* this never happens :) */
	return NULL;
}

/* methods */
int Connection::retrieve(char *buffer, int count) {
	return 0;
}

int Connection::transmit(const string &data) {
	return 0;
}

void Connection::start() {
}

void Connection::stop() {
}
