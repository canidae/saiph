#ifndef ELBERETH_H
/* defines */
#define ELBERETH_H
/* messages */
#define ELBERETH_BURNED_TEXT "  Some text has been burned into the floor here.  "
#define ELBERETH_DIGGED_TEXT "  Something is engraved here on the floor.  "
#define ELBERETH_DUSTED_TEXT "  Something is written here in the dust.  "
#define ELBERETH_FROSTED_TEXT "  Something is written here in the frost.  "
#define ELBERETH_YOU_READ "  You read:"
/* Elbereth */
#define ELBERETH_ELBERETH "Elbereth"

/* forward declare */
class Elbereth;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* engraves elbereth */
class Elbereth : public Analyzer {
	public:
		/* constructors */
		Elbereth(Saiph *saiph);

		/* methods */
		void complete();
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		int sequence;
		int last_look_internal_turn;
		int elbereth_count;
		bool burned;
		bool digged;
		bool dusted;
		bool frosted;
		bool append;

		/* methods */
		bool canEngrave();
};
#endif
