#include "Analyzers/Enhance.h"

#include "World.h"
#include "Actions/Enhance.h"

using namespace analyzer;
using namespace std;

/* constructors/destructor */
Enhance::Enhance() : Analyzer("Enhance") {
	World::queueAction(static_cast<action::Action*> (new action::Enhance(this)));
}

/* methods */
void Enhance::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_FEEL_MORE_CONFIDENT, 0) != string::npos) {
		World::queueAction(static_cast<action::Action*> (new action::Enhance(this)));
	}
}
