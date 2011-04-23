#include "Events/ElberethQuery.h"

using namespace event;

/* constructors/destructor */
ElberethQuery::ElberethQuery() : Event("ElberethQuery"), _count(0), _type(ELBERETH_NOT_HANDLED) {
}

ElberethQuery::~ElberethQuery() {
}

/* methods */
int ElberethQuery::id() {
	return ID;
}

int ElberethQuery::count() {
	return _count;
}

int ElberethQuery::count(int count) {
	_count = count;
	return this->count();
}

int ElberethQuery::type() {
	return _type;
}

int ElberethQuery::type(int type) {
	_type = type;
	return this->type();
}
