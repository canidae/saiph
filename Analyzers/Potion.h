#ifndef POTION_H
#define POTION_H
/* messages */
#define POTION_CALL_END " potion:  "
#define POTION_CALL_POTION1 "  Call a "
#define POTION_CALL_POTION2 "  Call an "

#include <string>
#include <vector>
#include "../Analyzer.h"

class Saiph;

class Potion : public Analyzer {
	public:
		Potion(Saiph *saiph);

		void complete();
		void parseMessages(const std::string &messages);

	private:
		std::vector<std::string> appearance;
		Saiph *saiph;
};
#endif
