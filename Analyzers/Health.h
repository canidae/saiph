#ifndef HEALTH_H
/* defines */
#define HEALTH_H
/* priorities */
#define HEALTH_REST_FOR_HP_HIGH 500
#define HEALTH_REST_FOR_HP_LOW 700
#define HEALTH_PRAY_FOR_HP 900

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
		void finish();

	private:
		/* variables */
		Saiph *saiph;
		bool resting;
		Request req;
};
#endif
