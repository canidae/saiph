#include "Analyzers/Sokoban.h"

#include "Debug.h"
#include "Globals.h"
#include "Saiph.h"
#include "World.h"
#include "Coordinate.h"
#include "Actions/Move.h"
#include "Actions/Search.h"
#include "Actions/Kick.h"
#include "Analyzers/Boulder.h"

#define RETRY_COUNT 10
#define TURNS_BETWEEN_RETRIES 10

using namespace analyzer;
using namespace std;

Sokoban::Sokoban() : Analyzer("Sokoban"), _retry_count(0), _retry_turn(-1), _given_up(false) {
	/* 8 sokoban levels */
	_moves.resize(8);

	/* Level 1A */
	Point a1a(8, 37);
	Point b1a(8, 38);
	Point c1a(8, 43);
	Point d1a(9, 38);
	Point e1a(9, 39);
	Point f1a(9, 42);
	Point g1a(9, 44);
	Point h1a(11, 41);
	Point i1a(14, 39);
	Point j1a(14, 40);
	Point k1a(14, 41);
	Point l1a(14, 42);
	addMoves(0, a1a, "j");
	addMoves(0, b1a, "llll");
	addMoves(0, h1a, "jj");
	addMoves(0, j1a, "k");
	addMoves(0, i1a, "h");
	addMoves(0, l1a, "k");
	addMoves(0, k1a, "hhhh");
	addMoves(0, l1a, "jhhhhhh");
	addMoves(0, h1a, "jhhhhhh");
	addMoves(0, j1a, "jhhhhhhhk");
	addMoves(0, f1a, "l");
	addMoves(0, b1a, "jjhjjjjhhhhhhhhkk");
	addMoves(0, g1a, "jhhhjjjjhhhhhhhhkkk");
	addMoves(0, f1a, "hhjjjjjhhhhhhhhkkkk");
	addMoves(0, c1a, "jjhhjjjjhhhhhhhhkkkkk");
	addMoves(0, d1a, "kj"); // looking for items beneath boulder
	addMoves(0, e1a, "kllljjhjjjjhhhhhhhhkkkkkk");

	/* Level 1B */
	Point a1b(8, 34);
	Point b1b(8, 42);
	Point c1b(9, 34);
	Point d1b(9, 41);
	Point e1b(10, 42);
	Point f1b(13, 40);
	Point g1b(14, 41);
	Point h1b(15, 41);
	Point i1b(16, 40);
	Point j1b(16, 42);
	addMoves(1, a1b, "l");
	addMoves(1, c1b, "k");
	addMoves(1, d1b, "lhhhhhhh");
	addMoves(1, e1b, "jjj");
	addMoves(1, h1b, "h");
	addMoves(1, i1b, "h");
	addMoves(1, j1b, "hhhh");
	addMoves(1, e1b, "jjjhhhhh");
	addMoves(1, g1b, "jjhhhhh");
	addMoves(1, h1b, "jhhhhhhk");
	addMoves(1, f1b, "jjjhhhhhhkk");
	addMoves(1, b1b, "jjjjjjjjhhhhhhhhkkkkl");
	addMoves(1, d1b, "llllllljjjjjjjhhhhhhhhkkkkll");
	addMoves(1, c1b, "jlllllllljjjjjjjhhhhhhhhkkkklll");

	/* Level 2A */
	Point a2a(9, 28);
	Point b2a(9, 34);
	Point c2a(10, 35);
	Point d2a(11, 28);
	Point e2a(11, 30);
	Point f2a(11, 35);
	Point g2a(12, 28);
	Point h2a(12, 31);
	Point i2a(13, 32);
	Point j2a(14, 29);
	Point k2a(14, 33);
	Point l2a(15, 31);
	Point m2a(15, 36);
	Point n2a(16, 33);
	Point o2a(16, 36);
	Point p2a(17, 29);
	addMoves(2, p2a, "lllk");
	addMoves(2, o2a, "ll");
	addMoves(2, n2a, "jhhhhh");
	addMoves(2, b2a, "jj");
	addMoves(2, c2a, "h");
	addMoves(2, m2a, "h");
	addMoves(2, f2a, "k");
	addMoves(2, b2a, "hh");
	addMoves(2, k2a, "j");
	addMoves(2, m2a, "ljlll");
	addMoves(2, n2a, "lllllllkkljllll");
	addMoves(2, k2a, "jjhhhhhlllllllkkljlllll");
	addMoves(2, l2a, "jjhhhlllllllkkljllllll");
	addMoves(2, p2a, "ljllkkljlllllll");
	addMoves(2, i2a, "jljjjllkkljllllllll");
	addMoves(2, c2a, "j");
	addMoves(2, f2a, "h");
	addMoves(2, a2a, "k");
	addMoves(2, g2a, "l");
	addMoves(2, d2a, "k");
	addMoves(2, e2a, "l");
	addMoves(2, b2a, "jjj");
	addMoves(2, d2a, "j");
	addMoves(2, a2a, "j");
	addMoves(2, b2a, "ljjjllkkljlllllllll");
	addMoves(2, j2a, "lllljjjllkkljllllllllll");
	addMoves(2, e2a, "ljjjljjjllkkljlllllllllll");
	addMoves(2, h2a, "lljjjjjllkkljllllllllllll");
	addMoves(2, g2a, "lllljjjjjllkkljlllllllllllll");

	/* Level 2B */
	Point a2b(8, 27);
	Point b2b(8, 28);
	Point c2b(8, 30);
	Point d2b(8, 31);
	Point e2b(9, 30);
	Point f2b(12, 27);
	Point g2b(13, 26);
	Point h2b(13, 27);
	Point i2b(13, 30);
	Point j2b(13, 32);
	Point k2b(13, 34);
	Point l2b(14, 27);
	Point m2b(14, 33);
	Point n2b(15, 26);
	Point o2b(15, 27);
	Point p2b(15, 28);
	Point q2b(15, 30);
	Point r2b(15, 33);
	Point s2b(16, 32);
	Point t2b(16, 34);
	addMoves(3, e2b, "hhhlllllk");
	addMoves(3, g2b, "kkkkk");
	addMoves(3, n2b, "kkkkkkl");
	addMoves(3, l2b, "lllll");
	addMoves(3, r2b, "l");
	addMoves(3, t2b, "ll");
	addMoves(3, s2b, "lllll");
	addMoves(3, m2b, "jjlllll");
	addMoves(3, r2b, "jlllll");
	addMoves(3, k2b, "jjjllllll");
	addMoves(3, l2b, "lljjlllllll");
	addMoves(3, j2b, "jjjllllllllll");
	addMoves(3, o2b, "kllllllljjlllllllll");
	addMoves(3, p2b, "klllllljjllllllllll");
	addMoves(3, h2b, "hjlllllllljjlllllllllll");
	addMoves(3, g2b, "jjjjjjlllllllljjllllllllllll");
	addMoves(3, n2b, "hjjjjjlllllllljjlllllllllllll");
	addMoves(3, a2b, "jjhjjjjlllllllljjllllllllllllll");
	addMoves(3, b2b, "jk"); // looking for items beneath boulder
	addMoves(3, c2b, "jk"); // looking for items beneath boulder
	addMoves(3, d2b, "jk"); // looking for items beneath boulder
	addMoves(3, f2b, "hl"); // looking for items beneath boulder
	addMoves(3, q2b, "khhhllllllljjlllllllllllllll");
	addMoves(3, i2b, "jhhhllllllljjllllllllllllllll");

	/* Level 3A */
	Point a3a(8, 34);
	Point b3a(9, 34);
	Point c3a(9, 35);
	Point d3a(9, 37);
	Point e3a(9, 38);
	Point f3a(10, 32);
	Point g3a(10, 33);
	Point h3a(11, 35);
	Point i3a(12, 36);
	Point j3a(12, 39);
	Point k3a(13, 33);
	Point l3a(13, 34);
	Point m3a(13, 37);
	Point n3a(15, 36);
	Point o3a(16, 35);
	Point p3a(17, 35);
	addMoves(4, o3a, "hh");
	addMoves(4, p3a, "ll");
	addMoves(4, n3a, "jjll");
	addMoves(4, l3a, "jjj");
	addMoves(4, o3a, "k");
	addMoves(4, l3a, "lljlll");
	addMoves(4, o3a, "ljjllllll");
	addMoves(4, k3a, "ljjjjlllllll");
	addMoves(4, g3a, "jjjljjjjllllllll");
	addMoves(4, f3a, "ljjjljjjjlllllllll");
	addMoves(4, m3a, "llhhhhhjjjjllllllllll");
	addMoves(4, j3a, "kk");
	addMoves(4, i3a, "jjjjjlllllllll");
	addMoves(4, h3a, "ljjjjjjllllllllll");
	addMoves(4, a3a, "ll");
	addMoves(4, d3a, "k"); // looking for item beneath boulder
	addMoves(4, e3a, "k"); // looking for item beneath boulder
	/* XXX:
	 * we deviate slightly from spoiler solution on nethack wiki here to reduce amount of moves.
	 * we move the last boulder twice, then move a previously unmoved boulder to check for items,
	 * before we do the remaining moves with last boulder */
	addMoves(4, c3a, "jj");
	addMoves(4, b3a, "h"); // looking for items beneath boulder
	addMoves(4, c3a, "ljjjjjjlllllllllll");

	/* Level 3B */
	Point a3b(8, 32);
	Point b3b(8, 33);
	Point c3b(8, 37);
	Point d3b(8, 38);
	Point e3b(9, 35);
	Point f3b(9, 37);
	Point g3b(9, 40);
	Point h3b(9, 41);
	Point i3b(12, 36);
	Point j3b(13, 32);
	Point k3b(13, 35);
	Point l3b(14, 32);
	Point m3b(15, 33);
	addMoves(5, m3b, "hllllll");
	addMoves(5, j3b, "l");
	addMoves(5, l3b, "jlllllll");
	addMoves(5, j3b, "jjlllllll");
	addMoves(5, a3b, "jjjjjjjlllllllll");
	addMoves(5, b3b, "hjjjjjjjllllllllll");
	addMoves(5, e3b, "k");
	addMoves(5, d3b, "jj");
	addMoves(5, f3b, "hhhk");
	addMoves(5, e3b, "j");
	addMoves(5, f3b, "hhjjjjjjjlllllllllll");
	addMoves(5, e3b, "hkhhjjjjjjjllllllllllll");
	addMoves(5, c3b, "jhhhkhhjjjjjjjlllllllllllll");
	addMoves(5, g3b, "jjj"); // looking for items beneath boulder
	addMoves(5, h3b, "h"); // looking for items beneath boulder
	addMoves(5, i3b, "jjjllllllllll");
	addMoves(5, k3b, "jjhhhlllllllllllllll");

	/* Level 4A */
	Point a4a(9, 29);
	Point b4a(9, 31);
	Point c4a(9, 33);
	Point d4a(9, 35);
	Point e4a(9, 37);
	Point f4a(11, 30);
	Point g4a(11, 32);
	Point h4a(11, 35);
	Point i4a(11, 37);
	Point j4a(12, 30);
	Point k4a(14, 30);
	Point l4a(14, 32);
	Point m4a(14, 34);
	Point n4a(15, 33);
	Point o4a(16, 29);
	Point p4a(16, 31);
	Point q4a(16, 35);
	Point r4a(18, 29);
	addMoves(6, k4a, "h");
	addMoves(6, n4a, "lll");
	addMoves(6, r4a, "lll");
	addMoves(6, p4a, "lljjjhhhhkl");
	addMoves(6, f4a, "hh");
	addMoves(6, j4a, "ljjjjlljjjhhhhhk");
	addMoves(6, g4a, "hh");
	addMoves(6, h4a, "hh");
	addMoves(6, d4a, "hjjjhhhjjjjl");
	addMoves(6, c4a, "ljjjhhhjjj");
	addMoves(6, h4a, "lkkkhkkkl");
	addMoves(6, g4a, "llllkkkhkkkll");
	addMoves(6, f4a, "llllllkkkhkkklll");
	addMoves(6, i4a, "hhhkkkhkkkllll");
	addMoves(6, c4a, "kkkklllkkkhkkklllll");
	addMoves(6, k4a, "llkkklllkkkhkkkllllll");
	addMoves(6, l4a, "hkkklllkkkhkkklllllll");
	addMoves(6, m4a, "hhhkkklllkkkhkkkllllllll");
	addMoves(6, n4a, "hhhhhkkkklllkkkhkkklllllllll");
	addMoves(6, r4a, "lkkkkhhkkklllkkkhkkkllllllllll");
	addMoves(6, p4a, "lllkkkkhhkkklllkkkhkkklllllllllll");
	addMoves(6, o4a, "l"); // looking for items beneath boulder
	addMoves(6, q4a, "h"); // looking for items beneath boulder
	addMoves(6, j4a, "lllllkkkkhhkkklllkkkhkkkllllllllllll");
	addMoves(6, d4a, "ljjjhhhhhklllllkkkkhhkkklllkkkhkkklllllllllllll");
	addMoves(6, b4a, "llljjjhhhjjjkkkklllkkkhkkkllllllllllllll");
	addMoves(6, a4a, "llllljjjhhhjjjkkkklllkkkhkkklllllllllllllll");
	addMoves(6, e4a, "hhhjjjhhhjjjkkkklllkkkhkkkllllllllllllllll");

	/* Level 4B */
	Point a4b(8, 30);
	Point b4b(9, 37);
	Point c4b(10, 28);
	Point d4b(10, 29);
	Point e4b(10, 38);
	Point f4b(11, 30);
	Point g4b(11, 31);
	Point h4b(11, 36);
	Point i4b(11, 37);
	Point j4b(12, 28);
	Point k4b(12, 31);
	Point l4b(12, 36);
	Point m4b(13, 29);
	Point n4b(13, 30);
	Point o4b(13, 38);
	Point p4b(14, 29);
	Point q4b(14, 31);
	Point r4b(14, 37);
	Point s4b(16, 32);
	Point t4b(18, 33);
	addMoves(7, t4b, "lllkkkk");
	addMoves(7, r4b, "k");
	addMoves(7, o4b, "kk");
	addMoves(7, i4b, "k");
	addMoves(7, h4b, "hhh");
	addMoves(7, k4b, "hh");
	addMoves(7, n4b, "j");
	addMoves(7, f4b, "k");
	addMoves(7, g4b, "k");
	addMoves(7, c4b, "kj");
	addMoves(7, a4b, "kkkl");
	addMoves(7, f4b, "jjkkkkkkkll");
	addMoves(7, c4b, "k");
	addMoves(7, d4b, "lkkkkklll");
	addMoves(7, c4b, "llkkkkllll");
	addMoves(7, n4b, "kkkkkkkkklllll");
	addMoves(7, p4b, "lkkkkkkkkkllllll");
	addMoves(7, m4b, "lkkkkkkkklllllll");
	addMoves(7, k4b, "kkklkkkkllllllll");
	addMoves(7, j4b, "kkkllkkkklllllllll");
	addMoves(7, h4b, "jhhhkkkkkkkllllllllll");
	addMoves(7, r4b, "j");
	addMoves(7, l4b, "khhhjhhhkkkkkkklllllllllll");
	addMoves(7, t4b, "kkkhhhjhhhkkkkkkkllllllllllll");
	addMoves(7, r4b, "hkkkhhhjhhhkkkkkkklllllllllllll");
	addMoves(7, b4b, "l");
	addMoves(7, i4b, "jjjhkkhhhjhhhkkkkkkkllllllllllllll");
	addMoves(7, e4b, "hhjhhhjhhhkkkkkkklllllllllllllll");
	addMoves(7, b4b, "jhhjhhhjhhhkkkkkkkllllllllllllllll");
	addMoves(7, g4b, "jjhkkkkkkklllllllllllllllll");
	addMoves(7, s4b, "lh"); // looking for items beneath boulder
	addMoves(7, q4b, "kkhkkkkkkkllllllllllllllllll");
}

void Sokoban::addMoves(int level, Point& boulder, const string& moves) {
	/* translate moves to positions */
	unsigned char prev = ILLEGAL_DIRECTION;
	Point move = boulder;
	for (int m = 0; m < (int) moves.size(); ++m) {
		if (prev != moves[m]) {
			/* shifting direction, need to place her correctly */
			/* place "pos" at boulder by repeating last move unless it's the first move */
			if (prev != ILLEGAL_DIRECTION)
				move.moveDirection(moves[m - 1]);
			/* then move point to opposite direction of the direction we're going to push */
			switch (moves[m]) {
			case N:
				move.moveSouth();
				break;

			case E:
				move.moveWest();
				break;

			case S:
				move.moveNorth();
				break;

			case W:
				move.moveEast();
				break;

			default:
				Debug::error() << "Error parsing sokoban movement '" << moves[m] << "'" << endl;
				break;
			}
			/* then add position to move to before continue pushing */
			prev = (unsigned char) moves[m];
		}
		_moves[level].push_back(make_pair(move, moves[m]));
		move.moveDirection(moves[m]);
		boulder.moveDirection(moves[m]);
	}
}

void Sokoban::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_PERHAPS_THATS_WHY) != string::npos) {
		/* monster blocking the way, should be marked as "I", no handling for the time being */
	}
}

void Sokoban::analyze() {
	if (_given_up || World::currentPriority() >= SOKOBAN_SOLVE_PRIORITY)
		return;

	if (_retry_turn > int(World::turn())) {
		// We don't want the normal desparation actions running - we aren't that desparate.
		World::setAction(static_cast<action::Action*> (new action::Search(this, SOKOBAN_REST_PRIORITY)));
		return;
	}

	for (unsigned ix = 0; ix < World::levels().size(); ++ix) {
		Level& lev = World::level(ix);

		if (lev.branch() != BRANCH_SOKOBAN)
			continue;

		int level = -1;
		/* we need to check if the entry exists, otherwise we get the default value (0), which cause issues */
		if (_levelmap.find(ix) != _levelmap.end())
			level = _levelmap[ix];
		if (level < 0) {
			for (int s = 0; s < (int) _moves.size(); ++s) {
				if (_moves[s].size() == 0)
					continue; // seems like this level is completed
				/* 1st. entry should be a unique boulder for each level */
				deque<pair<Point, char> >::iterator f = _moves[s].begin();
				Point b = f->first;
				b.moveDirection(f->second);
				if (lev.tile(b).symbol() != BOULDER)
					continue; // expected boulder, can't be this level
				/* this must be it */
				level = s;
				_levelmap[ix] = level;
				Debug::custom(name()) << "Recognized " << ix << " as sokoban level " << s << endl;
				/* clear alternative level so we don't mistakenly identifies a later level as it */
				_moves[s^1].clear();
				break;
			}
		}
		if (level < 0) {
			/* huh? */
			Debug::warning() << "Sokoban was unable to recognize level " << ix << endl;
			continue;
		}
		if (_moves[level].size() <= 0) {
			map<Point, int>::const_iterator pi = lev.symbols(STAIRS_UP).begin();
			if (pi != lev.symbols(STAIRS_UP).end() && pi->second == UNKNOWN_SYMBOL_VALUE) {
				Debug::custom(name()) << "Heading to unvisited Sokoban level at depth " << lev.depth() - 1 << endl;
				Tile tile = World::shortestPath(Coordinate(ix, pi->first));
				if (tile.cost() >= UNPASSABLE)
					continue;
				if (tile.direction() == NOWHERE)
					tile.direction(UP);
				World::setAction(static_cast<action::Action*> (new action::Move(this, tile, SOKOBAN_SOLVE_PRIORITY)));
			}
			continue;
		}

		/* go to next point */
		pair<Point, char> next_move = _moves[level].front();
		Point bopos = next_move.first;
		bopos.moveDirection(next_move.second);
		Point bnpos = bopos;
		bnpos.moveDirection(next_move.second);

		if (Level::isPassable(lev.tile(bopos).symbol())) {
			// the move looks complete, erase it and go to next
			_moves[level].pop_front();
			_retry_count = 0;
			if (_moves[level].size() > 0) {
				next_move = _moves[level].front();
				bopos = next_move.first;
				bopos.moveDirection(next_move.second);
				bnpos = bopos;
				bnpos.moveDirection(next_move.second);
			} else {
				/* look at that, we've solved this sokoban level */
				Debug::custom(name()) << "Solved sokoban level " << level << endl;
				/* let's "give up" solving sokoban, nothing left to solve */
				if (level >= 6)
					_given_up = true;
				continue;
			}
		}
		Tile tile = World::shortestPath(Coordinate(ix, next_move.first));
		if (tile.cost() == UNREACHABLE) {
			/* this is bad */
			Debug::custom(name()) << "Unable to move to " << tile << endl;
			return;
		} else if (tile.direction() == NOWHERE) {
			if (lev.tile(bopos).symbol() == BOULDER) {
				/* pushing boulder */
				tile.direction(next_move.second);
				Debug::custom(name()) << "Pushing a boulder: " << lev.tile(bopos) << " (" << next_move.second << ")" << endl;
			} else if (lev.tile(bopos).symbol() == UNKNOWN_TILE_UNPASSABLE) {
				Debug::custom(name()) << "Failed to push boulder" << endl;
				if (_retry_count < RETRY_COUNT) {
					lev.tile(bopos).symbol(BOULDER);
					++_retry_count;
					_retry_turn = World::turn() + TURNS_BETWEEN_RETRIES;
				} else {
					_given_up = true;
					return;
				}
			} else {
				/* uh, not good at all */
				Debug::custom(name()) << "Wanted to push a non-boulder: " << lev.tile(bopos) << endl;
				_given_up = true;
				return;
			}
		} else if (tile.cost() == UNPASSABLE) {
			/* possibly monster in the way */
			Debug::custom(name()) << "Tried to move on to an unpassable tile: " << lev.tile(bopos) << endl;
			return;
		} else {
			/* probably moving to correct point before pushing boulder */
			Debug::custom(name()) << "Moving to the right spot to push a boulder: " << tile << endl;
		}
		World::setAction(static_cast<action::Action*> (new action::Move(this, tile, SOKOBAN_SOLVE_PRIORITY)));
	}

	// odds and ends to make Sokoban work better - the loop above is the heart of exploration
	// we count on the loop above to assign level IDs
	if (World::level().branch() == BRANCH_SOKOBAN && _levelmap[Saiph::position().level()] >= 2) {
		for (map<Point, int>::const_iterator pi = World::level().symbols(TRAP).begin(); pi != World::level().symbols(TRAP).end(); ++pi) {
			Tile& tl = World::level().tile(pi->first);
			if (tl.cost() == UNREACHABLE)
				continue;
			if (World::view(pi->first) == '^')
				continue;
			Debug::custom(name()) << "Something maybe worth clearing at " << tl << endl;
			if (Point::gridDistance(Saiph::position(), pi->first) > 1)
				World::setAction(static_cast<action::Action*> (new action::Move(this, tl, SOKOBAN_CLEAR_ITEMS_PRIORITY)));
			else if (tl.monster() == ILLEGAL_MONSTER)
				World::setAction(static_cast<action::Action*> (new action::Kick(this, tl.direction(), SOKOBAN_CLEAR_ITEMS_PRIORITY)));
		}
	}
}
