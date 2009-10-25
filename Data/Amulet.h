#ifndef DATA_AMULET_H
#define DATA_AMULET_H

#include <map>
#include <string>
#include "Item.h"

namespace data {

	class Amulet : public Item {
	public:
		virtual ~Amulet();

		static void init();
		static const std::map<const std::string, const Amulet*>& amulets();
		const bool& conferEffect() const;

	protected:
		Amulet(const std::string& name, const int& cost, const int& weight, const int& material, const unsigned long long& properties, const bool& confer_effect);

		static void addToMap(const std::string& name, const Amulet* amulet);

	private:
		static std::map<const std::string, const Amulet*> _amulets;
		const bool _confer_effect;

		static void create(const std::string& name, const int& cost, const int& weight, const int& material, const unsigned long long& properties, const bool& confer_effect);
	};
}
#endif
