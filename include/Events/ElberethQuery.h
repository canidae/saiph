#ifndef EVENT_ELBERETH_QUERY_H
#define EVENT_ELBERETH_QUERY_H

#include "Event.h"

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

		ElberethQuery() : Event("ElberethQuery"), _count(0), _type(ELBERETH_NOT_HANDLED) {
		}

		virtual ~ElberethQuery() {
		}

		virtual int id() {
			return ID;
		}

		virtual int count() {
			return _count;
		}

		virtual int count(int count) {
			_count = count;
			return this->count();
		}

		virtual int type() {
			return _type;
		}

		virtual int type(int type) {
			_type = type;
			return this->type();
		}

	private:
		int _count;
		int _type;
	};
}
#endif
