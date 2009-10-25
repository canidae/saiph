#ifndef DATA_TOOL_H
#define DATA_TOOL_H

#include <map>
#include <string>
#include "Item.h"
#include "../Globals.h"

namespace data {

	class Tool : public Item {
	public:
		virtual ~Tool();

		static void init();
		static const std::map<const std::string, const Tool*>& tools();

	protected:
		Tool(const std::string& name, const int& cost, const int& weight, const int& material, const unsigned long long& properties);

		static void addToMap(const std::string& name, const Tool* tool);

	private:
		static std::map<const std::string, const Tool*> _tools;
	};
}
#endif
