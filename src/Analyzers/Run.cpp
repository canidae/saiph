#include "Analyzers/Run.h"

#include "EventBus.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Look.h"
#include "Actions/Move.h"
#include "Actions/Search.h"
#include "Events/RunAway.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Run::Run : Analyzer("Run") {
	/* register events */
	EventBus::registerEvent(RunAway::ID, this);
}

void Run::onEvent(Event * const evt) {
	if (evt->id() == RunAway::ID) {
		RunAway * const e = static_cast<RunAway * const> (evt);

