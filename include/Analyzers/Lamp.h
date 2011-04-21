#ifndef ANALYZER_LAMP_H
#define ANALYZER_LAMP_H

#include <string>
#include "Analyzers/Analyzer.h"

/* messages */
// This lamp has no oil.
// Your lamp flickers.
// Your lamp flickers considerably.
// Your lamp seems about to go out.
// Your lamp has gone out.
// Your oil lamp flickers.
// Your oil lamp flickers considerably.
// Your oil lamp seems about to go out.
// Your oil lamp has gone out.
// Your lantern is getting dim.
// Your lamp has run out of power.
// Your lamp is now on.
// Your lamp is now off.
// Your lamp goes out!
// Your brass lantern goes out!
#define LAMP_NO_OIL " lamp has no oil.  "
#define LAMP_LAMP_OUT " lamp has gone out.  "
#define LAMP_OIL_LAMP_OUT " lamp has gone out.  "
#define LAMP_LANTERN_OUT "  Your lantern has run out of power.  "
#define LAMP_LAMP_OUT_OF_POWER " lamp has run out of power.  "
#define LAMP_TURNED_ON " lamp is now on.  "
#define LAMP_TURNED_OFF " lamp is now off.  "
#define LAMP_LAMP_GOES_OUT " lamp goes out!  "
#define LAMP_LANTERN_GOES_OUT "  Your brass lantern goes out!  "
#define LAMP_LIT "lit"
/* threshold for turning lamp on/off */
#define LAMP_LIT_AREAS_THRESHOLD 0

namespace analyzer {

	class Lamp : public Analyzer {
	public:
		Lamp();

		void analyze();
		void parseMessages(const std::string& messages);
		void onEvent(event::Event* const event);

	private:
		unsigned char _lamp_key;
		bool _seen_oil_lamp;
		bool _seen_magic_lamp;

		void findLamp();
	};
}
#endif
