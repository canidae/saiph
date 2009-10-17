#ifndef ANALYZER_HEALTH_H
#define ANALYZER_HEALTH_H

#include "Analyzer.h"

/* TODO: clean this up */
#define PRIORITY_HEALTH_CURE_POLYMORPH 925
#define PRIORITY_HEALTH_CURE_DEADLY 900
#define PRIORITY_HEALTH_PRAY_FOR_HP 900
#define PRIORITY_HEALTH_REST_FOR_HP_LOW 700
#define PRIORITY_HEALTH_QUAFF_FOR_HP 700
#define PRIORITY_HEALTH_REST_FOR_HP_HIGH 500
#define PRIORITY_HEALTH_CURE_NON_DEADLY 480
#define PRIORITY_HEALTH_CURE_LYCANTHROPY 400

namespace analyzer {

	class Health : public Analyzer {
	public:
		Health();

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		bool _resting;
		int _prev_str;
		int _prev_dex;
		int _prev_con;
		int _prev_int;
		int _prev_wis;
		int _prev_cha;
	};
}
#endif
