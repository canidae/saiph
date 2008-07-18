#include "Food.h"

/* constructors */
Food::Food(Saiph *saiph) : Analyzer("Food"), saiph(saiph) {
	action = "";
	priority = ILLEGAL_PRIORITY;
	eat_order.push_back("tin");
	eat_order.push_back("meatball");
	eat_order.push_back("meat ring");
	eat_order.push_back("meat stick");
	eat_order.push_back("huge chunk of meat");
	eat_order.push_back("cream pie");
	eat_order.push_back("lichen corpse");
	eat_order.push_back("tripe ration");
	eat_order.push_back("melon");
	eat_order.push_back("apple");
	eat_order.push_back("pear");
	eat_order.push_back("kelp frond");
	eat_order.push_back("C-ration");
	eat_order.push_back("cram ration");
	eat_order.push_back("food ration");
	eat_order.push_back("gunyoki");
	eat_order.push_back("K-ration");
	eat_order.push_back("clove of garlic");
	eat_order.push_back("fortune cookie");
	eat_order.push_back("banana");
	eat_order.push_back("orange");
	eat_order.push_back("candy bar");
	eat_order.push_back("byte");
	eat_order.push_back("egg");
	eat_order.push_back("lump of royal jelly");
	eat_order.push_back("pancake");
	eat_order.push_back("lembas wafer");
	eat_order.push_back("carrot"); // will cure blindness
	eat_order.push_back("eucalyptus leaf"); // will cure sickness
	eat_order.push_back("sprig of wolfsbane"); // will cure lycanthropy
	eat_order.push_back("lizard corpse"); // wil cure stoning
}

/* methods */
void Food::announce(const Announce &announce) {
	if (announce.announce == ANNOUNCE_ITEM_ON_GROUND) {
		for (list<string>::iterator f = eat_order.begin(); f != eat_order.end(); ++f) {
			if (announce.data == *f) {
				/* yummy, food!
				 * request that someone pick it up for us */
				req.request = REQUEST_PICK_UP_ITEM;
				req.priority = FOOD_PICK_UP_PRIORITY;
				req.data = announce.data;
				req.value1 = announce.value1; // pick up all there is
				req.coordinate.branch = announce.coordinate.branch;
				req.coordinate.level = announce.coordinate.level;
				req.coordinate.row = announce.coordinate.row;
				req.coordinate.col = announce.coordinate.col;
				saiph->request(req); // no need to check for outcome of this
			}
		}
	}
}

void Food::command(string *command) {
	*command = action;
}

void Food::finish() {
	/* are we weak or fainting? */
	if (saiph->world->player.hunger < HUNGRY) {
		/* yes, we are */
	}
	/* "easter egg". if satiated or worse, eat bytes for funny death */
	if (saiph->world->player.hunger > CONTENT) {
	}
}

void Food::parseMessages(string *messages) {
}

bool Food::request(const Request &request) {
	return false;
}
