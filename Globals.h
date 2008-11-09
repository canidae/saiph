#ifndef GLOBALS_H
#define GLOBALS_H
/* dungeon limits */
#define ROWS 24
#define COLS 80
#define MAP_COL_BEGIN 0 // where the actual map width start
#define MAP_COL_END 79 // where the actual map width end
#define MAP_ROW_BEGIN 1 // where the actual map height begin
#define MAP_ROW_END 21 // where the actual map height end
#define BRANCH_MAIN 0
#define BRANCH_MINES 1
#define BRANCH_SOKOBAN 2
#define BRANCH_ROGUE 3
#define BRANCH_VLAD 4
#define BRANCH_WIZARD 5
#define BRANCH_ASTRAL 6
#define BRANCH_QUEST 7
/* unknown tracked symbol value */
#define UNKNOWN_SYMBOL_VALUE INT_MIN
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
#define BEATITUDE_UNKNOWN 0x1
#define CURSED 0x2
#define UNCURSED 0x4
#define BLESSED 0x8
/* alignment */
#define CHAOTIC -1
#define NEUTRAL 0
#define LAWFUL 1
/* attitude */
#define ATTITUDE_UNKNOWN 0x1
#define HOSTILE 0x2
#define FRIENDLY 0x4
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
/* armor slots */
#define ARMOR_SLOTS 7
#define ARMOR_SHIRT 0
#define ARMOR_SUIT 1
#define ARMOR_CLOAK 2
#define ARMOR_BOOTS 3
#define ARMOR_GLOVES 4
#define ARMOR_HELMET 5
#define ARMOR_SHIELD 6
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
#define MOVE_NOWHERE '.'
#define ILLEGAL_MOVE 0
/* actions */
#define ILLEGAL_ACTION 0
#define APPLY "a"
#define CLOSE_PAGE " "
#define DIP "#dip\n"
#define DROP "D"
#define EAT "e"
#define ENGRAVE "E"
#define ENHANCE "#enhance\n"
#define INVENTORY "i"
#define KICK ""
#define LOOK ":"
#define OPEN "o"
#define PICKUP ","
#define PRAY "#pray\n"
#define PUT_ON "P"
#define REMOVE "R"
#define REST "."
#define SEARCH "s"
#define TAKE_OFF "T"
#define THROW "t"
#define WEAR "W"
#define WIELD "w"
/* common answers */
#define HANDS "-"
#define NO "n"
#define YES "y"
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
#define UNKNOWN_TILE_UNPASSABLE 138
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
#define PET 139
/* priorities */
/*    0-199: explore/random/do stuff we normally wouldn't do (melee floating eye)
 *  200-399: move around, eg. attacking & looting
 *  400-599: don't move. engrave for protection, attack if not moving and protected, feed, etc
 *  600-799: don't panic, rest for hp, fix bad stuff
 *  800-999: panic, all hell is loose, pray, teleport, get the hell away
 * 1000->  : zero-turn actions, defined below
 */
#define ILLEGAL_PRIORITY -1
#define PRIORITY_CONTINUE_ACTION 1003
#define PRIORITY_SELECT_ITEM 1002
#define PRIORITY_CLOSE_PAGE 1001
#define PRIORITY_LOOK 1000
#define PRIORITY_MAX 999
#define PRIORITY_MIN 0
/* messages */
#define MESSAGE_CHOOSE_DIRECTION "  In what direction?  "
#define MESSAGE_CURRENT_SKILLS "  Current skills:  "
#define MESSAGE_DIP_IN_FOUNTAIN " into the fountain? [yn] (n)  " // begins with "  Dip the %s ..."
#define MESSAGE_DOOR_LOCKED "  This door is locked.  "
#define MESSAGE_DROP_WHICH_ITEMS "  What would you like to drop?  "
#define MESSAGE_DYWYPI "  Do you want your possessions identified? [ynq] "
#define MESSAGE_ENGRAVE_ADD "  Do you want to add to the current engraving? " // followed by "[ynq] (y)" or something
#define MESSAGE_ENGRAVE_DUST "  What do you want to write in the dust here?  "
#define MESSAGE_ENGRAVE_DUST_ADD "  What do you want to add to the writing in the dust here?  "
#define MESSAGE_ENGRAVE_FROST "  What do you want to write in the frost here?  "
#define MESSAGE_ENGRAVE_FROST_ADD "  What do you want to add to the writing in the frost here?  "
#define MESSAGE_ENGRAVE_WITH "  What do you want to write with? " // followed by "[- abcde...]" or something
#define MESSAGE_FEEL_MORE_CONFIDENT "  You feel more confident in your "
#define MESSAGE_FOR_INSTRUCTIONS "  (For instructions type a ?)  "
#define MESSAGE_FOR_WHAT_DO_YOU_WISH "  For what do you wish?  "
#define MESSAGE_GUARD_DISAPPEARS "  Suddenly, the guard disappears.  "
#define MESSAGE_HELLO_STRANGER "  \"Hello stranger, who are you?\" -  "
#define MESSAGE_NOT_CARRYING_ANYTHING "  Not carrying anything.  "
#define MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD "  Not carrying anything except gold.  "
#define MESSAGE_NOTHING_HAPPENS "  Nothing happens.  "
#define MESSAGE_PICK_A_SKILL "  Pick a skill to advance:  "
#define MESSAGE_PICK_UP_WHAT "  Pick up what?  "
#define MESSAGE_RECEIVED_EXCALIBUR "  From the murky depths, a hand reaches up to bless the sword.  " // wohoo
#define MESSAGE_TELEPORT_WHERE "  To what position do you want to be teleported?  "
#define MESSAGE_TEXT_BURNED "  Some text has been burned into the floor here.  "
#define MESSAGE_TEXT_DIGGED "  Something is engraved here on the floor.  "
#define MESSAGE_TEXT_DUSTED "  Something is written here in the dust.  "
#define MESSAGE_TEXT_FROSTED "  Something is written here in the frost.  "
#define MESSAGE_UNLOCK_IT "  Unlock it? [yn] (n)  "
#define MESSAGE_WHAT_TO_APPLY "  What do you want to use or apply? "
#define MESSAGE_WHAT_TO_DIP "  What do you want to dip? " // followed by "[abcd...]"
#define MESSAGE_WHAT_TO_EAT "  What do you want to eat? " // followed by "[abcd...]"
#define MESSAGE_WHAT_TO_REMOVE "  What do you want to remove? " // followed by "[abcd...]"
#define MESSAGE_WHAT_TO_PUT_ON "  What do you want to put on? " // followed by "[abcd...]"
#define MESSAGE_WHAT_TO_TAKE_OFF "  What do you want to take off? " // followed by "[abcd...]"
#define MESSAGE_WHAT_TO_THROW "  What do you want to throw? " // followed by "[abc...]"
#define MESSAGE_WHAT_TO_WEAR "  What do you want to wear? " // followed by "[abc...]"
#define MESSAGE_WHAT_TO_WIELD "  What do you want to wield? "
#define MESSAGE_WHICH_RING_FINGER "  Which ring-finger, Right or Left? [rl]  "
#define MESSAGE_YOU_READ "  You read:"
#define MESSAGE_YOU_WERE_WEARING "  You were wearing " // followed by what we took off

#endif
