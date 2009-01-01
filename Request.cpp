#include "Globals.h"
#include "Request.h"

/* constructors/destructor */
Request::Request() : request(ILLEGAL_REQUEST), priority(ILLEGAL_PRIORITY), value(0), beatitude(0), sustain(false), only_unknown_enchantment(false), key(0), data(""), coordinate() {
}
