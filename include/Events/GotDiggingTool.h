#ifndef EVENT_GOT_DIGGING_TOOL_H
#define EVENT_GOT_DIGGING_TOOL_H

#include "Events/Event.h"

namespace event {
	class GotDiggingTool : public Event {
	public:
		static const int ID;

		GotDiggingTool(unsigned char key);
		virtual ~GotDiggingTool();

		virtual int id();
		unsigned char key() const;

	private:
		unsigned char _key;
	};
}
#endif
