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
		void analyze();
		void onEvent(event::Event* const event);

	private:
		bool MRarmor, MRcloak, haveMR;
		bool reflectionArmor, reflectionAmulet, reflectionShield, haveReflection;
		unsigned char wand_of_wishing_key;
		unsigned char charging_key;
		bool named_empty;
		bool named_full;
		bool wished_for_charging;
		bool wished_for_aor;
		bool wished_for_speed;
		bool wished_for_gop;
		bool will_wish;
		std::string selectWish();
		bool wearing(const std::string& name);
	};
}
#endif
