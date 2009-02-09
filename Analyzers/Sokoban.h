#ifndef SOKOBAN_H
#define SOKOBAN_H

#include <vector>
#include <map>
#include "../Analyzer.h"
#include "../Point.h"
#include "../Level.h"

#define SOKOBAN_DEBUG_NAME "Sokoban] "

#define SOKOBAN_LEVELS 8
#define MAX_SOKOBAN_BOULDERS 21

using namespace std;

class Saiph;

struct Move {
	int boulder;
	unsigned char direction;

	Move(int boulder, unsigned char direction) : boulder(boulder), direction(direction) {}
};

class Sokoban : public Analyzer {
	public:
		Sokoban(Saiph *saiph);
		bool isSokobanLevel(void);
		bool isSokobanLevel(int level);
		int whichSokobanLevel();

		void analyze(void);
		void parseMessages(const std::string &messages);

	private:
		void targetNextLocation(int level, const Move &move);
		void moveBoulderToTarget(int level, const Move &move);

		Saiph *saiph;

		vector<vector<Point> > boulders;
		vector<vector<Move> > moves;

		map<int, int> levelMap;

		bool solved[SOKOBAN_LEVELS];
		bool started[SOKOBAN_LEVELS];

		vector<Move>::iterator currentMove;
		Point currentTarget;
		bool moving;
};
#endif	
