#ifndef ACTION_LIST_PLAYER_ATTRIBUTES_H
#define	ACTION_LIST_PLAYER_ATTRIBUTES_H

#include "World.h"
#include "Actions/Action.h"

namespace action {
	class ListPlayerAttributes : public Action {
	public:
		static const int ID;

		ListPlayerAttributes(analyzer::Analyzer* analyzer);
		virtual ~ListPlayerAttributes();

		virtual int id();
		virtual const Command& command();
		virtual void update(const std::string&);

	private:
		const Command _list_attributes;
		const Command _close_page;
	};
}
#endif
