#ifndef DIG_H
#define	DIG_H

#include <string>
#include "../Analyzer.h"

class Item;
class Saiph;

class Dig : public Analyzer {
	public:
		Dig(Saiph *saiph);

		void parseMessages(const std::string &message);
		void analyze();

	private:
		Saiph *saiph;
		unsigned char dig_direction;
		unsigned char digging_tool;

		int directionIs(int direction);
		bool directionIsWall(int direction);
		bool directionIsFloor(int direction);
		int boulderInDirection();
		bool isDiggingTool(const Item &i);
		bool isDiggingTool(unsigned char letter);
		unsigned char findDiggingTool();
		bool freeWeaponHand();
};
#endif	
