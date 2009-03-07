#ifndef _LEVELDATA_H
#define	_LEVELDATA_H

#include <string>
#include <map>
#include <vector>

#define MATCH_CERTAIN 100
#define MATCH_IMPOSSIBLE 0
#define MATCH_UNCERTAIN -1

class LevelData {
public:
	static std::map<std::string, LevelData *> levels;
	static void init();

	const std::string name;
	const std::vector< std::vector<char> > symbols;
	const int branch;
	const int min_depth;
	const int max_depth;

	LevelData(const std::string &name, const std::vector< std::vector<char> > &symbols, int branch, int min_depth, int max_depth) {}
	int match(Level level);
};

#endif	/* _LEVELDATA_H */

