#ifndef _TOOLDATA_H
#define	_TOOLDATA_H

#include <map>
#include <string>
#include "Item.h"
#include "../Globals.h"

namespace data {
	class Tool : public Item {
	public:
		static std::map<std::string, Tool *> tools;

		Tool(const std::string &name, int cost, int weight, int material, unsigned long long properties);
		virtual ~Tool() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Tool *tool);
	};
}
#endif
