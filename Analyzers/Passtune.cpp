#include <math.h>
#include <stdlib.h>
#include "Passtune.h"
#include "../Globals.h"
#include "../World.h"
#include "../Debug.h"

using namespace std;

Passtune::Passtune(Saiph *saiph) : Analyzer("Passtune"), saiph(saiph), solved(false), instrument(ILLEGAL_ITEM), castle_level(-1) {
	drawbridge_location = Point(12, 14);

	knights_moves.push_back(Point(11, 12));
	knights_moves.push_back(Point(13, 12));

	guess[0] = 0;

	discarded.resize(TOTAL_COMBINATIONS);

	Debug::info() << PASSTUNE_DEBUG_NAME << "Resizing to " << TOTAL_COMBINATIONS << endl;
}

void Passtune::parseMessages(const string &messages) {
	string::size_type gear_loc, tumbler_loc;
	if (messages.find(MESSAGE_DRAWBRIDGE_LOWERED, 0) != string::npos) {
		if (saiph->position.level == castle_level)
			solved = true;
	} else if (!solved && messages.find(PASSTUNE_MESSAGE_YOU_HEAR, 0) != string::npos &&
			((gear_loc = messages.find(PASSTUNE_GEAR, 0)) != string::npos ||
			(tumbler_loc = messages.find(PASSTUNE_TUMBLER, 0)) != string::npos)) {
		int gears = 0, tumblers = 0;

		if (gear_loc != string::npos) {
			gears = atoi(messages.substr(gear_loc - 1, 1).c_str());

			// optimized out if gears matched
			tumbler_loc = messages.find(PASSTUNE_TUMBLER, 0);
		}

		if (tumbler_loc != string::npos)
			tumblers = atoi(messages.substr(tumbler_loc - 1, 1).c_str());

		Debug::info(saiph->last_turn) << PASSTUNE_DEBUG_NAME << gears << " gears and " << tumblers << " tumblers, getting next tune to play" << endl;
		nextGuess(gears, tumblers);
		command = APPLY;
		priority = PRIORITY_LOWER_DRAWBRIDGE;
	}

	if (!saiph->world->question)
		return;
	if (messages.find(MESSAGE_PLAY_PASSTUNE, 0) != string::npos) {
		solved = true;
		command = YES;
		priority = PRIORITY_CONTINUE_ACTION;
		return;
	} else if (guess[0] == 0) {
		return;
	}

	if (messages.find(MESSAGE_WHAT_TO_APPLY, 0) != string::npos) {
		command = instrument;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_IMPROVISE, 0) != string::npos) {
		command = NO;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_WHAT_TUNE, 0) != string::npos) {
		command = guess;
		command.push_back('\n');
		priority = PRIORITY_CONTINUE_ACTION;
	}
}

void Passtune::analyze(void) {
	if (saiph->inventory_changed)
		findInstrument();

	if (castle_level == -1 && saiph->levels[saiph->position.level].depth >= 25 &&
		saiph->levels[saiph->position.level].depth <= 29) {
		unsigned char symbol = saiph->getDungeonSymbol(drawbridge_location);
		if (symbol == RAISED_DRAWBRIDGE || symbol == LOWERED_DRAWBRIDGE) {
			castle_level = saiph->position.level;
			Debug::info(saiph->last_turn) << PASSTUNE_DEBUG_NAME << "Identified castle level" << endl;
		}
	}

	if (!solved && instrument != ILLEGAL_ITEM &&
		castle_level == saiph->position.level &&
		priority <= PRIORITY_LOWER_DRAWBRIDGE) {

		unsigned int nearest = UNREACHABLE;
		unsigned char dir = ILLEGAL_DIRECTION;
		for (unsigned int i = 0; i < knights_moves.size(); ++i) {
			const PathNode &node = saiph->shortestPath(knights_moves[i]);
			if (node.cost >= UNPASSABLE || node.cost >= nearest)
				continue;
			dir = node.dir;
			nearest = node.cost;
		}

		if (nearest == UNREACHABLE) {
			Debug::warning(saiph->last_turn) << PASSTUNE_DEBUG_NAME << "Unable to path to a knight's move away from the castle drawbridge." << endl;
			return;
		} else if (dir != NOWHERE) {
			command = dir;
			priority = PRIORITY_LOWER_DRAWBRIDGE;
			return;
		}

		// Make sure we try our previous guess first.
		if (guess[0] == 0)
			nextGuess(UNKNOWN, UNKNOWN);

		Debug::info(saiph->last_turn) << PASSTUNE_DEBUG_NAME << "Trying to play: " << guess << endl;
		command = APPLY;
		priority = PRIORITY_LOWER_DRAWBRIDGE;
	}
}

void Passtune::nextGuess(int gears, int tumblers) {
	int possible = 0;

	if (gears != UNKNOWN && tumblers != UNKNOWN) {
		for (int c = 0; c < TOTAL_COMBINATIONS; ++c) {
			if (!discarded[c]) {
				bool used[PLACES];
				for (int a = 0; a < PLACES; ++a)
					used[a] = false;

				char current[PLACES];

				for (int i = 0; i < PLACES; ++i)
					current[i] = 'A' + ((int)(c / pow((float)NOTES, i)) % NOTES);

				int current_gears = 0, current_tumblers = 0;

				// Number of notes in the right place
				for (int a = 0; a < PLACES; ++a)
					if (guess[a] == current[a]) {
						current_gears++;
						current[a] = -1;
						used[a] = true;
					}

				// Number of notes in the wrong place
				for (int a = 0; a < PLACES; ++a)
					if (!used[a])
						for (int b = 0; b < PLACES; ++b)
							if (guess[a] == current[b]) {
								current_tumblers++;
								current[b] = -1;
								break;
							}

				if (tumblers != current_tumblers ||
					gears != current_gears)
					discarded[c] = true;
			}

			if (!discarded[c])
				possible++;
		}
		Debug::info() << PASSTUNE_DEBUG_NAME << "Possible combinations left " << possible << endl;
	}

	int total = 0;
	vector<int> temp;

	for (unsigned int i = 0; i < discarded.size(); ++i)
		if (!discarded[i]) {
			++total;
			temp.push_back(i);
		}

	int current_combination = temp[random() % total];
	temp.clear();

	for (int n = 0; n < PLACES; ++n)
		guess[n] = 'A' + ((int)(current_combination / pow((float)NOTES, n)) % NOTES);

	Debug::info() << PASSTUNE_DEBUG_NAME << possible << " combinations left, guessing " << guess << endl;
}

void Passtune::findInstrument() {
	map<unsigned char, Item>::iterator i = saiph->inventory.find(instrument);
	if (i != saiph->inventory.end() && isInstrument(i->second))
		return;
	for (i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (isInstrument(i->second)) {
			instrument = i->first;
			return;
		}
	}
	instrument = ILLEGAL_ITEM;
}

bool Passtune::isInstrument(const Item &item) {
	if (item.beatitude == CURSED ||
		(item.name != "flute" && item.name != "horn" &&
		item.name != "harp" && item.name != "bugle" &&
		item.name != "magic harp"))
		return false;
	return true;
}

