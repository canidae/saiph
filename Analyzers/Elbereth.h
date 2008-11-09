#ifndef ELBERETH_H
#define ELBERETH_H
/* Elbereth */
#define ELBERETH_ELBERETH "Elbereth"

#include <string>
#include "../Analyzer.h"

class Request;
class Saiph;

class Elbereth : public Analyzer {
	public:
		Elbereth(Saiph *saiph);

		void complete();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		int last_look_internal_turn;
		int elbereth_count;
		bool burned;
		bool digged;
		bool dusted;
		bool frosted;

		bool canEngrave();
};
#endif
