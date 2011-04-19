#ifndef ANALYZER_DIGGING_TOOL_H
#define ANALYZER_DIGGING_TOOL_H

#include <string>
#include <map>
#include "Analyzers/Analyzer.h"

class Item;

namespace analyzer {

	class DiggingTool : public Analyzer {
	public:
		DiggingTool();

		void onEvent(event::Event* const event);

	private:
		int scoreDigger(const Item& item);
		void rankDiggers(int&, int&, const std::map<unsigned char, Item>*, const std::map<unsigned char, Item>*);
		void findDigger();

		int _digging_tool_key;
	};
}
#endif
