#include "Events/ChangedSkills.h"

using namespace event;

/* constructors/destructor */
ChangedSkills::ChangedSkills() : Event("ChangedSkills") {
}

ChangedSkills::~ChangedSkills() {
}

/* methods */
int ChangedSkills::id() {
	return ID;
}
