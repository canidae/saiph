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
#define KNAPSACK_LIMIT 52

/* unknown tracked symbol value */
#define UNKNOWN_SYMBOL_VALUE INT_MIN

/* colors & attributes */
#define NO_COLOR 0
#define BOLD 1
#define INVERSE 7
#define BOLD_OFFSET 60
#define INVERSE_OFFSET 10
#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37
#define INVERSE_BLACK 40
#define INVERSE_RED 41
#define INVERSE_GREEN 42
#define INVERSE_YELLOW 43
#define INVERSE_BLUE 44
#define INVERSE_MAGENTA 45
#define INVERSE_CYAN 46
#define INVERSE_WHITE 47
#define BOLD_BLACK 90
#define BOLD_RED 91
#define BOLD_GREEN 92
#define BOLD_YELLOW 93
#define BOLD_BLUE 94
#define BOLD_MAGENTA 95
#define BOLD_CYAN 96
#define BOLD_WHITE 97
#define INVERSE_BOLD_BLACK 100
#define INVERSE_BOLD_RED 101
#define INVERSE_BOLD_GREEN 102
#define INVERSE_BOLD_YELLOW 103
#define INVERSE_BOLD_BLUE 104
#define INVERSE_BOLD_MAGENTA 105
#define INVERSE_BOLD_CYAN 106
#define INVERSE_BOLD_WHITE 107

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

/* directions */
#define ILLEGAL_DIRECTION 0
#define NW 'y'
#define N 'k'
#define NE 'u'
#define W 'h'
#define E 'l'
#define SW 'b'
#define S 'j'
#define SE 'n'
#define UP '<'
#define DOWN '>'
#define NOWHERE '.'

/* commands */
#define ILLEGAL_ACTION 0
#define APPLY "a"
#define CLOSE_PAGE " "
#define DIP "#dip\n"
#define DROP_MENU "D"
#define DROP "d"
#define EAT "e"
#define ENGRAVE "E"
#define ENHANCE "#enhance\n"
#define FIGHT "F"
#define INVENTORY "i"
#define KICK ""
#define LOOK ":"
#define NAME "#name\n"
#define OPEN "o"
#define PICKUP ","
#define PRAY "#pray\n"
#define PUT_ON "P"
#define QUAFF "q"
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
#define UNKNOWN_TILE_DIAGONALLY_UNPASSABLE 137
#define UNKNOWN_TILE_UNPASSABLE 138
#define ROGUE_STAIRS 139
#define MINES_FOUNTAIN 140
#define SHOP_TILE 141

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
#define PET 255

/* priorities
 * "global" priorities structure:
 * PRIORITY_<what_to_do>
 * examples:
 * PRIORITY_CONTINUE_ACTION
 * PRIORITY_LOOK
 *
 * "analyzer" priorities structure:
 * PRIORITY_<analyzer>_<what_to_do>
 * examples:
 * PRIORITY_BEATITUDE_DROP_ALTAR
 * PRIORITY_WEAPON_WIELD */
/* illegal, max & min */
#define ILLEGAL_PRIORITY -1
#define PRIORITY_MAX 999
#define PRIORITY_MIN 0
/* 1000->  : zero-turn actions */
#define PRIORITY_CONTINUE_ACTION 1003
#define PRIORITY_SELECT_ITEM 1002
#define PRIORITY_CLOSE_PAGE 1001
#define PRIORITY_LOOK 1000
/* 800-999: panic, all hell is loose, pray, teleport, get the hell away */
#define PRIORITY_VAULT_GO_OUT 999
#define PRIORITY_FOOD_PRAY_FOR_FOOD 950
#define PRIORITY_HEALTH_CURE_POLYMORPH 925
#define PRIORITY_HEALTH_CURE_DEADLY 900
#define PRIORITY_HEALTH_PRAY_FOR_HP 900
/* 600-799: don't panic, rest for hp, fix bad stuff */
#define PRIORITY_FOOD_EAT_FAINTING 750
#define PRIORITY_FOOD_EAT_WEAK 725
#define PRIORITY_HEALTH_REST_FOR_HP_LOW 700
#define PRIORITY_HEALTH_QUAFF_FOR_HP 700
/* 400-599: don't move. engrave for protection, attack if not moving and protected, feed, etc */
#define PRIORITY_HEALTH_REST_FOR_HP_HIGH 500
#define PRIORITY_WEAPON_WIELD 475
#define PRIORITY_FIGHT_ATTACK 450
#define PRIORITY_HEALTH_CURE_NON_DEADLY 450
#define PRIORITY_LOOT_LOOT_STASH 450
#define PRIORITY_FOOD_EAT_CORPSE 435
#define PRIORITY_HEALTH_CURE_LYCANTHROPY 400
#define PRIORITY_POTION_QUAFF_GAIN_LEVEL 400
/* 200-399: move around, eg. attacking & looting */
#define PRIORITY_FOOD_EAT_HUNGRY 350
#define PRIORITY_SHOP_DROP_DIGGING_TOOL 330
#define PRIORITY_FIGHT_MOVE 325
#define PRIORITY_LAMP_TOGGLE 300
#define PRIORITY_SHOP_ENTER 275
#define PRIORITY_LOOT_VISIT_STASH 250
#define PRIORITY_LOOT_DROP_ITEMS 245
#define PRIORITY_BEATITUDE_DROP_ALTAR 230
#define PRIORITY_AMULET_WEAR 225
#define PRIORITY_ARMOR_WEAR 225
#define PRIORITY_EXCALIBUR_DIP 225
#define PRIORITY_RING_WEAR 225
/* 0-199: explore/random/do stuff we normally wouldn't do (melee floating eye) */
#define PRIORITY_DOOR_OPEN 150
#define PRIORITY_EXPLORE_FIND_ROGUE_STAIRS 70
#define PRIORITY_EXPLORE_STAIRS_UP 60
#define PRIORITY_EXPLORE_EXPLORE 50
#define PRIORITY_EXPLORE_STAIRS_DOWN 40
#define PRIORITY_EXPLORE_TRAVEL 30
#define PRIORITY_EXPLORE_SEARCH 20
#define PRIORITY_FIGHT_BLUE_E 15

/* discard item */
#define DISCARD "DISCARD"

/* messages */
#define MESSAGE_CANT_REACH_OVER_PIT "  You can't reach over the edge of the pit.  "
#define MESSAGE_CHOOSE_DIRECTION "  In what direction?  "
#define MESSAGE_CRAWL_OUT_OF_PIT "  You crawl to the edge of the pit.  "
#define MESSAGE_CURRENT_SKILLS "  Current skills:  "
#define MESSAGE_DESTROY_POTION_FIRE " boils and explodes!  "
#define MESSAGE_DESTROY_POTION_FIRE2 " boil and explode!  "
#define MESSAGE_DESTROY_POTION_COLD " freeze and shatter!  "
#define MESSAGE_DESTROY_POTION_COLD2 " freezes and shatters!  "
#define MESSAGE_DESTROY_RING " turns to dust and vanishes!  "
#define MESSAGE_DESTROY_RING2 " turn to dust and vanish!  "
#define MESSAGE_DESTROY_WAND " breaks apart and explodes!  "
#define MESSAGE_DESTROY_WAND2 " break apart and explode!  "
#define MESSAGE_DIP_IN_FOUNTAIN " into the fountain? [yn] (n)  " // begins with "  Dip the %s ..."
#define MESSAGE_DOOR_LOCKED "  This door is locked.  "
#define MESSAGE_DRINK_FROM_FOUNTAIN "  Drink from the fountain? [yn] (n)  "
#define MESSAGE_DROP_WHICH_ITEMS "  What would you like to drop?  "
#define MESSAGE_DYWYPI "  Do you want your possessions identified? [ynq] "
#define MESSAGE_ENGRAVE_ADD "  Do you want to add to the current engraving? "
#define MESSAGE_ENGRAVE_DUST "  What do you want to write in the dust here?  "
#define MESSAGE_ENGRAVE_DUST_ADD "  What do you want to add to the writing in the dust here?  "
#define MESSAGE_ENGRAVE_FROST "  What do you want to write in the frost here?  "
#define MESSAGE_ENGRAVE_FROST_ADD "  What do you want to add to the writing in the frost here?  "
#define MESSAGE_ENGRAVE_WITH "  What do you want to write with? "
#define MESSAGE_FEEL_MORE_CONFIDENT "  You feel more confident in your "
#define MESSAGE_FOOCUBUS_QUESTION "  \"Shall I remove your " // followed by what he/she wants to remove.
#define MESSAGE_FOOCUBUS_REMOVE "  \"Take off your " // followed by what the foocubus removed.
#define MESSAGE_FOR_INSTRUCTIONS "  (For instructions type a ?)  "
#define MESSAGE_FOR_WHAT_DO_YOU_WISH "  For what do you wish?  "
#define MESSAGE_GUARD_DISAPPEARS "  Suddenly, the guard disappears.  "
#define MESSAGE_HELLO_STRANGER "  \"Hello stranger, who are you?\" -  "
#define MESSAGE_LIMBS_ARE_STIFFENING "  Your limbs are stiffening.  "
#define MESSAGE_LIMBS_TURNED_TO_STONE "  Your limbs have turned to stone.  "
#define MESSAGE_NOT_CARRYING_ANYTHING "  Not carrying anything.  "
#define MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD "  Not carrying anything except gold.  "
#define MESSAGE_NOTHING_HAPPENS "  Nothing happens.  "
#define MESSAGE_NOTHING_TO_EAT "  You don't have anything to eat.  "
#define MESSAGE_PICK_A_SKILL "  Pick a skill to advance:  "
#define MESSAGE_PICK_UP_WHAT "  Pick up what?  "
#define MESSAGE_RECEIVED_EXCALIBUR "  From the murky depths, a hand reaches up to bless the sword.  " // wohoo
#define MESSAGE_SLOWING_DOWN "  You are slowing down.  "
#define MESSAGE_TELEPORT_WHERE "  To what position do you want to be teleported?  "
#define MESSAGE_TEXT_BURNED "  Some text has been burned into the floor here.  "
#define MESSAGE_TEXT_DIGGED "  Something is engraved here on the floor.  "
#define MESSAGE_TEXT_DUSTED "  Something is written here in the dust.  "
#define MESSAGE_TEXT_FROSTED "  Something is written here in the frost.  "
#define MESSAGE_UNLOCK_IT "  Unlock it? [yn] (n)  "
#define MESSAGE_WHAT_TO_APPLY "  What do you want to use or apply? "
#define MESSAGE_WHAT_TO_DIP "  What do you want to dip? "
#define MESSAGE_WHAT_TO_DRINK "  What do you want to drink? "
#define MESSAGE_WHAT_TO_DROP "  What do you want to drop? "
#define MESSAGE_WHAT_TO_EAT "  What do you want to eat? "
#define MESSAGE_WHAT_TO_REMOVE "  What do you want to remove? "
#define MESSAGE_WHAT_TO_PUT_ON "  What do you want to put on? "
#define MESSAGE_WHAT_TO_TAKE_OFF "  What do you want to take off? "
#define MESSAGE_WHAT_TO_THROW "  What do you want to throw? "
#define MESSAGE_WHAT_TO_WEAR "  What do you want to wear? "
#define MESSAGE_WHAT_TO_WIELD "  What do you want to wield? "
#define MESSAGE_WHICH_RING_FINGER "  Which ring-finger, Right or Left? [rl]  "
#define MESSAGE_YOU_FINISH_TAKING_OFF "  You finish taking off your " // followed by helmet/gloves/boots/suit
#define MESSAGE_YOU_READ "  You read:"
#define MESSAGE_YOU_WERE_WEARING "  You were wearing " // followed by what we took off

#endif
