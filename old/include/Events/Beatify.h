#ifndef EVENT_BEATIFY_H
#define	EVENT_BEATIFY_H

#include "Events/Event.h"

namespace event {
	class Beatify : public Event {
	public:
		static const int ID;

		Beatify(unsigned char key, int priority);
		virtual ~Beatify();

		virtual int id();
		virtual unsigned char key();
		virtual int priority();

	private:
		const unsigned char _key;
		const int _priority;
	};
}
#endif
