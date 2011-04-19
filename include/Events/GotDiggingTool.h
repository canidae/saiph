#ifndef EVENT_QUEST_GOT_DIGGING_TOOL_H
#define EVENT_QUEST_GOT_DIGGING_TOOL_H

#include "Events/Event.h"

namespace event {

	class GotDiggingTool : public Event {
	public:
		static const int ID;

		GotDiggingTool(unsigned char key) : Event("GotDiggingTool"), _key(key) {
		}

		virtual int id() {
			return ID;
		}

		unsigned char key() const {
			return _key;
		}

	private:
		unsigned char _key;
	};
}
#endif
