#ifndef _ELBERETHQUERY_H
#define	_ELBERETHQUERY_H

#include "Event.h"

#define ENGRAVING_NOT_HANDLED -1
#define ENGRAVING_MUST_CHECK 0
#define ENGRAVING_DUSTED 1
#define ENGRAVING_SEMIPERM 2
#define ENGRAVING_PERMANENT 3

namespace event {
	class ElberethQuery : public Event {
	public:
		static int id;

		int number_of_elbereths;
		int engraving_type;

		// initialize these to invalid values to spot bugs
		ElberethQuery() : number_of_elbereths(0), engraving_type(ENGRAVING_NOT_HANDLED) {}
		virtual ~ElberethQuery() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "ElberethQuery";}
	};
}
#endif
