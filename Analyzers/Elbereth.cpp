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
Elbereth::Elbereth() : Analyzer("Elbereth"), elbereth_count(0), engraving_type(ELBERETH_MUST_CHECK) {
	/* register events */
	EventBus::registerEvent(ElberethQuery::id, this);
}

void Elbereth::onEvent(Event * const evt) {
	if (evt->getID() == ElberethQuery::id) {
		ElberethQuery * const q = static_cast<ElberethQuery * const> (evt);
		if (World::getLastActionID() != action::Look::id) {
			/* data is outdated */
			engraving_type = ELBERETH_MUST_CHECK;
			elbereth_count = 0;
		}
		q->engraving_type = engraving_type;
		q->number_of_elbereths = elbereth_count;
	}
}

void Elbereth::parseMessages(const string &messages) {
	/* figure out if something is engraved here */
	string::size_type pos = messages.find(MESSAGE_YOU_READ);
	if (pos == string::npos) {
		/* no elbereths here :( */
		elbereth_count = 0;
		engraving_type = ELBERETH_NONE;
		return;
	}
	/* is it written in dust, burned or digged? */
	if (messages.find(MESSAGE_DUSTED_TEXT) != string::npos) {
		/* it's dusted. */
		engraving_type = ELBERETH_DUSTED;
	} else if (messages.find(MESSAGE_BURNED_TEXT) != string::npos) {
		/* it's burned */
		engraving_type = ELBERETH_PERMANENT;
	} else if (messages.find(MESSAGE_DIGGED_TEXT) != string::npos) {
		/* it's digged */
		engraving_type = ELBERETH_SEMIPERM;
	} else if (messages.find(MESSAGE_FROSTED_TEXT) != string::npos) {
		/* it's frosted */
		engraving_type = ELBERETH_DUSTED;
	} else {
		/* it's unexpected */
		engraving_type = ELBERETH_NOT_HANDLED;
		return;
	}
	elbereth_count = 1; // we found one already with the first find() call
	while ((pos = messages.find(ELBERETH, pos + 1)) != string::npos)
		++elbereth_count; // found another elbereth
}
