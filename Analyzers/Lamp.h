#ifndef LAMP_H
#define LAMP_H
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
#define LAMP_NO_OIL "  This lamp has no oil.  "
#define LAMP_LAMP_OUT "  Your lamp has gone out.  "
#define LAMP_OIL_LAMP_OUT "  Your oil lamp has gone out.  "
#define LAMP_LANTERN_OUT "  Your lamp has run out of power.  "
#define LAMP_TURNED_ON "  Your lamp is now on.  "
#define LAMP_TURNED_OFF "  Your lamp is now off.  "
/* threshold for turning lamp on/off */
#define LAMP_LIT_AREAS_THRESHOLD 0
/* priorities */
#define LAMP_PRIORITY_TOGGLE 300

#include <string>
#include "../Analyzer.h"
#include "../Request.h"

class Request;
class Saiph;

class Lamp : public Analyzer {
	public:
		Lamp(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		Request req;
		unsigned char lamp_key;

		void findLamp();
		bool isAreaDark();
};
#endif
