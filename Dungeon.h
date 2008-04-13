#ifndef DUNGEON_H
/* defines */
#define DUNGEON_H
/* buffer for various stuff */
#define BUFFER_SIZE 65536
/* dungeon limits */
#define ROWS 24
#define COLS 80
#define MAP_ROW_START 1 // where the actual map start
#define MAP_ROW_END 21 // and end
#define ATTRIBUTES_ROW 22
#define STATUS_ROW 23
/* map symbols */
#define SOLID_ROCK ' '
#define VERTICAL_WALL '|'
#define HORIZONTAL_WALL '-'
#define FLOOR '.'
#define OPEN_DOOR -128
#define CLOSED_DOOR '7'
#define IRON_BARS -127
#define TREE -126
#define CORRIDOR '#'
#define STAIRS_UP '<'
#define STAIRS_DOWN '>'
#define ALTAR '_'
#define GRAVE '\\'
#define THRONE -125
#define SINK -124
#define FOUNTAIN '{'
#define WATER '}'
#define ICE -123
#define LAVA -122
#define LOWERED_DRAWBRIDGE -121
#define RAISED_DRAWBRIDGE '8'
#define TRAP '^'
/* colours & attributes */
#define INVERSE 7
#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37
/* monsters */
#define MONSTER_a 'a'
#define MONSTER_b 'b'
#define MONSTER_c 'c'
#define MONSTER_d 'd'
#define MONSTER_e 'e'
#define MONSTER_f 'f'
#define MONSTER_g 'g'
#define MONSTER_h 'h'
#define MONSTER_i 'i'
#define MONSTER_j 'j'
#define MONSTER_k 'k'
#define MONSTER_l 'l'
#define MONSTER_m 'm'
#define MONSTER_n 'n'
#define MONSTER_o 'o'
#define MONSTER_p 'p'
#define MONSTER_q 'q'
#define MONSTER_r 'r'
#define MONSTER_s 's'
#define MONSTER_t 't'
#define MONSTER_u 'u'
#define MONSTER_v 'v'
#define MONSTER_w 'w'
#define MONSTER_x 'x'
#define MONSTER_y 'y'
#define MONSTER_z 'z'
#define MONSTER_A 'A'
#define MONSTER_B 'B'
#define MONSTER_C 'C'
#define MONSTER_D 'D'
#define MONSTER_E 'E'
#define MONSTER_F 'F'
#define MONSTER_G 'G'
#define MONSTER_H 'H'
#define MONSTER_I 'I'
#define MONSTER_J 'J'
#define MONSTER_K 'K'
#define MONSTER_L 'L'
#define MONSTER_M 'M'
#define MONSTER_N 'N'
#define MONSTER_O 'O'
#define MONSTER_P 'P'
#define MONSTER_Q 'Q'
#define MONSTER_R 'R'
#define MONSTER_S 'S'
#define MONSTER_T 'T'
#define MONSTER_U 'U'
#define MONSTER_V 'V'
#define MONSTER_W 'W'
#define MONSTER_X 'X'
#define MONSTER_Y 'Y'
#define MONSTER_Z 'Z'
#define MONSTER_AT '@'
#define MONSTER_6 '6'
#define MONSTER_APOSTROPHE '\''
#define MONSTER_AND '&'
#define MONSTER_SEMICOLON ';'
#define MONSTER_COLON ':'
#define MONSTER_TILDE '~'
/* objects */
#define ILLEGAL 'm'
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
#define PET -120
#define PLAYER -119


/* forward declare */
class Dungeon;

/* includes */
#include "Player.h"

/* namespace */
using namespace std;

/* a dungeon represents a map, including attribute/status rows */
class Dungeon {
	public:
		/* variables */
		char map[ROWS][COLS + 1];
		char colour[ROWS][COLS];
		char messages[BUFFER_SIZE];
		bool menu;
		bool question;
		Player player;

		/* constructors */
		Dungeon();

		/* methods */
		void clone(const Dungeon *dungeon);
};
#endif
