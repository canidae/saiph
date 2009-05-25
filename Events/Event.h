#ifndef _EVENT_H
#define	_EVENT_H

#include <string>

#define ILLEGAL_EVENT_ID -1

namespace event {
	class Event {
	public:
		virtual ~Event() {};

		static void init();
		static void destroy();
		virtual int getID() = 0;
		virtual std::string getName() = 0;

	private:
		static bool initialized;
	};
}
#endif
