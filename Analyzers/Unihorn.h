#ifndef UNIHORN_H
#define UNIHORN_H
/* turns between each time we'll use the unihorn */
#define UNIHORN_UNIHORN_TIMEOUT 3
/* messages */
#define UNIHORN_NOTHING_HAPPENS "  Nothing happens.  " // nothing can be fixed
#define UNIHORN_NOTHING_SEEMS_TO_HAPPEN "  Nothing seems to happen.  " // something can be fixed, but it wasn't

#include <string>
#include "../Analyzer.h"

class Request;
class Saiph;

class Unihorn : public Analyzer {
	public:
		Unihorn(Saiph *saiph);

		void analyze();
		void complete();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		unsigned char unihorn_key;
		int apply_priority;
		int sequence;

		void findUnihorn();
};
#endif
