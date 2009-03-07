#include "LevelData.h"
#include "../Point.h"

map<std::string, LevelData *> LevelData::levels;

LevelData::LevelData(const std::string &name, const std::vector< std::vector<char> > &symbols, int branch, int min_depth, int max_depth) : name(name), symbols(symbols), branch(branch), min_depth(min_depth), max_depth(max_depth) {
}

int LevelData::match(Level l) {
	if (l.branch != branch || l.depth < min_depth || l.depth > max_depth)
		return MATCH_IMPOSSIBLE;
	int matches = 0, skipped = 0;
	//assume symbols is rectangular and has at least one row vector
	for (int row = 0; row < symbols.size(); row++)
		for (int col = 0; col < symbols[0].size(); col++) {
			char bullet = l.getDungeonSymbol(Point(row+MAP_ROW_BEGIN, col+MAP_COL_BEGIN));
			char target = symbols[row][col];
			if (bullet == UNKNOWN_TILE || bullet == UNKNOWN_TILE_UNPASSABLE || bullet == UNKNOWN_TILE_DIAGONALLY_UNPASSABLE) {
				skipped++;
				continue;
			}
			//unexpected wall; walls can't be created, so it's not a match
			if ((bullet == VERTICAL_WALL && target != VERTICAL_WALL) ||
					(bullet == HORIZONTAL_WALL && target != HORIZONTAL_WALL))
				return MATCH_IMPOSSIBLE;
			//stairs mismatch; any difference means no match
			if ((bullet == STAIRS_DOWN ^ target == STAIRS_DOWN) ||
					(bullet == STAIRS_UP ^ target == STAIRS_UP))
				return MATCH_IMPOSSIBLE;
			//unexpected lava; lava cannot be created, so it's not a match
			if (bullet == LAVA && target != LAVA)
				return MATCH_IMPOSSIBLE;
			if (bullet == target)
				matches++;
		}
	int checked = (symbols.size() * symbols[0].size() - skipped);
	if (checked < 10) //need moar samples
		return MATCH_UNCERTAIN;
	return (matches*100) / checked;
}

void LevelData::init() {
}
