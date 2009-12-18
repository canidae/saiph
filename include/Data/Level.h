#ifndef DATA_LEVEL_H
#define DATA_LEVEL_H

#include <string>
#include <map>
#include <vector>

#define MATCH_CERTAIN 100
#define MATCH_IMPOSSIBLE 0
#define MATCH_UNCERTAIN -1

namespace data {

	class Level {
	public:
		static std::map<std::string, Level*> levels;
		static void init();

		const std::string name;
		const std::vector< std::vector<char> > symbols;
		const int branch;
		const int min_depth;
		const int max_depth;

		Level(const std::string& name, const std::vector< std::vector<char> >& symbols, int branch, int min_depth, int max_depth) {
		}
		int match(Level level);
	};
}
#endif
