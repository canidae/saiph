#ifndef PASSTUNE_H
#define	PASSTUNE_H

#include <vector>
#include <string>
#include "Analyzer.h"
#include "../Saiph.h"

#define PASSTUNE_DEBUG_NAME "Passtune] "

#define PASSTUNE_TUMBLER " tumbler"
#define PASSTUNE_GEAR " gear"

#define PASSTUNE_MESSAGE_YOU_HEAR "  You hear "

#define PLACES 5
#define NOTES 7
#define TOTAL_COMBINATIONS 16807 // 5**7

#define UNKNOWN -1

class Passtune : public Analyzer {
	public:
		Passtune(Saiph *saiph);

		void parseMessages(const std::string &messages);
		void analyze(void);

	private:
		Saiph *saiph;
		bool solved;
		char instrument;
		bool play_instrument;
		int castle_level;
		Point drawbridge_location;
		std::vector<Point> knights_moves;

		std::vector<bool> discarded;

		char guess[PLACES+1];

		int nextGuess(int gears, int tumblers);

		void findInstrument(void);
		bool isInstrument(const Item &item);
};
#endif
