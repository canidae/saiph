#ifndef SHOP_H
#define SHOP_H
/* shop keeper says "digging tool", "pick-axe", and "mattock"

   Formats:
	"Will you please leave your XXX outside?"
        "Leave the XXX outside."
*/

//				 "Will you please leave your pick-axe outside?"
#define SHOP_MESSAGE_PICKAXE "  \"Will you please leave your pick-axe outside?\"  "
#define SHOP_MESSAGE_PICKAXE_PLURAL "  \"Will you please leave your pick-axes outside?\"  "
#define SHOP_MESSAGE_MATTOCK "  \"Will you please leave your mattock outside?\"  "
#define SHOP_MESSAGE_MATTOCK_PLURAL "  \"Will you please leave your mattocks outside?\"  "
#define SHOP_MESSAGE_TOOLS "  \"Will you please leave your digging tools outside?\"  "

#define SHOP_MESSAGE_ANGRY_PICKAXE "  \"Leave the pick-axe outside.\"  "
#define SHOP_MESSAGE_ANGRY_PICKAXE_PLURAL "  \"Leave the pick-axes outside.\"  "
#define SHOP_MESSAGE_ANGRY_MATTOCK "  \"Leave the mattock outside.\"  "
#define SHOP_MESSAGE_ANGRY_MATTOCK_PLURAL "  \"Leave the mattocks outside.\"  "
#define SHOP_MESSAGE_ANGRY_TOOLS "  \"Leave the digging tools outside.\"  "

#define SHOP_IDLE 0
#define SHOP_MOVE_AWAY 1
#define SHOP_DROP_TOOLS 2
#define SHOP_LOOK_AT_GROUND 3
#define SHOP_MOVE_INTO_SHOP 4
#define SHOP_BROWSING 5

#include <string>
#include "../Analyzer.h"
#include "../Point.h"

class Saiph;

class Shop : public Analyzer {
	public:
		Shop(Saiph *saiph);

		void parseMessages(const std::string &messages);
		void analyze();

	private:
		Saiph *saiph;
		Point exit;
		unsigned char direction;
		int state;
};
#endif
