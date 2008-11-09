#ifndef UNIHORN_H
#define UNIHORN_H

#include <string>
#include "../Analyzer.h"

class Request;
class Saiph;

class Unihorn : public Analyzer {
	public:
		Unihorn(Saiph *saiph);

		void complete();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		unsigned char unihorn_key;

		void findUnihorn();
};
#endif
