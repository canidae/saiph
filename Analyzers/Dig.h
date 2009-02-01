#ifndef DIG_H
#define	DIG_H

#include "../Analyzer.h"
#include "../Saiph.h"
#include <string>

class Dig : public Analyzer {
	public:
		Dig(Saiph* saiph);
		void parseMessages(const std::string& message);
		void analyze();

	private:
		Saiph* saiph;
		unsigned char dig_direction;
		int directionIs(int direction);
		bool directionIsWall(int direction);
		bool directionIsFloor(int direction);
		int boulderInDirection();
		unsigned char findDiggingTool();
};
#endif	
