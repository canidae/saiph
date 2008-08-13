#ifndef HEALTH_H
/* defines */
#define HEALTH_H
/* priorities */
#define HEALTH_CURE_LYCANTHROPY 150
#define HEALTH_REST_FOR_HP_HIGH 500
#define HEALTH_REST_FOR_HP_LOW 700
#define HEALTH_PRAY_FOR_HP 900
#define HEALTH_UNIHORN_NON_DEADLY 450
#define HEALTH_UNIHORN_DEADLY 800
#define HEALTH_PRAY_DEADLY 900
/* messages */
#define HEALTH_FEEL_FEVERISH "  You feel feverish.  "
#define HEALTH_FEEL_PURIFIED "  You feel purified.  "

/* forward declare */
class Health;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Health : public Analyzer {
	public:
		/* constructors */
		Health(Saiph *saiph);

		/* methods */
		void analyze();
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		bool resting;
		bool lycanthropy;
		Request req;
};
#endif
