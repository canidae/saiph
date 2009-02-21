#include "Monster.h"

/* constructors/destructor */
Monster::Monster(unsigned char symbol, int color, int last_seen) : symbol(symbol), color(color), last_seen(last_seen), visible(false), attitude(ATTITUDE_UNKNOWN), shopkeeper(false), priest(false), minotaur(false) {
}
