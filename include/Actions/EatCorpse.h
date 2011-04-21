#ifndef ACTION_EAT_CORPSE_H
#define ACTION_EAT_CORPSE_H

#include "Actions/Action.h"

namespace action {
	class EatCorpse : public Action {
	public:
		static const int ID;

		EatCorpse(analyzer::Analyzer* analyzer, const std::string& corpse, int priority);
		virtual ~EatCorpse();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string& messages);

	private:
		const Command _eat;
		const std::string _corpse;
		const Command _answer_no;
		const Command _answer_yes;
		const Command _escape;
	};
}
#endif
