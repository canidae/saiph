#include <stdlib.h>
#include "Elbereth.h"
#include "../EventBus.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/Look.h"
#include "../Events/ElberethQuery.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Elbereth::Elbereth() : Analyzer("Elbereth"), _elbereth_count(0), _engraving_type(ELBERETH_MUST_CHECK) {
	/* register events */
	EventBus::registerEvent(ElberethQuery::ID, this);
}

void Elbereth::onEvent(Event* const evt) {
	if (evt->id() == ElberethQuery::ID) {
		ElberethQuery* const q = static_cast<ElberethQuery* const> (evt);
		if (World::getLastActionID() != action::Look::ID) {
			/* data is outdated */
			_engraving_type = ELBERETH_MUST_CHECK;
			_elbereth_count = 0;
		}
		q->type(_engraving_type);
		q->count(_elbereth_count);
	}
}

void Elbereth::parseMessages(const string& messages) {
	/* figure out if something is engraved here */
	string::size_type pos = messages.find(MESSAGE_YOU_READ);
	if (pos == string::npos) {
		/* no elbereths here :( */
		_elbereth_count = 0;
		_engraving_type = ELBERETH_NONE;
		return;
	}
	/* is it written in dust, burned or digged? */
	if (messages.find(MESSAGE_DUSTED_TEXT) != string::npos) {
		/* it's dusted. */
		_engraving_type = ELBERETH_DUSTED;
	} else if (messages.find(MESSAGE_BURNED_TEXT) != string::npos) {
		/* it's burned */
		_engraving_type = ELBERETH_PERMANENT;
	} else if (messages.find(MESSAGE_DIGGED_TEXT) != string::npos) {
		/* it's digged */
		_engraving_type = ELBERETH_SEMIPERM;
	} else if (messages.find(MESSAGE_FROSTED_TEXT) != string::npos) {
		/* it's frosted */
		_engraving_type = ELBERETH_DUSTED;
	} else {
		/* it's unexpected */
		_engraving_type = ELBERETH_NOT_HANDLED;
		return;
	}
	_elbereth_count = 1; // we found one already with the first find() call
	while ((pos = messages.find(ELBERETH, pos + 1)) != string::npos)
		++_elbereth_count; // found another elbereth
}
