#include "Monster.h"

/* constructors/destructor */
Monster::Monster(unsigned char symbol, int color, int last_seen, bool visible, int attitude, bool shopkeeper, bool priest, bool minotaur) : symbol(symbol), color(color), last_seen(last_seen), visible(visible), attitude(attitude), shopkeeper(shopkeeper), priest(priest), minotaur(minotaur) {
}
