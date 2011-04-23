#include "Events/QuestStatus.h"

using namespace event;

/* constructors/destructor */
QuestStatus::QuestStatus(int nstate, int portal_level) : Event("QuestStatus"), _new_state(nstate), _portal_level(portal_level) {
}

QuestStatus::~QuestStatus() {
}

/* methods */
int QuestStatus::id() {
	return ID;
}

int QuestStatus::newState() const {
	return _new_state;
}

int QuestStatus::portalLevel() const {
	return _portal_level;
}
