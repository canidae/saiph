#ifndef EVENT_QUEST_STATUS_H
#define EVENT_QUEST_STATUS_H

#include "Events/Event.h"

#define QUEST_STATUS_NONE 0
#define QUEST_STATUS_REJECTED 1
#define QUEST_STATUS_ACCEPTED 2
#define QUEST_STATUS_COMPLETED 3

namespace event {

	class QuestStatus : public Event {
	public:
		static const int ID;

		QuestStatus(int nstate) : Event("QuestStatus"), _new_state(nstate) {
		}

		virtual ~QuestStatus() {
		}

		virtual int id() {
			return ID;
		}

		int newState() const {
			return _new_state;
		}

	private:
		int _new_state;
	};
}
#endif
