#ifndef ACTION_ANSWER_H
#define ACTION_ANSWER_H

#include "Actions/Action.h"

namespace action {

	class Answer : public Action {
	public:
		static const int ID;

		Answer(analyzer::Analyzer* analyzer, const std::string& answer);
		Answer(analyzer::Analyzer* analyzer, unsigned char answer);
		virtual ~Answer();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		const Command _answer;
	};
}
#endif
