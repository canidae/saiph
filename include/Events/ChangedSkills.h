#ifndef EVENT_CHANGED_SKILLS_H
#define EVENT_CHANGED_SKILLS_H

#include <set>
#include "Events/Event.h"

namespace event {

	class ChangedSkills : public Event {
	public:
		static const int ID;

		ChangedSkills() : Event("ChangedSkills") {
		}

		virtual int id() {
			return ID;
		}
	};
}
#endif
