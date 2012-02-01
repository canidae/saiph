#ifndef ACTION_FAR_LOOK_H
#define ACTION_FAR_LOOK_H

#include "Actions/Action.h"
#include "Point.h"
#include <vector>
#include <string>

namespace action {
	class FarLook : public Action {
	public:
		static const int ID;

		class Request {
		public:
			Request(const Point& where);
			Point where;
			std::string result;
		};

		FarLook(analyzer::Analyzer* analyzer, std::vector<Request>& requests);
		virtual ~FarLook();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		std::vector<Request>::iterator _current;
		std::vector<Request>::iterator _end;
		Command _command;
	};
}
#endif
