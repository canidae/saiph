#ifndef DATA_LAMP_H
#define	DATA_LAMP_H

#include <map>
#include <string>
#include "Tool.h"

namespace data {

	class Lamp : public Tool {
	public:
		virtual ~Lamp();

		static void init();
		static const std::map<const std::string, const Lamp*>& lamps();

	protected:
		Lamp(const std::string& name, const int& cost, const int& weight, const int& material, const unsigned long long& properties);

		static void addToMap(const std::string& name, const Lamp* lamp);

	private:
		static std::map<const std::string, const Lamp*> _lamps;

		static void create(const std::string& name, const int& cost, const int& weight, const int& material, const unsigned long long& properties);
	};
}
#endif
