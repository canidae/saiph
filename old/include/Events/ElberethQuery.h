#ifndef EVENT_ELBERETH_QUERY_H
#define EVENT_ELBERETH_QUERY_H

#include "Events/Event.h"

#define ELBERETH_NOT_HANDLED -1
#define ELBERETH_MUST_CHECK 0
#define ELBERETH_INEFFECTIVE 1
#define ELBERETH_NONE 2
#define ELBERETH_DUSTED 3
#define ELBERETH_SEMIPERM 4
#define ELBERETH_PERMANENT 5

namespace event {
	class ElberethQuery : public Event {
	public:
		static const int ID;

		ElberethQuery();
		virtual ~ElberethQuery();

		virtual int id();
		virtual int count();
		virtual int count(int count);
		virtual int type();
		virtual int type(int type);

	private:
		int _count;
		int _type;
	};
}
#endif
