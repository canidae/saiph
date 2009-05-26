#ifndef _ELBERETHQUERY_H
#define	_ELBERETHQUERY_H

#include "Event.h"

#define ENGRAVING_DUSTED 1
#define ENGRAVING_SEMIPERM 2
#define ENGRAVING_PERMANENT 3

namespace event {
	class ElberethQuery : public Event {
	public:
		static int id;

		int number_of_elbereths, engraving_type;

		//initialize these to invalid values to spot bugs
		ElberethQuery() : number_of_elbereths(-1), engraving_type(-1) {}
		virtual ~ElberethQuery() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "ElberethQuery";}
	};
}

#endif	/* _ELBERETHQUERY_H */

