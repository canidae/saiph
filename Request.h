#ifndef REQUEST_H
#define REQUEST_H
/* requests */
#define ILLEGAL_REQUEST 0
#define REQUEST_APPLY_UNIHORN 1 // action
#define REQUEST_ELBERETH_OR_REST 2 // action
#define REQUEST_UPDATED_INVENTORY 3 // event
#define REQUEST_DIRTY_INVENTORY 4 // event
#define REQUEST_DIRTY_STASH 5 // event
#define REQUEST_EAT 6 // action
#define REQUEST_PRAY 7 // action
#define REQUEST_ARMOR_WEAR 8 // action
#define REQUEST_ITEM_GROUP_SET_AMOUNT 9 // hmm, possibly remove
#define REQUEST_ITEM_GROUP_ADD 10 // hmm, possibly remove
#define REQUEST_ITEM_PICKUP 11 // hmm, possibly remove
#define REQUEST_WEAPON_WIELD 12 // action
#define REQUEST_ADD_THROWN_WEAPON 13 // hmm?
#define REQUEST_RING_WEAR 14 // action
#define REQUEST_AMULET_WEAR 15 // action
#define REQUEST_QUAFF_HEALING 16 // action
#define REQUEST_CALL_ITEM 17 // action
#define REQUEST_NAME_ITEM 18 // action
#define REQUEST_BEATIFY_ITEMS 19 // event
#define REQUEST_BECOME_BLIND 20 // hmm
#define REQUEST_UNBLIND 21 // hmm

#include <string>
#include "Coordinate.h"

class Request {
	public:
		int request;
		int priority;
		int value;
		int beatitude;
		bool sustain;
		bool only_unknown_enchantment;
		unsigned char key;
		std::string data;
		Coordinate coordinate;

		Request();
};
#endif
