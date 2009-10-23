#include <vector>
#include <sstream>

#include "Sokoban.h"
#include "../Globals.h"
#include "../Saiph.h"
#include "../Point.h"
#include "../Debug.h"

using namespace analyzer;
using namespace std;

Sokoban::Sokoban(Saiph* saiph) : Analyzer("Sokoban"), saiph(saiph), moving(false), pushFailures(0) {
	loadLevels();

	for (unsigned int a = 0; a < levels.size(); ++a) {
		started.push_back(false);
		solved.push_back(false);
	}
}

void Sokoban::parseMessages(const string& messages) {
	if (moving && currentTarget.level == saiph->position.level) {
		if (messages.find(MESSAGE_PERHAPS_THATS_WHY, 0) != string::npos) {
			if (++pushFailures > SOKOBAN_MAX_PUSH_FAILURES) {
				/* TODO make Saiph come back after she gets more projectiles to throw */
				moving = false;
				return;
			}
			/* There is a monster behind the boulder, so try to wait for them to move and push again. */
			command = REST;
			priority = PRIORITY_SOLVE_SOKOBAN;
			return;
		}
		int level = levelMap[saiph->position.level];
		Point& boulder = levels[level].boulders[(*currentMove).boulder];
		if ((Point)saiph->position == boulder) {
			/* we made it to the square where the boulder was, so the bouler must be pushed */
			moveBoulderToTarget(level, *currentMove);
			++currentMove;
			pushFailures = 0;
			Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Moved boulder, going to move number " << currentMove - levels[level].solution.begin() << endl;
			if (currentMove == levels[level].solution.end()) {
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
	if (moving && currentTarget.level != saiph->position.level &&
		pushFailures < SOKOBAN_MAX_PUSH_FAILURES) {
		/* We must have fallen into one of the holes */
		const PathNode& node = saiph->shortestPath(currentTarget);
		if (node.cost >= UNPASSABLE) {
			Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Couldn't return to current target level. Giving up for now." << endl;
			return;
		}
		Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Attempting to move back to target level " << currentTarget.level << endl;
		priority = PRIORITY_SOLVE_SOKOBAN;
		command = node.dir;
		return;
	}
	if (saiph->levels[saiph->position.level].branch == BRANCH_SOKOBAN) {
		int level = levelMap[saiph->position.level];
		if (level == 0) {
			/* we haven't been here before; figure out which level we're on */
			/* Note that if we ever get some more random sokoban levels, we'll just have to use whichSokobanLevel */
			level = (saiph->position.level - saiph->branch_sokoban.level) * 2;
			if (isSokobanLevel(level + 1))
				++level;
			levelMap[saiph->position.level] = level;
			Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "currently in Sokoban level " << level << endl;
		}
		if (!solved[level]) {
			if (!started[level]) {
				/* first time we've run on this level, so initialize our state */
				currentMove = levels[level].solution.begin();
				currentTarget.level = saiph->position.level;
				targetNextLocation(level, *currentMove);
				started[level] = true;
				moving = true;
			}

			if (saiph->position == currentTarget) {
				Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Got to target " << (Point)currentTarget << ", pushing boulder '" << currentMove->direction << "'" << endl;
				command = currentMove->direction;
				priority = PRIORITY_SOLVE_SOKOBAN;
			} else {
				Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Trying to path to " << (Point)currentTarget << " from " << (Point)saiph->position << endl;

				const PathNode& node = saiph->shortestPath(currentTarget);
				if (node.cost >= UNPASSABLE) {
					Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "pathing failed" << endl;
					return;
				}
				Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "Will arrive in " << node.moves << " moves" << endl;
				priority = PRIORITY_SOLVE_SOKOBAN;
				command = node.dir;
			}
		}
	}
}

void Sokoban::targetNextLocation(int level, const Move& move) {
	Point boulder = levels[level].boulders[move.boulder];

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

void Sokoban::moveBoulderToTarget(int level, const Move& move) {
	Point& boulder = levels[level].boulders[move.boulder];
	Debug::info(saiph->last_turn) << SOKOBAN_DEBUG_NAME << "moved boulder from " << boulder;
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
	Debug::debugfile << " to " << boulder << endl;
}

bool Sokoban::isSokobanLevel() {
	return whichSokobanLevel() != -1;
}

int Sokoban::whichSokobanLevel() {
	for (unsigned int a = 0; a < levels.size(); ++a)
		if (isSokobanLevel(a))
			return a;

	return -1;
}

bool Sokoban::isSokobanLevel(unsigned int level) {
	vector<Point> b = levels[level].boulders;
	for (vector<Point>::iterator i = b.begin(); i != b.end(); ++i)
		if (saiph->getDungeonSymbol(*i) != BOULDER)
			return false;

	return true;
}

void Sokoban::loadLevels() {
	bool error = false;
	ifstream file("sokoban.dat");
	lineNumber = 0;

	if (!file) {
		Debug::error() << SOKOBAN_DEBUG_NAME << "Couldn't open sokoban.dat" << endl;
		return;
	}

	while (file) {
		SokobanLevel level;

		if (loadBoulders(file, level.boulders) &&
			loadSolutions(file, level.solution)) {
			levels.push_back(level);
		} else {
			if (!file.eof())
				error = true;
			break;
		}
	}

	file.close();

	if (error)
		Debug::error() << SOKOBAN_DEBUG_NAME << "Error in sokoban.dat at line " << lineNumber << ", stopping processing." << endl;
	else
		Debug::info() << SOKOBAN_DEBUG_NAME << "Loaded " << levels.size() << " levels from sokoban.dat" << endl;
}

bool Sokoban::loadBoulders(ifstream& file, vector<Point>& boulders) {
	string line;
	getnextline(file, line);

	while (file && line != ".") {
		istringstream is(line);
		int x, y;
		if (is >> x && is >> y)
			boulders.push_back(Point(x, y));
		else {
			Debug::error() << SOKOBAN_DEBUG_NAME << "Couldn't parse boulder line \"" << line << "\"" << endl;
			return false;
		}
		getnextline(file, line);
	}

	if (boulders.size() == 0)
		return false;
	else
		return true;
}

bool Sokoban::loadSolutions(ifstream& file, vector<Move>& moves) {
	string line;
	getnextline(file, line);

	while (file && line != ".") {
		char boulderChar;
		int boulder;

		if (line.length() < 3 || line[1] != ' ') {
			Debug::error() << SOKOBAN_DEBUG_NAME << "Line too short \"" << line << "\"" << endl;
			return false;
		}

		boulderChar = line[0];
		// Convert upper to lowercase
		if (boulderChar > 0x60 && boulderChar <= 0x7A)
			boulderChar -= 0x20;
		if (boulderChar <= 0x40 || boulderChar > 0x5A)
			return false;

		boulder = boulderChar - 'A';

		for (unsigned int x = 2; x < line.length(); ++x) {
			switch (line[x]) {
			case ' ':
				continue;
			case 'u':
				moves.push_back(Move(boulder, N));
				break;
			case 'd':
				moves.push_back(Move(boulder, S));
				break;
			case 'l':
				moves.push_back(Move(boulder, W));
				break;
			case 'r':
				moves.push_back(Move(boulder, E));
				break;
			default:
				return false;
			}
		}

		getnextline(file, line);
	}

	if (moves.size() == 0)
		return false;
	else
		return true;
}

void Sokoban::getnextline(ifstream& file, string& line) {
	// get the next line ignoring blank lines and the comments
	do {
		getline(file, line);
		++lineNumber;
		string::size_type loc = line.find('#');
		if (loc != string::npos)
			line = line.substr(0, loc);
		loc = line.find_first_not_of(' ');
		if (loc != 0 && loc != string::npos)
			line = line.substr(loc);
		loc = line.find_last_not_of(" \t\f\v\n\r");
		if (loc != line.length() - 1)
			line = line.substr(0, loc + 1);
	} while (file && line.empty());
}
