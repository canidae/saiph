#include <vector>

#include "Sokoban.h"
#include "../Globals.h"
#include "../Saiph.h"
#include "../Point.h"
#include "../Debug.h"

using namespace std;

#define CURRENT_LEVEL(x) saiph->levels[saiph->position.level].dungeonmap[(x).row][(x).col]

Sokoban::Sokoban(Saiph *saiph) : Analyzer("Sokoban"), saiph(saiph), moving(false) {
	// setup boulder locations
	#include "Sokoban/boulders.h"

	// setup solutions
	#include "Sokoban/solutions.h"

	for (int a = 0; a < SOKOBAN_LEVELS; ++a) {
		started[a] = false;
		solved[a] = false;
	}
}

void Sokoban::parseMessages(const string &messages) {
	if (moving && saiph->levels[saiph->position.level].branch == BRANCH_SOKOBAN) {
		int level = levelMap[saiph->position.level];
		Point &boulder = boulders[level][(*currentMove).boulder];
		if ((Point)saiph->position == boulder) {
			/* we made it to the square where the boulder was, so the bouler must be pushed */
			moveBoulderToTarget(level, *currentMove);
			++currentMove;
			Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Moved boulder, going to move number " << currentMove - moves[level].begin() << endl;
			if (currentMove == moves[level].end()) {
				/* we've completed all our moves for this level */
				Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Solved sokoban level " << level << endl;
				solved[level] = true;
				moving = false;
			} else {
				targetNextLocation(level, *currentMove);
			}
		}
	}
}

void Sokoban::analyze() {
	if (priority > PRIORITY_SOLVE_SOKOBAN)
		return;
	if (saiph->levels[saiph->position.level].branch == BRANCH_SOKOBAN) {
		int level = levelMap[saiph->position.level];
		if (level == 0) {
			/* we haven't been here before; figure out which level we're on */
			level = (saiph->position.level - saiph->branch_sokoban.level) * 2;
			if (isSokobanLevel(level + 1))
				++level;
			levelMap[saiph->position.level] = level;
			Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "currently in Sokoban level " << level << endl;
		}
		if (!solved[level]) {
			if (!started[level]) {
				/* first time we've run on this level, so initialize our state */
				currentMove = moves[level].begin();
				targetNextLocation(level, *currentMove);
				started[level] = true;
				moving = true;
			}

			if ((Point)saiph->position == currentTarget) {
				Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Got to target " << currentTarget << ", pushing boulder '" << currentMove->direction << "'" << endl;
				command = currentMove->direction;
				priority = PRIORITY_SOLVE_SOKOBAN;
			} else {
				Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Trying to path to " << currentTarget << " from " << (Point)saiph->position << endl;

				int moves = 0;
				unsigned char next_move = saiph->shortestPath(currentTarget, false, &moves);
				if (next_move == ILLEGAL_DIRECTION) {
					Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "pathing failed" << endl;
					return;
				}
				Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Will arrive in " << moves << " moves" << endl;
				priority = PRIORITY_SOLVE_SOKOBAN;
				command = next_move;
			}
		}
	}
}

void Sokoban::targetNextLocation(int level, const Move &move) {
	Point boulder = boulders[level][move.boulder];

	switch (move.direction) {
	case N:
		currentTarget.row = boulder.row + 1;
		currentTarget.col = boulder.col;
		break;
	case S:
		currentTarget.row = boulder.row - 1;
		currentTarget.col = boulder.col;
		break;
	case W:
		currentTarget.row = boulder.row;
		currentTarget.col = boulder.col + 1;
		break;
	case E:
		currentTarget.row = boulder.row;
		currentTarget.col = boulder.col - 1;
		break;
	default:
		break;
	}
}

void Sokoban::moveBoulderToTarget(int level, const Move &move) {
	Point &boulder = boulders[level][move.boulder];
	Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "moved boulder from " << boulders[level][move.boulder];
	switch (move.direction) {
	case N:
		boulder.row--;
		break;
	case S:
		boulder.row++;
		break;
	case W:
		boulder.col--;
		break;
	case E:
		boulder.col++;
		break;
	default:
		break;
	}
	Debug::debugfile << " to " << boulders[level][move.boulder] << endl;
}

bool Sokoban::isSokobanLevel() {
	return whichSokobanLevel() != -1;
}

int Sokoban::whichSokobanLevel() {
	for (int a = 0; a < SOKOBAN_LEVELS; ++a)
		if (isSokobanLevel(a))
			return a;

	return -1;
}

bool Sokoban::isSokobanLevel(int level) {
	vector<Point> b = boulders[level];
	for (vector<Point>::iterator i = b.begin(); i != b.end(); ++i)
		if (CURRENT_LEVEL(*i) != BOULDER)
				return false;

	return true;
}
