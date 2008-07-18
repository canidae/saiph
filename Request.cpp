#include "Request.h"

/* constructors */
Request::Request() : data("") {
	request = ILLEGAL_REQUEST;
	priority = ILLEGAL_PRIORITY;
	value1 = 0;
}
