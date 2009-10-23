#include "Connection.h"
#include "Local.h"
#include "Telnet.h"

using namespace std;

/* constructors/destructor */
Connection::Connection() {
}

Connection::~Connection() {
}

/* static methods */
Connection* Connection::create(const int& interface) {
	switch (interface) {
	case CONNECTION_LOCAL:
		return new Local();
		break;

	case CONNECTION_TELNET:
		return new Telnet();
		break;

	default:
		return NULL;
	}
	/* this never happens :) */
	return NULL;
}

/* methods */
int Connection::retrieve(char*, const int&) {
	return 0;
}

int Connection::transmit(const string&) {
	return 0;
}

void Connection::start() {
}

void Connection::stop() {
}
