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

	private:
		Saiph *saiph;
		std::vector<std::string> wishes;
};
#endif
