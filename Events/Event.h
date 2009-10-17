#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H

#include <string>

#define ILLEGAL_EVENT_ID -1

namespace event {

	class Event {
	public:

		Event(const std::string &name) : _name(name) {
		}

		virtual ~Event() {
		}

		virtual int id() = 0;

		virtual const std::string &name() {
			return _name;
		}

	private:
		std::string _name;
	};
}
#endif
