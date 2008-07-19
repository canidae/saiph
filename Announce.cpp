#include "Announce.h"

/* constructors */
Announce::Announce() : data("") {
	announce = ILLEGAL_ANNOUNCE;
	priority = ILLEGAL_PRIORITY;
	key = 0;
	value1 = 0;
}
