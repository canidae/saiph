#ifndef WISH_H
#define WISH_H
/* messages */
#define WISH_MESSAGE "  For what do you wish?  "

#include <string>
#include <vector>
#include "../Analyzer.h"

class Saiph;

class Wish : public Analyzer {
	public:
		Wish(Saiph *saiph);

		void parseMessages(const std::string &messages);
		void analyze();
	private:
		Saiph *saiph;
		bool MRarmor, MRcloak, haveMR;
		bool reflectionArmor, reflectionAmulet, reflectionShield, haveReflection;
		unsigned char wand_of_wishing_key;
		std::string selectWish();
		bool wearing(const std::string& name);
};
#endif
