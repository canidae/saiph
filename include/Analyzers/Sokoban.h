#ifndef ANALYZER_SOKOBAN_H
#define ANALYZER_SOKOBAN_H

#include <map>
#include <deque>
#include <vector>
#include "Point.h"
#include "Analyzers/Analyzer.h"

#define SOKOBAN_SOLVE_PRIORITY 100

namespace analyzer {

	class Sokoban : public Analyzer {
	public:
		Sokoban();

		void analyze(void);
		void parseMessages(const std::string& messages);

	private:
		std::vector<std::deque<Point> > _moves;
		std::map<int, int> _levelmap;

		void addMoves(int level, Point pos, const std::string& moves);
	};
}
#endif
