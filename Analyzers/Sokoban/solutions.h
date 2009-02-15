#define PUSH_hidden(x,y) m->push_back(Move((x)-'A',y));
#define PUSH_N_hidden(x,y,z) for (int i = 0; i < (y); ++i) { PUSH_hidden(x,z) }

#define PUSH_UP(x) PUSH_hidden(x,N)
#define PUSH_UP_N(x,y) PUSH_N_hidden(x,y,N)

#define PUSH_DOWN(x) PUSH_hidden(x,S)
#define PUSH_DOWN_N(x,y) PUSH_N_hidden(x,y,S)

#define PUSH_LEFT(x) PUSH_hidden(x,W)
#define PUSH_LEFT_N(x,y) PUSH_N_hidden(x,y,W)

#define PUSH_RIGHT(x) PUSH_hidden(x,E)
#define PUSH_RIGHT_N(x,y) PUSH_N_hidden(x,y,E)

#define SETUP_MOVES m = new vector<Move>;
#define SAVE_MOVES moves.push_back(*m);

vector<Move> *m;

SETUP_MOVES
#include "solutions/1a.h"
SAVE_MOVES

SETUP_MOVES
#include "solutions/1b.h"
SAVE_MOVES

SETUP_MOVES
#include "solutions/2a.h"
SAVE_MOVES

SETUP_MOVES
#include "solutions/2b.h"
SAVE_MOVES

SETUP_MOVES
#include "solutions/3a.h"
SAVE_MOVES

SETUP_MOVES
#include "solutions/3b.h"
SAVE_MOVES

SETUP_MOVES
#include "solutions/4a.h"
SAVE_MOVES

SETUP_MOVES
#include "solutions/4b.h"
SAVE_MOVES
