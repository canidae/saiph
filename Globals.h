#ifndef GLOBALS_H
/* defines */
#define GLOBALS_H
/* dungeon limits */
#define ROWS 24
#define COLS 80
#define MAP_COL_BEGIN 0 // where the actual map width start
#define MAP_COL_END 79 // where the actual map width end
#define MAP_ROW_BEGIN 1 // where the actual map height begin
#define MAP_ROW_END 21 // where the actual map height end
#define ATTRIBUTES_ROW 22
#define STATUS_ROW 23
/* colors & attributes */
#define NOCOLOR 0
#define INVERSE 7
#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37
/* map symbols */
#define SOLID_ROCK ' '
#define VERTICAL_WALL '|'
#define HORIZONTAL_WALL '-'
#define FLOOR '.'
#define OPEN_DOOR 128
#define CLOSED_DOOR '7'
#define IRON_BARS 129
#define TREE 130
#define CORRIDOR '#'
#define STAIRS_UP '<'
#define STAIRS_DOWN '>'
#define ALTAR '_'
#define GRAVE '\\'
#define THRONE 131
#define SINK 132
#define FOUNTAIN '{'
#define WATER '}'
#define ICE 133
#define LAVA 134
#define LOWERED_DRAWBRIDGE 135
#define RAISED_DRAWBRIDGE '8'
#define TRAP '^'
/* items */
#define ILLEGAL_ITEM 0
#define WEAPON ')'
#define ARMOR '['
#define RING '='
#define AMULET '"'
#define TOOL '('
#define FOOD '%'
#define POTION '!'
#define SCROLL '?'
#define SPELLBOOK '+'
#define WAND '/'
#define GOLD '$'
#define GEM '*'
#define STATUE '`'
#define BOULDER '0'
#define IRON_BALL '`'
#define CHAINS '`'
#define VENOM '`'
/* special "monsters" */
#define ILLEGAL_MONSTER 0
#define PET 136
#define PLAYER 137

#endif
