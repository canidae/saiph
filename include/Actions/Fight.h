#ifndef ACTION_FIGHT_H
#define ACTION_FIGHT_H

#include "Actions/Action.h"

namespace action {
	class Fight : public Action {
	public:
		static const int ID;

		Fight(analyzer::Analyzer* analyzer, unsigned char direction, int priority, bool attack_friendly = true);
		virtual ~Fight();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _fight;
		const Command _attack_friendly;
	};
}
#endif
