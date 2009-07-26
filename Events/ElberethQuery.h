#ifndef EVENT_ELBERETH_QUERY_H
#define EVENT_ELBERETH_QUERY_H

#include "Event.h"

#define ELBERETH_NOT_HANDLED -1
#define ELBERETH_MUST_CHECK 0
#define ELBERETH_NONE 1
#define ELBERETH_DUSTED 2
#define ELBERETH_SEMIPERM 3
#define ELBERETH_PERMANENT 4

namespace event {
	class ElberethQuery : public Event {
	public:
		static int id;

		int number_of_elbereths;
		int engraving_type;

		// initialize these to invalid values to spot bugs
		ElberethQuery() : number_of_elbereths(0), engraving_type(ELBERETH_NOT_HANDLED) {}
		virtual ~ElberethQuery() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "ElberethQuery";}
	};
}
#endif
