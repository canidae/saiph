#ifndef _EVENT_H
#define	_EVENT_H

#define ILLEGAL_EVENT_TYPE -1

namespace event {
	class Event {
	public:
		Event() {};
		virtual ~Event() {};

		virtual int getType() {return ILLEGAL_EVENT_TYPE;}
	};
}
#endif
