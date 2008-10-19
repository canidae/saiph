#ifndef HEALTH_H
#define HEALTH_H
/* priorities */
#define HEALTH_CURE_DEADLY 900
#define HEALTH_CURE_LYCANTHROPY 400
#define HEALTH_CURE_NON_DEADLY 450
#define HEALTH_REST_FOR_HP_HIGH 500
#define HEALTH_REST_FOR_HP_LOW 700
#define HEALTH_PRAY_FOR_HP 900
/* messages */
#define HEALTH_FEEL_FEVERISH "  You feel feverish.  "
#define HEALTH_FEEL_PURIFIED "  You feel purified.  "

#include "../Analyzer.h"
#include "../Request.h"

class Saiph;

class Health : public Analyzer {
	public:
		Health(Saiph *saiph);

		void analyze();

	private:
		Saiph *saiph;
		bool resting;
		int prev_st;
		int prev_dx;
		int prev_co;
		int prev_in;
		int prev_wi;
		int prev_ch;
		Request req;
};
#endif
