#ifndef SOKOBAN_H
#define SOKOBAN_H

#include <vector>
#include <map>
#include "../Analyzer.h"
#include "../Point.h"
#include "../Coordinate.h"
#include "../Level.h"

#define SOKOBAN_DEBUG_NAME "Sokoban] "

using namespace std;

class Saiph;

struct Move {
	int boulder;
	unsigned char direction;

	Move(int boulder, unsigned char direction) : boulder(boulder), direction(direction) {}
};

struct SokobanLevel {
	vector<Point> boulders;
	vector<Move> solution;
};

class Sokoban : public Analyzer {
	public:
		Sokoban(Saiph *saiph);
		bool isSokobanLevel(void);
		bool isSokobanLevel(unsigned int level);
		int whichSokobanLevel();

		void analyze(void);
		void parseMessages(const std::string &messages);

	private:
		void targetNextLocation(int level, const Move &move);
		void moveBoulderToTarget(int level, const Move &move);

		void loadLevels();
		bool loadBoulders(ifstream &file, vector<Point> &boulders);
		bool loadSolutions(ifstream &file, vector<Move> &moves);
		void getnextline(ifstream &file, string &line);

		Saiph *saiph;

		int lineNumber;
		vector<SokobanLevel> levels;

		map<int, int> levelMap;

		vector<bool> solved;
		vector<bool> started;

		vector<Move>::iterator currentMove;
		Coordinate currentTarget;
		bool moving;
};
#endif
