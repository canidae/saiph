#ifndef WISH_H
#define WISH_H

#include <string>
#include <vector>
#include "../Analyzer.h"

class Saiph;

class Wish : public Analyzer {
	public:
		Wish(Saiph *saiph);

		void complete();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		std::vector<std::string> wishes;
};
#endif
