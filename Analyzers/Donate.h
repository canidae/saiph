#ifndef DONATE_H
#define	DONATE_H

#define DONATE_HOW_MUCH_OFFER "  How much will you offer?  "
#define DONATE_TALK_TO_WHOM "  Talk to whom? "

#include "../Analyzer.h"
#include "../Saiph.h"

class Donate : public Analyzer {
	public:
		Donate(Saiph *saiph);
		void parseMessages(const std::string &messages);
		void analyze();

	private:
		Saiph *saiph;
		unsigned char priest_dir;
		Coordinate priest_loc;
};
#endif
