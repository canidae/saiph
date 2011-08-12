#ifndef ACTION_CALL_MONSTER_H
#define ACTION_CALL_MONSTER_H

#include "Actions/Action.h"
#include "Point.h"
#include <vector>
#include <string>

namespace action {
	class CallMonster : public Action {
	public:
		static const int ID;

		CallMonster(analyzer::Analyzer* analyzer, const std::vector<std::pair<Point, std::string> >& requests);
		virtual ~CallMonster();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		std::vector<std::pair<Point, std::string> > _requests;
		Command _command;

		static int _nao_christen; // +1 means extra 'a', 0 means probing needed
	};
}
#endif
