#ifndef UNIHORN_H
/* defines */
#define UNIHORN_H
/* turns between each time we'll use the unihorn */
#define UNIHORN_UNIHORN_TIMEOUT 3
/* messages */
#define UNIHORN_NOTHING_HAPPENS "  Nothing happens.  " // nothing can be fixed
#define UNIHORN_NOTHING_SEEMS_TO_HAPPEN "  Nothing seems to happen.  " // something can be fixed, but it wasn't

/* forward declare */
class Unihorn;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Unihorn : public Analyzer {
	public:
		/* constructors */
		Unihorn(Saiph *saiph);

		/* methods */
		void analyze();
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		unsigned char unihorn_key;
		int apply_priority;

		/* methods */
		void findUnihorn();
};
#endif
