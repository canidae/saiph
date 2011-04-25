#ifndef ANALYZER_MEDUSA_H
#define ANALYZER_MEDUSA_H

#include "Analyzers/Analyzer.h"

/* priorities */
// NOTE: We currently have no way to stop saiph from walking up into Medusa's death glare, except to run a higher priority action!  So KILL_MEDUSA must be higher priority than all actions that like to walk on general levels.
#define PRIORITY_MEDUSA_DIG_DOWN 100
#define PRIORITY_MEDUSA_KILL_MEDUSA 300

namespace analyzer {
	class Medusa : public Analyzer {
	public:
		Medusa();

		void analyze();
		void onEvent(event::Event* const);

	private:
		int _medusa_level;
		unsigned _medusa_done_turn;
		bool _fighting_medusa;
		bool _medusa_killed;
		bool _wearing_blinder;
		unsigned char _pick_key;
	};
}
#endif
