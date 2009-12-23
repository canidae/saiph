#ifndef ANALYZER_DONATE_H
#define ANALYZER_DONATE_H

#include "Coordinate.h"
#include "Analyzers/Analyzer.h"

#define DONATE_HOW_MUCH_OFFER "  How much will you offer?  "
#define DONATE_TALK_TO_WHOM "  Talk to whom? "

class Saiph;

namespace analyzer {
	class Donate : public Analyzer {
	public:
		Donate(Saiph* saiph);

		void parseMessages(const std::string& messages);
		void analyze();

	private:
		Saiph* saiph;
		unsigned char priest_dir;
		Coordinate priest_loc;
	};
}
#endif
