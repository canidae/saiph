#ifndef _EVENT_H
#define	_EVENT_H

#include <string>

#define ILLEGAL_EVENT_ID -1
#define ILLEGAL_EVENT_NAME "None"

namespace event {
	class Event {
	public:
		virtual ~Event() {};

		virtual int getID() {return ILLEGAL_EVENT_ID;}
		virtual std::string getName() {return ILLEGAL_EVENT_NAME;}
	};
}
#endif
