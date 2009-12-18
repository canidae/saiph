#ifndef ANALYZER_SOKOBAN_H
#define ANALYZER_SOKOBAN_H

#include <vector>
#include <map>
#include "Analyzer.h"
#include "../Point.h"
#include "../Coordinate.h"
#include "../Level.h"

#define SOKOBAN_DEBUG_NAME "Sokoban] "
#define SOKOBAN_MAX_PUSH_FAILURES 15

class Saiph;

struct Move {
	int boulder;
	unsigned char direction;

	Move(int boulder, unsigned char direction) : boulder(boulder), direction(direction) {}
};

struct SokobanLevel {
	std::vector<Point> boulders;
	std::vector<Move> solution;
};

namespace analyzer {
	class Sokoban : public Analyzer {
	public:
		Sokoban(Saiph* saiph);
		bool isSokobanLevel(void);
		bool isSokobanLevel(unsigned int level);
		int whichSokobanLevel();

		void analyze(void);
		void parseMessages(const std::string& messages);

	private:
		void targetNextLocation(int level, const Move& move);
		void moveBoulderToTarget(int level, const Move& move);

		void loadLevels();
		bool loadBoulders(std::ifstream& file, std::vector<Point>& boulders);
		bool loadSolutions(std::ifstream& file, std::vector<Move>& moves);
		void getnextline(std::ifstream& file, std::string& line);

		Saiph* saiph;

		int lineNumber;
		std::vector<SokobanLevel> levels;

		std::map<int, int> levelMap;

		std::vector<bool> solved;
		std::vector<bool> started;

		std::vector<Move>::iterator currentMove;
		Coordinate currentTarget;
		bool moving;
		int pushFailures;
	};
}
#endif
