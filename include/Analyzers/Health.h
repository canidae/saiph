#ifndef ANALYZER_HEALTH_H
#define ANALYZER_HEALTH_H

#include "Analyzers/Analyzer.h"

/* TODO: clean this up */
#define PRIORITY_HEALTH_CURE_STONING 950
#define PRIORITY_HEALTH_CURE_POLYMORPH 925
#define PRIORITY_HEALTH_CURE_DEADLY 900
#define PRIORITY_HEALTH_PRAY_FOR_HP 900
#define PRIORITY_HEALTH_REST_FOR_HP_LOW 700
#define PRIORITY_HEALTH_QUAFF_FOR_HP 700
#define PRIORITY_HEALTH_REST_FOR_HP_HIGH 500
#define PRIORITY_HEALTH_CURE_NON_DEADLY 480
#define PRIORITY_HEALTH_CURE_LYCANTHROPY 400
/* turns we'll have to wait before applying unihorn again after successful use */
#define UNIHORN_UNIHORN_TIMEOUT 5
/* messages */
#define UNIHORN_NOTHING_HAPPENS "  Nothing happens.  " // nothing can be fixed
#define UNIHORN_NOTHING_SEEMS_TO_HAPPEN "  Nothing seems to happen.  " // something can be fixed, but it wasn't

namespace analyzer {

	class Health : public Analyzer {
	public:
		Health();

		void analyze();
		void parseMessages(const std::string& messages);
		void onEvent(event::Event * const event);

	private:
		bool _resting;
		bool _was_polymorphed;
		int _prev_str;
		int _prev_dex;
		int _prev_con;
		int _prev_int;
		int _prev_wis;
		int _prev_cha;
		unsigned char _lizard_key;
		unsigned char _unihorn_key;
		unsigned int _unihorn_use_turn;
		int _unihorn_priority;

		bool canApplyUnihorn();
	};
}
#endif
