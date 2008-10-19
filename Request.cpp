#include "Globals.h"
#include "Request.h"

/* constructors/destructor */
Request::Request() : request(ILLEGAL_REQUEST), priority(ILLEGAL_PRIORITY), value(0), status(0), key(0), data("") {
}
