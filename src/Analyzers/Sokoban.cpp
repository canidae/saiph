#include "Analyzers/Sokoban.h"

#include "Debug.h"
#include "Globals.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Move.h"

using namespace analyzer;
using namespace std;

Sokoban::Sokoban() : Analyzer("Sokoban") {
	/* 8 sokoban levels */
	_moves.resize(8);

	/* Level 1A */
	addMoves(0, Point(8, 37), "j");
	addMoves(0, Point(8, 38), "llll");
	addMoves(0, Point(11, 41), "jj");
	addMoves(0, Point(14, 40), "k");
	addMoves(0, Point(14, 39), "h");
	addMoves(0, Point(14, 42), "k");
	addMoves(0, Point(14, 41), "hhhh");
	addMoves(0, Point(13, 42), "jhhhhhh");
	addMoves(0, Point(13, 41), "jhhhhhh");
	addMoves(0, Point(13, 40), "jhhhhhhhk");
	addMoves(0, Point(9, 42), "l");
	addMoves(0, Point(8, 42), "jjhjjjjhhhhhhhhkk");
	addMoves(0, Point(9, 44), "jhhhjjjjhhhhhhhhkkk");
	addMoves(0, Point(9, 43), "hhjjjjjhhhhhhhhkkkk");
	addMoves(0, Point(8, 43), "jjhhjjjjhhhhhhhhkkkkk");
	addMoves(0, Point(9, 39), "klll");
	addMoves(0, Point(9, 38), "k");
	addMoves(0, Point(8, 42), "jjhjjjjhhhhhhhhkkkkkk");

	/* Level 1B */
	addMoves(1, Point(8, 34), "l");
	addMoves(1, Point(9, 34), "k");
	addMoves(1, Point(9, 41), "lhhhhhhh");
	addMoves(1, Point(10, 42), "jjj");
	addMoves(1, Point(15, 41), "h");
	addMoves(1, Point(16, 40), "h");
	addMoves(1, Point(16, 42), "hhhh");
	addMoves(1, Point(13, 42), "jjjhhhhh");
	addMoves(1, Point(14, 41), "jjhhhhh");
	addMoves(1, Point(15, 40), "jhhhhhhk");
	addMoves(1, Point(13, 40), "jjjhhhhhhkk");
	addMoves(1, Point(8, 42), "jjjjjjjjhhhhhhhhkkkkl");
	addMoves(1, Point(9, 35), "llllllljjjjjjjhhhhhhhhkkkkll");
	addMoves(1, Point(8, 34), "jlllllllljjjjjjjhhhhhhhhkkkklll");

	/* Level 2A */
	addMoves(2, Point(17, 29), "lllk");
	addMoves(2, Point(16, 36), "ll");
	addMoves(2, Point(16, 33), "jhhhhh");
	addMoves(2, Point(9, 34), "jj");
	addMoves(2, Point(10, 35), "h");
	addMoves(2, Point(15, 36), "h");
	addMoves(2, Point(11, 35), "k");
	addMoves(2, Point(11, 34), "hh");
	addMoves(2, Point(14, 33), "j");
	addMoves(2, Point(15, 35), "ljlll");
	addMoves(2, Point(16, 33), "lllllllkkljllll");
	addMoves(2, Point(15, 33), "jjhhhhhlllllllkkljlllll");
	addMoves(2, Point(12, 31), "jjhhhlllllllkkljllllll");
	addMoves(2, Point(16, 26), "hjhhhlllllllkkljlllllll");
	addMoves(2, Point(13, 32), "jljjjhhhhhlllllllkkljllllllll");
	addMoves(2, Point(10, 34), "j");
	addMoves(2, Point(10, 35), "h");
	addMoves(2, Point(9, 28), "k");
	addMoves(2, Point(12, 28), "l");
	addMoves(2, Point(14, 29), "k");
	addMoves(2, Point(11, 30), "l");
	addMoves(2, Point(11, 32), "jjj");
	addMoves(2, Point(13, 29), "j");
	addMoves(2, Point(8, 28), "j");
	addMoves(2, Point(14, 32), "ljjjhhhhhlllllllkkljlllllllll");
	addMoves(2, Point(14, 29), "lllljjjhhhhhlllllllkkljllllllllll");
	addMoves(2, Point(11, 31), "ljjjljjjhhhhhlllllllkkljlllllllllll");
	addMoves(2, Point(13, 42), "lljjjjjhhhhhlllllllkkljllllllllllll");
	addMoves(2, Point(11, 28), "k");
	addMoves(2, Point(12, 29), "lllljjjjjhhhhhlllllllkkljlllllllllllll");

	/* Level 2B */
	addMoves(3, Point(9, 30), "hhhlllllk");
	addMoves(3, Point(13, 26), "kkkkk");
	addMoves(3, Point(15, 26), "kkkkkkl");
	addMoves(3, Point(14, 27), "lllll");
	addMoves(3, Point(15, 33), "l");
	addMoves(3, Point(16, 34), "ll");
	addMoves(3, Point(16, 32), "lllll");
	addMoves(3, Point(14, 33), "jjlllll");
	addMoves(3, Point(15, 34), "jlllll");
	addMoves(3, Point(13, 34), "jjjllllll");
	addMoves(3, Point(14, 32), "lljjlllllll");
	addMoves(3, Point(13, 32), "jjjllllllllll");
	addMoves(3, Point(15, 27), "kllllllljjlllllllll");
	addMoves(3, Point(15, 28), "klllllljjllllllllll");
	addMoves(3, Point(13, 27), "hjlllllllljjlllllllllll");
	addMoves(3, Point(8, 26), "jjjjjjlllllllljjllllllllllll");
	addMoves(3, Point(9, 27), "hjjjjjlllllllljjlllllllllllll");
	addMoves(3, Point(8, 27), "jjhjjjjlllllllljjllllllllllllll");
	addMoves(3, Point(15, 30), "khhhllllllljjlllllllllllllll");
	addMoves(3, Point(8, 31), "k");
	addMoves(3, Point(8, 30), "k");
	addMoves(3, Point(8, 28), "k");
	addMoves(3, Point(12, 27), "l");
	addMoves(3, Point(13, 30), "jhhhllllllljjllllllllllllllll");

	/* Level 3A */
	addMoves(4, Point(16, 35), "hh");
	addMoves(4, Point(17, 35), "ll");
	addMoves(4, Point(15, 36), "jjll");
	addMoves(4, Point(13, 34), "jjj");
	addMoves(4, Point(16, 33), "k");
	addMoves(4, Point(16, 34), "lljlll");
	addMoves(4, Point(15, 33), "ljjllllll");
	addMoves(4, Point(13, 33), "ljjjjlllllll");
	addMoves(4, Point(10, 33), "jjjljjjjllllllll");
	addMoves(4, Point(10, 32), "ljjjljjjjlllllllll");
	addMoves(4, Point(13, 37), "llhhhhhjjjjllllllllll");
	addMoves(4, Point(12, 39), "kk");
	addMoves(4, Point(12, 36), "jjjjjlllllllll");
	addMoves(4, Point(11, 35), "ljjjjjjllllllllll");
	addMoves(4, Point(8, 34), "ll");
	addMoves(4, Point(9, 35), "jjljjjjjj");
	addMoves(4, Point(9, 34), "h");
	addMoves(4, Point(10, 39), "l");
	addMoves(4, Point(9, 38), "kk");
	addMoves(4, Point(9, 37), "j");
	addMoves(4, Point(17, 36), "lllllllllll");

	/* Level 3B */
	addMoves(5, Point(15, 33), "hllllll");
	addMoves(5, Point(13, 32), "l");
	addMoves(5, Point(14, 32), "jlllllll");
	addMoves(5, Point(13, 33), "jjlllllll");
	addMoves(5, Point(8, 32), "jjjjjjjlllllllll");
	addMoves(5, Point(8, 33), "hjjjjjjjllllllllll");
	addMoves(5, Point(9, 35), "k");
	addMoves(5, Point(8, 38), "jj");
	addMoves(5, Point(9, 37), "hhhk");
	addMoves(5, Point(8, 35), "j");
	addMoves(5, Point(8, 34), "hhjjjjjjjlllllllllll");
	addMoves(5, Point(8, 34), "hkhhjjjjjjjllllllllllll");
	addMoves(5, Point(8, 37), "jhhhkhhjjjjjjjlllllllllllll");
	addMoves(5, Point(8, 40), "k");
	addMoves(5, Point(9, 40), "k");
	addMoves(5, Point(9, 41), "l");
	addMoves(5, Point(12, 36), "jjjllllllllll");
	addMoves(5, Point(13, 35), "jjhhhlllllllllllllll");

	/* Level 4A */
	addMoves(6, Point(14, 30), "h");
	addMoves(6, Point(15, 33), "lll");
	addMoves(6, Point(18, 29), "lll");
	addMoves(6, Point(16, 31), "lljjjhhhhhkll");
	addMoves(6, Point(11, 30), "hh");
	addMoves(6, Point(12, 30), "ljjjjlljjjhhhhhk");
	addMoves(6, Point(11, 32), "hh");
	addMoves(6, Point(11, 35), "hh");
	addMoves(6, Point(9, 35), "hjjjhhhjjjjl");
	addMoves(6, Point(9, 33), "ljjjhhhjjj");
	addMoves(6, Point(11, 33), "lkkkhkkkl");
	addMoves(6, Point(11, 30), "llllkkkhkkkll");
	addMoves(6, Point(11, 28), "llllllkkkhkkklll");
	addMoves(6, Point(11, 37), "hhhkkkhkkkllll");
	addMoves(6, Point(15, 31), "kkkklllkkkhkkklllll");
	addMoves(6, Point(14, 29), "llkkklllkkkhkkkllllll");
	addMoves(6, Point(14, 32), "hkkklllkkkhkkklllllll");
	addMoves(6, Point(14, 34), "hhhkkklllkkkhkkkllllllll");
	addMoves(6, Point(15, 36), "hhhhhkkkklllkkkhkkklllllllll");
	addMoves(6, Point(18, 32), "lkkkkhhkkklllkkkhkkkllllllllll");
	addMoves(6, Point(18, 30), "lllkkkkhhkkklllkkkhkkklllllllllll");
	addMoves(6, Point(18, 28), "lllllkkkkhhkkklllkkkhkkkllllllllllll");
	addMoves(6, Point(16, 32), "ljjjhhhhhklllllkkkkhhkkklllkkkhkkklllllllllllll");
	addMoves(6, Point(9, 31), "llljjjhhhjjjkkkklllkkkhkkkllllllllllllll");
	addMoves(6, Point(9, 29), "llllljjjhhhjjjkkkklllkkkhkkklllllllllllllll");
	addMoves(6, Point(9, 37), "hhhjjjhhhjjjkkkklllkkkhkkk");
	addMoves(6, Point(16, 29), "h");
	addMoves(6, Point(16, 35), "l");
	addMoves(6, Point(5, 33), "llllllllllllllll");

	/* Level 4B */
	addMoves(7, Point(18, 33), "lllkkkk");
	addMoves(7, Point(14, 37), "k");
	addMoves(7, Point(13, 38), "kk");
	addMoves(7, Point(11, 37), "k");
	addMoves(7, Point(11, 36), "hhh");
	addMoves(7, Point(12, 31), "hh");
	addMoves(7, Point(13, 30), "j");
	addMoves(7, Point(11, 30), "k");
	addMoves(7, Point(11, 31), "k");
	addMoves(7, Point(10, 28), "kj");
	addMoves(7, Point(8, 30), "kkkl");
	addMoves(7, Point(10, 30), "jjkkkkkkkll");
	addMoves(7, Point(10, 28), "k");
	addMoves(7, Point(10, 29), "lkkkkklll");
	addMoves(7, Point(9, 28), "llkkkkllll");
	addMoves(7, Point(14, 30), "kkkkkkkkklllll");
	addMoves(7, Point(14, 29), "lkkkkkkkkkllllll");
	addMoves(7, Point(13, 29), "lkkkkkkkklllllll");
	addMoves(7, Point(12, 29), "kkklkkkkllllllll");
	addMoves(7, Point(12, 28), "kkkllkkkklllllllll");
	addMoves(7, Point(11, 33), "jhhhkkkkkkkllllllllll");
	addMoves(7, Point(13, 37), "j");
	addMoves(7, Point(12, 36), "khhhjhhhkkkkkkklllllllllll");
	addMoves(7, Point(14, 36), "kkkhhhjhhhkkkkkkkllllllllllll");
	addMoves(7, Point(13, 38), "hkkkhhhjhhhkkkkkkklllllllllllll");
	addMoves(7, Point(9, 37), "l");
	addMoves(7, Point(11, 37), "jjjhkkhhhjhhhkkkkkkkllllllllllllll");
	addMoves(7, Point(10, 38), "hhjhhhjhhhkkkkkkklllllllllllllll");
	addMoves(7, Point(9, 38), "jhhjhhhjhhhkkkkkkkllllllllllllllll");
	addMoves(7, Point(10, 31), "jjhkkkkkkklllllllllllllllll");
	addMoves(7, Point(14, 31), "kkhkkkkkkk");
	addMoves(7, Point(16, 32), "h");
	addMoves(7, Point(11, 38), "k");
	addMoves(7, Point(5, 30), "llllllllllllllllll");
}

void Sokoban::addMoves(int level, Point pos, const string& moves) {
	/* translate moves to positions */
	unsigned char prev = ILLEGAL_DIRECTION;
	for (int m = 0; m < (int) moves.size(); ++m) {
		if (prev != moves[m]) {
			/* shifting direction, need to place her correctly */
			/* place "pos" at boulder by repeating last move unless it's the first move */
			if (prev != ILLEGAL_DIRECTION)
				pos.moveDirection(moves[m - 1]);
			/* then move point to opposite direction of the direction we're going to push */
			switch (moves[m]) {
			case N:
				pos.moveSouth();
				break;

			case E:
				pos.moveWest();
				break;

			case S:
				pos.moveNorth();
				break;

			case W:
				pos.moveEast();
				break;

			default:
				Debug::error() << "Error parsing sokoban movement '" << moves[m] << "'" << endl;
				break;
			}
			/* then add position to move to before continue pushing */
			_moves[level].push_back(pos);
			prev = (unsigned char) moves[m];
		}
		_moves[level].push_back(pos.moveDirection(moves[m]));
	}
}

void Sokoban::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_PERHAPS_THATS_WHY) != string::npos) {
		/* monster blocking the way, should be marked as "I", no handling for the time being */
	}
}

void Sokoban::analyze() {
	if (World::currentPriority() >= SOKOBAN_SOLVE_PRIORITY)
		return;
	if (World::level().branch() != BRANCH_SOKOBAN)
		return;
	map<int, int>::iterator l = _levelmap.find(Saiph::position().level());
	int level = -1;
	if (l == _levelmap.end()) {
		for (int s = 0; s < (int) _moves.size(); ++s) {
			/* 2nd. entry should be a unique boulder for each level */
			deque<Point>::iterator f = _moves[s].begin();
			if (f == _moves[s].end() || ++f == _moves[s].end())
				continue; // seems like this level is completed
			if (World::level().tile(*f).symbol() != BOULDER)
				continue; // expected boulder, can't be this level
			/* this must be it */
			_levelmap[Saiph::position().level()] = s;
			level = s;
			break;
		}
	} else {
		/* already discovered this sokoban level */
		level = l->second;
	}
	if (level == -1) {
		/* huh? */
		Debug::warning() << "Sokoban was unable to recognize this level" << endl;
		return;
	}
	/* go to next point */
	Point p = _moves[level].front();
	if (p == Saiph::position()) {
		/* we're standing at the next point, erase it and go to next */
		_moves[level].pop_front();
		if (_moves[level].size() > 0) {
			p = _moves[level].front();
		} else {
			/* look at that, we've solved this sokoban level */
			Debug::custom(name()) << "Solved sokoban level " << level << endl;
			return;
		}
	}
	Tile& tile = World::shortestPath(p);
	if (tile.cost() == UNREACHABLE) {
		/* this is bad */
		Debug::custom(name()) << "Unable to move to " << tile << endl;
		return;
	} else if (tile.cost() == UNPASSABLE) {
		if (tile.symbol() == BOULDER) {
			/* pushing boulder */
			Debug::custom(name()) << "Pushing a boulder: " << tile << endl;
		} else {
			/* uh, not good at all */
			Debug::custom(name()) << "Wanted to move on to an unpassable non-boulder square: " << tile << endl;
			return;
		}
	} else {
		/* probably moving to correct point before pushing boulder */
		Debug::custom(name()) << "Moving to the right spot to push a boulder: " << tile << endl;
	}
	World::setAction(static_cast<action::Action*> (new action::Move(this, tile, SOKOBAN_SOLVE_PRIORITY)));
}
