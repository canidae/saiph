#ifndef EVENT_QUEST_STATUS_H
#define EVENT_QUEST_STATUS_H

#include "Events/Event.h"

#define QUEST_STATUS_NOT_READY 0
#define QUEST_STATUS_READY 1
#define QUEST_STATUS_GIVEN 2
#define QUEST_STATUS_COMPLETED 3
#define QUEST_STATUS_REJECTED 4

namespace event {

	class QuestStatus : public Event {
	public:
		static const int ID;

		QuestStatus(int nstate, int portal_level) : Event("QuestStatus"), _new_state(nstate), _portal_level(portal_level) {
		}

		virtual ~QuestStatus() {
		}

		virtual int id() {
			return ID;
		}

		int newState() const {
			return _new_state;
		}

		int portalLevel() const {
			return _portal_level;
		}

	private:
		int _new_state;
		int _portal_level;
	};
}
#endif
