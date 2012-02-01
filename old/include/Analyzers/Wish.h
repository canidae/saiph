#ifndef ANALYZER_WISH_H
#define ANALYZER_WISH_H

#include <string>
#include <vector>
#include "Analyzers/Analyzer.h"

namespace analyzer {
	class Wish : public Analyzer {
	public:
		Wish();

		void parseMessages(const std::string& messages);
		void actionCompleted(const std::string& messages);
		void analyze();
		void onEvent(event::Event* const event);

	private:
		unsigned char wand_of_wishing_key;
		unsigned char charging_key;
		std::string _last_wish_call;
		std::string selectWish(bool from_wand);
		bool have(const std::string& name);
	};
}
#endif
