#ifndef ELBERETH_H
#define ELBERETH_H

#include <string>
#include "Analyzer.h"

/* messages */
#define ELBERETH_BURNED_TEXT "  Some text has been burned into the floor here.  "
#define ELBERETH_DIGGED_TEXT "  Something is engraved here on the floor.  "
#define ELBERETH_DUSTED_TEXT "  Something is written here in the dust.  "
#define ELBERETH_FROSTED_TEXT "  Something is written here in the frost.  "
#define ELBERETH_YOU_READ "  You read:"
/* Elbereth */
#define ELBERETH_ELBERETH "Elbereth"

class Request;
class Saiph;

namespace analyzer {
	class Elbereth : public Analyzer {
	public:
		Elbereth();

		void parseMessages(const std::string &messages);
		void onEvent(event::Event *const evt);

	private:
		int elbereth_count;
		int engraving_type;
		int real_turn_look;
	};
}
#endif
