#ifndef DIG_H
#define	DIG_H

#include <list>
#include <string>
#include "Analyzer.h"
#include "../Point.h"
#include "../Request.h"

/* Dig messages */
#define DIG_NOT_ENOUGH_ROOM "  There isn't enough room to "

class Item;
class Saiph;

namespace analyzer {
	class Dig : public Analyzer {
	public:
		Dig(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &message);

	private:
		Request req;
		Saiph *saiph;
		unsigned char digging_tool;
		unsigned char dig_direction;
		Point last_dig_target;
		Coordinate last_dig_location;
		std::list<Point> dig_locations;

		void findDiggingTool();
		bool freeWeaponHand();
		bool isDiggingTool(const Item &item);
	};
}
#endif
