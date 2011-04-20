#ifndef ANALYZER_QUEST_H
#define ANALYZER_QUEST_H

#include <map>
#include <string>
#include "Point.h"
#include "Analyzers/Analyzer.h"

/* priorities */
#define PRIORITY_QUEST_GET_QUEST 200

namespace analyzer {

	class Quest : public Analyzer {
	public:
		Quest();

		void analyze();
		void parseMessages(const std::string& messages);
		void onEvent(event::Event* const event);

	private:
		void setStatus(int, int);

		Point _leader_pos;
		int _status;
		int _portal_level;
		bool _seen_bell;
		bool _seen_arti;
		std::map<int, std::string> artifacts;
		std::map<int, std::string> accepted_messages;
	};
}
#endif
