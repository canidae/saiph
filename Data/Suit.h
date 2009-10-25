#ifndef DATA_SUIT_H
#define DATA_SUIT_H

#include <map>
#include <string>
#include "Armor.h"

namespace data {

	class Suit : public Armor {
	public:
		virtual ~Suit();

		static void init();
		static const std::map<const std::string, const Suit*>& suits();

	protected:
		Suit(const std::string& name, const int& cost, const int& weight, const int& material, const int& ac, const int& mc, const unsigned long long& properties);

		static void addToMap(const std::string& name, const Suit* suit);

	private:
		static std::map<const std::string, const Suit*> _suits;

		static void create(const std::string& name, const int& cost, const int& weight, const int& material, const int& ac, const int& mc, const unsigned long long& properties);
	};
}
#endif
