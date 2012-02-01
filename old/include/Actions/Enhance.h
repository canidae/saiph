#ifndef ACTION_ENHANCE_H
#define ACTION_ENHANCE_H

#include "Actions/Action.h"
#include "Command.h"
#include "Data/Skill.h"

#include <string>

namespace action {
	class Enhance : public Action {
	public:
		static const int ID;

		Enhance(analyzer::Analyzer* analyzer);
		virtual ~Enhance();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		const Command _do_enhance;
		const Command _next_page;
		const Command _enhance_a;
		int _results[P_NUM_SKILLS];
	};
}
#endif
