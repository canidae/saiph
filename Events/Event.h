#ifndef _EVENT_H
#define	_EVENT_H

#define ILLEGAL_EVENT_ID -1

namespace event {
	class Event {
	public:
		Event() {};
		virtual ~Event() {};

		virtual int getID() {return ILLEGAL_EVENT_ID;}
	};
}
#endif
