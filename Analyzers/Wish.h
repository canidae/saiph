#ifndef WISH_H
#define WISH_H
#define WISH_DEBUG_NAME "Wish] "

#include <string>
#include <vector>
#include "../Analyzer.h"

class Saiph;

class Wish : public Analyzer {
	public:
		Wish(Saiph *saiph);

		void analyze(const std::string &messages);

	private:
		Saiph *saiph;
		bool MRarmor, MRcloak, haveMR;
		bool reflectionArmor, reflectionAmulet, reflectionShield, haveReflection;
		unsigned char wand_of_wishing_key;
		bool zapping_wand;

		void parseMessages(const std::string &messages);
		std::string selectWish();
		bool wearing(const std::string& name);
};
#endif
