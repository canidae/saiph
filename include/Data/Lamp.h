#ifndef DATA_LAMP_H
#define	DATA_LAMP_H

#include <map>
#include <string>
#include "Data/Tool.h"

namespace data {

	class Lamp : public Tool {
	public:
		virtual ~Lamp();

		static void init();
		static const std::map<const std::string, const Lamp*>& lamps();

	protected:
		Lamp(const std::string& name, int cost, int weight, int material, unsigned long long properties);

		static void addToMap(const std::string& name, const Lamp* lamp);

	private:
		static std::map<const std::string, const Lamp*> _lamps;

		static void create(const std::string& name, int cost, int weight, int material, unsigned long long properties);
	};
}
#endif
