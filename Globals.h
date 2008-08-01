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
#define LEVEL_NAME_SIZE 16
#define MAX_BRANCHES 7
#define MAX_DUNGEON_DEPTH 64
#define BRANCH_MAIN 0
#define BRANCH_MINES 1
#define BRANCH_SOKOBAN 2
#define BRANCH_VLAD 3
#define BRANCH_WIZARD 4
#define BRANCH_ASTRAL 5
#define BRANCH_QUEST 6
/* colors & attributes */
#define NO_COLOR 0
#define INVERSE 7
#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37
/* beatitude */
#define CURSED -1
#define UNCURSED 0
#define BLESSED 1
#define BEATITUDE_UNKNOWN 2
/* alignment */
#define CHAOTIC -1
#define NEUTRAL 0
#define LAWFUL 1
/* encumbrance */
#define UNENCUMBERED 0
#define BURDENED 1
#define STRESSED 2
#define STRAINED 3
#define OVERTAXED 4
#define OVERLOADED 5
/* hunger */
#define FAINTING -3
#define WEAK -2
#define HUNGRY -1
#define CONTENT 0
#define SATIATED 1
#define OVERSATIATED 2
/* movement */
#define MOVE_NW 'y'
#define MOVE_N 'k'
#define MOVE_NE 'u'
#define MOVE_W 'h'
#define MOVE_E 'l'
#define MOVE_SW 'b'
#define MOVE_S 'j'
#define MOVE_SE 'n'
#define MOVE_UP '<'
#define MOVE_DOWN '>'
#define ILLEGAL_MOVE 0
/* actions */
#define ILLEGAL_ACTION 0
#define DIP "#dip\n"
#define DROP 'D'
#define EAT 'e'
#define ENGRAVE 'E'
#define KICK 4
#define LOOK ":"
#define OPEN 'o'
#define PICKUP ','
#define PRAY "#pray\n"
#define REST '.'
#define SEARCH 's'
#define THROW 't'
/* common answers */
#define HANDS '-'
#define NO 'n'
#define YES 'y'
/* elbereth */
#define ELBERETH "Elbereth" // note the missing "\n". we'll search for Elbereth, can't have a "\n" then
/* dungeon symbols */
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
#define UNKNOWN_TILE 136
#define UNKNOWN_TILE_DIAGONALLY_PASSABLE 137
/* item symbols */
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
/* special monster symbols */
#define ILLEGAL_MONSTER 0
#define PET 138
#define PLAYER 139
/* priorities */
/*    0-199: explore/random/do stuff we normally wouldn't do (melee floating eye)
 *  200-399: move around, eg. attacking & looting
 *  400-599: don't move. engrave for protection, attack if not moving and protected, feed, etc
 *  600-799: don't panic, rest for hp, fix bad stuff
 *  800-999: panic, all hell is loose, pray, teleport, get the hell away
 * 1000->  : zero-turn actions, defined below
 */
#define ILLEGAL_PRIORITY -1
#define PRIORITY_CONTINUE_ACTION 1004
#define PRIORITY_SELECT_ITEM 1003
#define PRIORITY_CLOSE_ITEM_LIST 1002
#define PRIORITY_LOOK 1001
#define PRIORITY_MAX 1000
/* messages */
#define MESSAGE_BUMP_INTO_DOOR "  Ouch!  You bump into a door.  "
#define MESSAGE_BURNED_TEXT "  Some text has been burned into the floor here.  "
#define MESSAGE_CHOOSE_DIRECTION "  In what direction?  "
#define MESSAGE_DIGGED_TEXT "  Something is engraved here on the floor.  "
#define MESSAGE_DIP_IN_FOUNTAIN " into the fountain? [yn] (n)  " // begins with "  Dip the %s ..."
#define MESSAGE_DOOR_CLOSES "  The door closes.  "
#define MESSAGE_DOOR_LOCKED "  This door is locked.  "
#define MESSAGE_DOOR_OPENS "  The door opens.  "
#define MESSAGE_DOOR_RESISTS "  The door resists!  "
#define MESSAGE_DROP_WHICH_ITEMS "  What would you like to drop?  "
#define MESSAGE_DUSTED_TEXT "  Something is written here in the dust.  "
#define MESSAGE_DYWYPI "  Do you want your possessions identified? [ynq] "
#define MESSAGE_ENGRAVE_ADD "  Do you want to add to the current engraving? " // followed by "[ynq] (y)" or something
#define MESSAGE_ENGRAVE_DUST "  What do you want to write in the dust here?  "
#define MESSAGE_ENGRAVE_DUST_ADD "  What do you want to add to the writing in the dust here?  "
#define MESSAGE_ENGRAVE_WITH "  What do you want to write with? " // followed by "[- abcde...]" or something
#define MESSAGE_FOUNTAIN_DRIES_UP "  The fountain dries up!  "
#define MESSAGE_FOUNTAIN_DRIES_UP2 "  As the hand retreats, the fountain disappears!  "
#define MESSAGE_FOUNTAIN_HERE "  There is a fountain here.  "
#define MESSAGE_GOT_NOTHING_TO_EAT "  You don't have anything to eat.  "
#define MESSAGE_KICK_DOOR_FAIL "  WHAMMM!!!  "
#define MESSAGE_KICK_DOOR_OPEN "  As you kick the door, it crashes open!  "
#define MESSAGE_MANY_OBJECTS_HERE "  There are many objects here.  "
#define MESSAGE_NO_DOOR "  You see no door there.  "
#define MESSAGE_NOT_CARRYING_ANYTHING "  Not carrying anything.  "
#define MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD "  Not carrying anything except gold.  "
#define MESSAGE_OPEN_DOOR_HERE "  There is an open door here.  "
#define MESSAGE_PICK_UP_WHAT "  Pick up what?  "
#define MESSAGE_RECEIVED_EXCALIBUR "  From the murky depths, a hand reaches up to bless the sword.  " // wohoo
#define MESSAGE_SEVERAL_OBJECTS_HERE "  There are several objects here.  "
#define MESSAGE_SHOP_ON_LEVEL1 "  You hear someone cursing shoplifters.  "
#define MESSAGE_SHOP_ON_LEVEL2 "  You hear the chime of a cash register.  "
#define MESSAGE_SHOP_ON_LEVEL3 "  You hear Neiman and Marcus arguing!  "
#define MESSAGE_STAIRCASE_DOWN_HERE "  There is a staircase down here.  "
#define MESSAGE_STAIRCASE_UP_HERE "  There is a staircase up here.  "
#define MESSAGE_THERE_IS_NOTHING_HERE "  There is nothing here to pick up.  "
#define MESSAGE_THINGS_THAT_ARE_HERE "  Things that are here:  "
#define MESSAGE_THINGS_THAT_YOU_FEEL_HERE "  Things that you feel here:  "
#define MESSAGE_VAULT_GUARD "  \"Hello stranger, who are you?\" -  "
#define MESSAGE_WHAT_TO_DIP "  What do you want to dip? " // followed by "[abcd...]"
#define MESSAGE_WHAT_TO_EAT "  What do you want to eat? " // followed by "[abcd...]"
#define MESSAGE_WHAT_TO_THROW "  What do you want to throw? " // followed by "[abc...]"
#define MESSAGE_YOU_DONT_HAVE "  You don't have that object.  "
#define MESSAGE_YOU_FEEL_HERE "  You feel here " // not two spaces here as it's followed by eg. "a lichen corpse"
#define MESSAGE_YOU_READ "  You read:" // followed by the text. no space after ":" because message parser is a bit buggy
#define MESSAGE_YOU_SEE_HERE "  You see here " // not two spaces here as it's followed by eg. "a lichen corpse"
#define MESSAGE_YOU_SEE_NO_OBJECTS "  You see no objects here.  "

#define MESSAGE_YOU_DID_DAMAGE "  \"Cad!  You did 400 zorkmids worth of damage!\"  Pay?  [yn] (n)  "

#endif
