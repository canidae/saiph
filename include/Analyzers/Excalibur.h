#ifndef ANALYZER_EXCALIBUR_H
#define ANALYZER_EXCALIBUR_H

#include <string>
#include "Analyzers/Analyzer.h"

#define PRIORITY_EXCALIBUR_DIP 200
#define EXCALIBUR_LONG_SWORD "long sword"

namespace analyzer {

	class Excalibur : public Analyzer {
	public:
		Excalibur();

		void analyze();
		void parseMessages(const std::string& messages);
		void onEvent(event::Event * const event);

	private:
		unsigned char _long_sword_key;
	};
}
#endif
