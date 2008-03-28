#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

/* forward declare */
class MessageParser;

#include "Saiph.h"

using namespace std;

class MessageParser {
	public:
		/* variables */
		Saiph *saiph;

		/* constructors */
		MessageParser(Saiph *saiph);

		/* destructors */
		~MessageParser();

		/* methods */
		void parse();

	private:
};

#endif
