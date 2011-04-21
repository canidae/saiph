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
		bool wished_for_charging;
		bool zapping_wand;
		std::string selectWish();
		unsigned long long extrinsics;
		unsigned long long currentExtrinsics();
		bool wearing(const std::string& name);
	};
}
#endif
