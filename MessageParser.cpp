#include "MessageParser.h"

/* constructors */
MessageParser::MessageParser(Saiph *saiph) {
	this->saiph = saiph;
}

/* destructors */
MessageParser::~MessageParser() {
}

/* methods */
void MessageParser::parse() {
	if (saiph->world->messages[0] != '\0')
		cerr << "Messages: " << saiph->world->messages << endl;
}
