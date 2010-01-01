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
#define BRANCHES 8 // remember to update this when branches are added
#define KNAPSACK_LIMIT 52
#define UNREACHABLE UINT_MAX
#define UNPASSABLE UNREACHABLE - 1

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
#define BEATITUDE_UNKNOWN 0 << 0
#define CURSED            1 << 0
#define UNCURSED          1 << 1
#define BLESSED           1 << 2

/* alignment */
#define CHAOTIC   1 << 0
#define NEUTRAL   1 << 1
#define LAWFUL    1 << 2
#define UNALIGNED 1 << 3

/* attitude */
#define ATTITUDE_UNKNOWN 0 << 0
#define HOSTILE          1 << 0
#define FRIENDLY         1 << 1

/* encumbrance */
#define UNENCUMBERED 1 << 0
#define BURDENED     1 << 1
#define STRESSED     1 << 2
#define STRAINED     1 << 3
#define OVERTAXED    1 << 4
#define OVERLOADED   1 << 5

/* hunger */
#define FAINTING     1 << 0
#define WEAK         1 << 1
#define HUNGRY       1 << 3
#define CONTENT      1 << 4
#define SATIATED     1 << 5
#define OVERSATIATED 1 << 6

/* slots */
#define ILLEGAL_SLOT -1
#define SLOTS 15
#define SLOT_SHIRT 0
#define SLOT_SUIT 1
#define SLOT_CLOAK 2
#define SLOT_BOOTS 3
#define SLOT_GLOVES 4
#define SLOT_HELMET 5
#define SLOT_SHIELD 6
#define SLOT_AMULET 7
#define SLOT_LEFT_RING 8
#define SLOT_RIGHT_RING 9
#define SLOT_WEAPON 10
#define SLOT_OFFHAND_WEAPON 11
#define SLOT_EYES 12
#define SLOT_ALTERNATE_WEAPON 13
#define SLOT_QUIVER 14

/* weapon types */
#define WEAPON_AXE            1 << 0
#define WEAPON_BOOMERANG      1 << 1
#define WEAPON_BOW            1 << 2
#define WEAPON_BROADSWORD     1 << 3
#define WEAPON_CLUB           1 << 4
#define WEAPON_CROSSBOW       1 << 5
#define WEAPON_DAGGER         1 << 6
#define WEAPON_DART           1 << 7
#define WEAPON_FLAIL          1 << 8
#define WEAPON_HAMMER         1 << 9
#define WEAPON_JAVELIN        1 << 10
#define WEAPON_KNIFE          1 << 11
#define WEAPON_LANCE          1 << 12
#define WEAPON_LONGSWORD      1 << 13
#define WEAPON_MACE           1 << 14
#define WEAPON_MORNINGSTAR    1 << 15
#define WEAPON_PICKAXE        1 << 16
#define WEAPON_POLEARM        1 << 17
#define WEAPON_QUARTERSTAFF   1 << 18
#define WEAPON_SABER          1 << 19
#define WEAPON_SCIMITAR       1 << 20
#define WEAPON_SHORTSWORD     1 << 21
#define WEAPON_SHURIKEN       1 << 22
#define WEAPON_SLING          1 << 23
#define WEAPON_SPEAR          1 << 24
#define WEAPON_TRIDENT        1 << 25
#define WEAPON_TWOHANDEDSWORD 1 << 26
#define WEAPON_UNICORNHORN    1 << 27
#define WEAPON_WHIP           1 << 28
#define WEAPON_BOW_AMMO       1 << 29
#define WEAPON_CROSSBOW_AMMO  1 << 30
#define WEAPON_SLING_AMMO     1 << 31

/* genders */
#define UNKNOWN_GENDER 0 << 0
#define FEMALE         1 << 0
#define MALE           1 << 1
#define NEUTER         1 << 2

/* roles */
#define UNKNOWN_ROLE 0 << 0
#define ARCHEOLOGIST 1 << 0
#define BARBARIAN    1 << 1
#define CAVEMAN      1 << 2
#define HEALER       1 << 3
#define KNIGHT       1 << 4
#define MONK         1 << 5
#define PRIEST       1 << 6
#define RANGER       1 << 7
#define ROGUE        1 << 8
#define SAMURAI      1 << 9
#define TOURIST      1 << 10
#define VALKYRIE     1 << 11
#define WIZARD       1 << 12

/* directions */
#define ILLEGAL_DIRECTION '\0'
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
#define MINES_FOUNTAIN 140 // TODO: get rid of
#define SHOP_TILE 141 // TODO: get rid of
#define MAGIC_PORTAL 142
#define OUTSIDE_MAP 254

/* trap values */
#define TRAP_PIT 1
#define TRAP_SPIKED_PIT 2

/* special monster symbols */
#define ILLEGAL_MONSTER 0
#define PET 255

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

/* properties */
#define PROPERTY_FIRE              ((unsigned long long) (1LL << 0)) // match MR_FIRE
#define PROPERTY_COLD              ((unsigned long long) (1LL << 1)) // match MR_COLD
#define PROPERTY_SLEEP             ((unsigned long long) (1LL << 2)) // match MR_SLEEP
#define PROPERTY_DISINT            ((unsigned long long) (1LL << 3)) // match MR_DISINT
#define PROPERTY_SHOCK             ((unsigned long long) (1LL << 4)) // match MR_ELEC
#define PROPERTY_POISON            ((unsigned long long) (1LL << 5)) // match MR_POISON
#define PROPERTY_ACID              ((unsigned long long) (1LL << 6)) // match MR_ACID
#define PROPERTY_STONE             ((unsigned long long) (1LL << 7)) // match MR_STONE
#define PROPERTY_SEE_INVISIBLE     ((unsigned long long) (1LL << 8))
#define PROPERTY_LEVITATION        ((unsigned long long) (1LL << 9))
#define PROPERTY_WATERWALKING      ((unsigned long long) (1LL << 10))
#define PROPERTY_MAGICAL_BREATHING ((unsigned long long) (1LL << 11))
#define PROPERTY_DISPLACEMENT      ((unsigned long long) (1LL << 12))
#define PROPERTY_STRENGTH          ((unsigned long long) (1LL << 13))
#define PROPERTY_FUMBLING          ((unsigned long long) (1LL << 14))
#define PROPERTY_MAGICRES          ((unsigned long long) (1LL << 15))
#define PROPERTY_REFLECTION        ((unsigned long long) (1LL << 16))
#define PROPERTY_INVISIBLE         ((unsigned long long) (1LL << 17))
#define PROPERTY_VISIBLE           ((unsigned long long) (1LL << 18))
#define PROPERTY_BRILLIANCE        ((unsigned long long) (1LL << 19))
#define PROPERTY_ESP               ((unsigned long long) (1LL << 20))
#define PROPERTY_STUPIDITY         ((unsigned long long) (1LL << 21))
#define PROPERTY_DEXTERITY         ((unsigned long long) (1LL << 22))
#define PROPERTY_JUMPING           ((unsigned long long) (1LL << 23))
#define PROPERTY_KICKING           ((unsigned long long) (1LL << 24))
#define PROPERTY_STEALTH           ((unsigned long long) (1LL << 25))
#define PROPERTY_VERYFAST          ((unsigned long long) (1LL << 26))
#define PROPERTY_SLIPPERY          ((unsigned long long) (1LL << 27))
#define PROPERTY_MAGIC             ((unsigned long long) (1LL << 28))
#define PROPERTY_RANDOM_APPEARANCE ((unsigned long long) (1LL << 29))
#define PROPERTY_CASTING_BONUS     ((unsigned long long) (1LL << 30))
#define PROPERTY_TELEPORT          ((unsigned long long) (1LL << 31))
#define PROPERTY_TELEPORT_CONTROL  ((unsigned long long) (1LL << 32))
#define PROPERTY_LIFE_SAVING       ((unsigned long long) (1LL << 33))
#define PROPERTY_STRANGULATION     ((unsigned long long) (1LL << 34))
#define PROPERTY_UNCHANGING        ((unsigned long long) (1LL << 35))
#define PROPERTY_RESTFUL_SLEEP     ((unsigned long long) (1LL << 36))
#define PROPERTY_SEX_CHANGE        ((unsigned long long) (1LL << 37))
#define PROPERTY_LYCANTHROPY       ((unsigned long long) (1LL << 38))
#define PROPERTY_SEARCHING         ((unsigned long long) (1LL << 39))
#define PROPERTY_ARTIFACT          ((unsigned long long) (1LL << 40))

/* discard item */
#define DISCARD "DISCARD"
/* elbereth */
#define ELBERETH "Elbereth"

/* messages */
/* list duplicate lines: grep "MESSAGE_" Globals.h | cut -d" " -f3- | sort | uniq -d */
#define MESSAGE_BREAK_SHOP_DOOR "  \"Cad!  You did 400 zorkmids worth of damage!\"  Pay?  [yn] (n)  "
#define MESSAGE_BURNED_TEXT "  Some text has been burned into the floor here.  "
#define MESSAGE_CANNOT_REACH_BOTTOM_OF_PIT "  You cannot reach the bottom of the pit.  "
#define MESSAGE_CANT_REACH_OVER_PIT "  You can't reach over the edge of the pit.  "
#define MESSAGE_CLOSED_FOR_INVENTORY "\"Closed for inventory.\""
#define MESSAGE_COLD_RES_GAIN1 "  You feel full of hot air.  "
#define MESSAGE_COLD_RES_LOSE1 "  You feel cooler!  "
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
#define MESSAGE_DIG_DIRECTION "  In what direction do you want to dig? "
#define MESSAGE_DIGGED_TEXT "  Something is engraved here on the floor.  "
#define MESSAGE_DIP_IN_FOUNTAIN " into the fountain? [yn] (n)  " // begins with "  Dip the %s ..."
#define MESSAGE_DIP_IN_WATER " into the pool of water? [yn] (n)  " // begins with "  Dip the %s ..."
#define MESSAGE_DISINTEGRATION_RES_GAIN1 "  You feel very firm.  "
#define MESSAGE_DISINTEGRATION_RES_GAIN2 "  You feel totally together, man.  "
#define MESSAGE_DONT_BE_RIDICULOUS "  Don't be ridiculous!  "
#define MESSAGE_DONT_EVEN_BOTHER "  Don't even bother.  "
#define MESSAGE_DOOR_LOCKED "  This door is locked.  "
#define MESSAGE_DRAWBRIDGE_LOWERED "  You see a drawbridge coming down!  "
#define MESSAGE_DRINK_FROM_FOUNTAIN "  Drink from the fountain? [yn] (n)  "
#define MESSAGE_DROP_WHICH_ITEMS "  What would you like to drop?  "
#define MESSAGE_DUSTED_TEXT "  Something is written here in the dust.  "
#define MESSAGE_DYWYPI "  Do you want your possessions identified? [ynq] "
#define MESSAGE_EAT_IT_1 "  There is "       // these four
#define MESSAGE_EAT_IT_2 " here; eat it? "   // are used
#define MESSAGE_EAT_ONE_1 "  There are "     // for eating
#define MESSAGE_EAT_ONE_2 " here; eat one? " // stuff on ground
#define MESSAGE_ENGRAVE_ADD "  Do you want to add to the current engraving? "
#define MESSAGE_ENGRAVE_DUST "  What do you want to write in the dust here? "
#define MESSAGE_ENGRAVE_DUST_ADD "  What do you want to add to the writing in the dust here? "
#define MESSAGE_ENGRAVE_FROST "  What do you want to write in the frost here? "
#define MESSAGE_ENGRAVE_FROST_ADD "  What do you want to add to the writing in the frost here? "
#define MESSAGE_ENGRAVE_WITH "  What do you want to write with? "
#define MESSAGE_ENTER_GEHENNOM "  Are you sure you want to enter? [yn] (n)  "
#define MESSAGE_FALL_INTO_PIT "  You fall into a pit!  "
#define MESSAGE_FEEL_MORE_CONFIDENT "  You feel more confident in your "
#define MESSAGE_FIRE_RES_GAIN1 "  You feel a momentary chill.  "
#define MESSAGE_FIRE_RES_GAIN2 "  You be chillin'.  "
#define MESSAGE_FIRE_RES_LOSE1 "  You feel warmer!  "
#define MESSAGE_FOOCUBUS_QUESTION "  \"Shall I remove your " // followed by what he/she wants to remove.
#define MESSAGE_FOOCUBUS_REMOVE "  \"Take off your " // followed by what the foocubus removed.
#define MESSAGE_FOR_INSTRUCTIONS "  (For instructions type a ?)  "
#define MESSAGE_FOR_WHAT_DO_YOU_WISH "  For what do you wish?  "
#define MESSAGE_FROSTED_TEXT "  Something is written here in the frost.  "
#define MESSAGE_GOT_BURDENED "  Your movements are slowed slightly because of your load.  "
#define MESSAGE_GOT_OVERTAXED "  You can barely move a handspan with this load!  "
#define MESSAGE_GOT_STRAINED "  You stagger under your heavy load.  Movement is very hard.  "
#define MESSAGE_GOT_STRESSED "  You rebalance your load.  Movement is difficult.  "
#define MESSAGE_GUARD_DISAPPEARS "  Suddenly, the guard disappears.  "
#define MESSAGE_HELLO_STRANGER "  \"Hello stranger, who are you?\" -  "
#define MESSAGE_HOW_MUCH_OFFER "  How much will you offer?  "
#define MESSAGE_HURT_LEFT_LEG "  Your left leg is in no shape for kicking.  "
#define MESSAGE_HURT_RIGHT_LEG "  Your right leg is in no shape for kicking.  "
#define MESSAGE_IMPROVISE "  Improvise? [yn] (n)  "
#define MESSAGE_IN_WHAT_DIRECTION "  In what direction?  "
#define MESSAGE_IS_DESTROYED " is destroyed!  "
#define MESSAGE_IS_KILLED " is killed!  "
#define MESSAGE_ITEM_TO_CALL "  What do you want to call? "
#define MESSAGE_ITEM_TO_NAME "  What do you want to name? "
#define MESSAGE_LEG_IS_BETTER "  Your leg feels somewhat better.  "
#define MESSAGE_LEVITATION_GAIN1 "  You start to float in the air!  "
#define MESSAGE_LEVITATION_GAIN2 "  You float up, out of the pit!  "
#define MESSAGE_LEVITATION_LOSE1 "  You float gently to the " // followed by kind of floor
#define MESSAGE_LEVITATION_LOSE2 "  You crash to the " // followed by kind of floor
#define MESSAGE_LIMBS_ARE_STIFFENING "  Your limbs are stiffening.  "
#define MESSAGE_LIMBS_TURNED_TO_STONE "  Your limbs have turned to stone.  "
#define MESSAGE_LYCANTHROPY_GAIN1 "  You feel feverish.  "
#define MESSAGE_LYCANTHROPY_LOSE1 "  You feel purified.  "
#define MESSAGE_MANY_MORE_OBJECTS_HERE "  There are many more objects here.  "
#define MESSAGE_MANY_OBJECTS_HERE "  There are many objects here.  "
#define MESSAGE_NAME_INDIVIDUAL_OBECT "  Name an individual object? [ynq] (q)  "
#define MESSAGE_NOT_CARRYING_ANYTHING "  Not carrying anything.  "
#define MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD "  Not carrying anything except gold.  "
#define MESSAGE_NOTHING_HAPPENS "  Nothing happens.  "
#define MESSAGE_NOTHING_TO_EAT "  You don't have anything to eat.  "
#define MESSAGE_PERHAPS_THATS_WHY "  Perhaps that's why you cannot move it.  "
#define MESSAGE_PICK_A_SKILL "  Pick a skill to advance:  "
#define MESSAGE_PICK_UP_WHAT "  Pick up what?  "
#define MESSAGE_PLAY_PASSTUNE "  Play the passtune? [yn] (n)  "
#define MESSAGE_POISON_RES_GAIN1 "  You feel healthy.  "
#define MESSAGE_POISON_RES_GAIN2 "  You feel especially healthy.  "
#define MESSAGE_POISON_RES_LOSE1 "  You feel a little sick!  "
#define MESSAGE_POLYMORPH "  You turn into "
#define MESSAGE_REALLY_ATTACK "Really attack"
#define MESSAGE_RECEIVED_EXCALIBUR "  From the murky depths, a hand reaches up to bless the sword.  " // wohoo
#define MESSAGE_SEVERAL_MORE_OBJECTS_HERE "  There are several more objects here.  "
#define MESSAGE_SEVERAL_OBJECTS_HERE "  There are several objects here.  "
#define MESSAGE_SHOCK_RES_GAIN1 "  Your health currently feels amplified!  "
#define MESSAGE_SHOCK_RES_GAIN2 "  You feel grounded in reality.  "
#define MESSAGE_SHOCK_RES_LOSE1 "  You feel conductive!  "
#define MESSAGE_SLEEP_RES_GAIN1 "  You feel wide awake.  "
#define MESSAGE_SLEEP_RES_LOSE1 "  You feel tired!  "
#define MESSAGE_SLOWING_DOWN "  You are slowing down.  "
#define MESSAGE_STEALS " steals "
#define MESSAGE_STILL_IN_PIT "  You are still in a pit.  "
#define MESSAGE_STOLE " stole "
#define MESSAGE_STOP_EATING "  Stop eating? [yn] (y)  "
#define MESSAGE_SUCCEED_UNLOCKING "  You succeed in unlocking the door.  "
#define MESSAGE_SUCCEED_PICKING "  You succeed in picking the lock.  "
#define MESSAGE_TALK_TO_WHOM "  Talk to whom? "
#define MESSAGE_TELEPATHY_GAIN1 "  You feel a strange mental acuity.  "
#define MESSAGE_TELEPATHY_LOSE1 "  Your senses fail!  "
#define MESSAGE_TELEPORT_CONTROL_GAIN1 "  You feel in control of yourself.  "
#define MESSAGE_TELEPORT_CONTROL_GAIN2 "  You feel centered in your personal space.  "
#define MESSAGE_TELEPORT_WHERE "  To what position do you want to be teleported?  "
#define MESSAGE_TELEPORTITIS_GAIN1 "  You feel very jumpy.  "
#define MESSAGE_TELEPORTITIS_GAIN2 "  You feel diffuse.  "
#define MESSAGE_TELEPORTITIS_LOSE1 "  You feel less jumpy.  "
#define MESSAGE_UNLOCK_IT "  Unlock it? [yn] (n)  "
#define MESSAGE_UNPOLYMORPH "  You return to "
#define MESSAGE_WHAT_TO_APPLY "  What do you want to use or apply? "
#define MESSAGE_WHAT_TO_CALL_ITEM "  Call "
#define MESSAGE_WHAT_TO_DIP "  What do you want to dip? "
#define MESSAGE_WHAT_TO_DIP_INTO "  What do you want to dip into? "
#define MESSAGE_WHAT_TO_DRINK "  What do you want to drink? "
#define MESSAGE_WHAT_TO_DROP "  What do you want to drop? "
#define MESSAGE_WHAT_TO_EAT "  What do you want to eat? "
#define MESSAGE_WHAT_TO_GENOCIDE_BLESSED  "  What class of monsters do you wish to genocide?  "
#define MESSAGE_WHAT_TO_GENOCIDE_UNCURSED "  What monster do you want to genocide? [type the name]  "
#define MESSAGE_WHAT_TO_NAME_ITEM "  What do you want to name this "
#define MESSAGE_WHAT_TO_PUT_ON "  What do you want to put on? "
#define MESSAGE_WHAT_TO_REMOVE "  What do you want to remove? "
#define MESSAGE_WHAT_TO_RUB "  What do you want to rub? "
#define MESSAGE_WHAT_TO_TAKE_OFF "  What do you want to take off? "
#define MESSAGE_WHAT_TO_THROW "  What do you want to throw? "
#define MESSAGE_WHAT_TO_WEAR "  What do you want to wear? "
#define MESSAGE_WHAT_TO_WIELD "  What do you want to wield? "
#define MESSAGE_WHAT_TO_ZAP "  What do you want to zap? "
#define MESSAGE_WHAT_TUNE "  What tune are you playing? "
#define MESSAGE_WHICH_RING_FINGER "  Which ring-finger, Right or Left? [rl]  "
#define MESSAGE_YOU_DESTROY "  You destroy the "
#define MESSAGE_YOU_DIG_A_PIT "  You dig a pit in the floor.  "
#define MESSAGE_YOU_CANT_HOLD "  You can't even hold anything!  "
#define MESSAGE_YOU_FINISH_TAKING_OFF "  You finish taking off your " // followed by helmet/gloves/boots/suit
#define MESSAGE_YOU_FINISH_YOUR_PRAYER "  You finish your prayer.  "
#define MESSAGE_YOU_FLOAT_OUT_OF_PIT "  You float up, out of the pit!  "
#define MESSAGE_YOU_KILL "  You kill the "
#define MESSAGE_YOU_READ "  You read:"
#define MESSAGE_YOU_WERE_WEARING "  You were wearing " // followed by what we took off

#endif
