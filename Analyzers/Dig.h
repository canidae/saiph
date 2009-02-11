#ifndef DIG_H
#define	DIG_H

#include <list>
#include <string>
#include "../Analyzer.h"
#include "../Point.h"
#include "../Request.h"

class Item;
class Saiph;

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
		std::list<Point> dig_locations;

		void findDiggingTool();
		bool isDiggingTool(const Item &item);

		int directionIs(int direction);
		bool directionIsWall(int direction);
		bool directionIsFloor(int direction);
		int boulderInDirection();
		bool isDiggingTool(unsigned char letter);
		bool freeWeaponHand();
};
#endif	
