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
Elbereth::Elbereth() : Analyzer("Elbereth"), elbereth_count(0), engraving_type(ENGRAVING_MUST_CHECK), real_turn_look(-1) {
	/* register events */
	EventBus::registerEvent(ElberethQuery::id, this);
}

void Elbereth::onEvent(Event *const evt) {
	if (evt->getID() == ElberethQuery::id) {
		ElberethQuery *const q = static_cast<ElberethQuery *const>(evt);
		if (real_turn_look != World::real_turn) {
			/* data is outdated */
			engraving_type = ENGRAVING_MUST_CHECK;
			elbereth_count = 0;
			World::setAction(static_cast<action::Action *>(new action::Look(this)));
		}
		q->engraving_type = engraving_type;
		q->number_of_elbereths = elbereth_count;
	}
}

void Elbereth::parseMessages(const string &messages) {
	/* figure out if something is engraved here */
	string::size_type pos = messages.find(ELBERETH_YOU_READ, 0);
	if (pos == string::npos) {
		/* no elbereths here :( */
		elbereth_count = 0;
		engraving_type = -1;
		return;
	}
	/* is it written in dust, burned or digged? */
	if (messages.find(ELBERETH_DUSTED_TEXT, 0) != string::npos) {
		/* it's dusted. */
		engraving_type = ENGRAVING_DUSTED;
	} else if (messages.find(ELBERETH_BURNED_TEXT, 0) != string::npos) {
		/* it's burned */
		engraving_type = ENGRAVING_PERMANENT;
	} else if (messages.find(ELBERETH_DIGGED_TEXT, 0) != string::npos) {
		/* it's digged */
		engraving_type = ENGRAVING_SEMIPERM;
	} else if (messages.find(ELBERETH_FROSTED_TEXT, 0) != string::npos) {
		/* it's frosted */
		engraving_type = ENGRAVING_DUSTED;
	} else {
		/* it's unexpected */
		engraving_type = -1;
		return;
	}
	real_turn_look = World::real_turn;
	elbereth_count = 1; // we found one already with the first find() call
	while ((pos = messages.find(ELBERETH_ELBERETH, pos + 1)) != string::npos)
		++elbereth_count; // found another elbereth
}
