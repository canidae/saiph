#ifndef MESSAGEPARSER_H
/* defines */
#define MESSAGEPARSER_H

/* forward declare */
class MessageParser;

/* includes */
#include "Saiph.h"

/* namespace */
using namespace std;

/* this class parses messages */
class MessageParser {
	public:
		/* variables */
		Saiph *saiph;

		/* constructors */
		MessageParser(Saiph *saiph);

		/* methods */
		void parse();

	private:
};
#endif
