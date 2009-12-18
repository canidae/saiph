#include "Analyzers/Elbereth.h"

#include <stdlib.h>
#include "EventBus.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Look.h"
#include "Data/Monster.h"
#include "Events/ElberethQuery.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Elbereth::Elbereth() : Analyzer("Elbereth"), _elbereth_count(0), _engraving_type(ELBERETH_MUST_CHECK) {
	/* register events */
	EventBus::registerEvent(ElberethQuery::ID, this);
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
	_elbereth_count = 0;
	while ((pos = messages.find(ELBERETH, pos + 1)) != string::npos)
		++_elbereth_count; // found another elbereth
}

void Elbereth::onEvent(Event * const evt) {
	if (evt->id() == ElberethQuery::ID) {
		ElberethQuery * const q = static_cast<ElberethQuery * const> (evt);
		if (World::lastActionID() != action::Look::ID) {
			/* data is outdated */
			_engraving_type = ELBERETH_MUST_CHECK;
			_elbereth_count = 0;
		}
		if (Saiph::engulfed() || Saiph::blind() || Saiph::hallucinating() || Saiph::confused() || Saiph::stunned() || (Saiph::extrinsics() & PROPERTY_LEVITATION) != 0 || World::level().tile().symbol() == GRAVE || World::level().tile().symbol() == ALTAR || World::level().tile().symbol() == FOUNTAIN || World::level().tile().symbol() == WATER || World::level().tile().symbol() == LAVA) {
			_engraving_type = ELBERETH_INEFFECTIVE;
			_elbereth_count = 0;
		} else {
			for (map<Point, Monster>::const_iterator m = World::level().monsters().begin(); m != World::level().monsters().end(); ++m) {
				if (m->second.attitude() == FRIENDLY)
					continue; // friendly monsters won't attack us
				if (m->second.symbol() == 'X')
					continue; // ghosts respect elbereth (won't get any monster data from ghosts)
				if (m->second.data() != NULL && !m->second.data()->ignoresElbereth())
					continue; // monster won't ignore Elbereth
				if (abs(m->first.row() - Saiph::position().row()) > 1 || abs(m->first.col() - Saiph::position().col() > 1))
					continue; // not next to us
				if (m->second.symbol() != '@' && m->second.data() != NULL && m->second.data()->name().find("were") == 0)
					continue; // werefoo in animal form respects Elbereth
				/* this monster ignores elbereth and is next to us */
				_engraving_type = ELBERETH_INEFFECTIVE;
				_elbereth_count = 0;
				break;
			}
		}
		q->type(_engraving_type);
		q->count(_elbereth_count);
	}
}
