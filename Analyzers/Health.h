#ifndef HEALTH_H
#define HEALTH_H

#include "Analyzer.h"
#include "../Request.h"

class Saiph;

class Health : public Analyzer {
	public:
		Health(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		bool resting;
		int prev_st;
		int prev_dx;
		int prev_co;
		int prev_in;
		int prev_wi;
		int prev_ch;
		Request req;
};
#endif
