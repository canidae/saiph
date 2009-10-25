#ifndef DATA_HAMMER_H
#define DATA_HAMMER_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Hammer : public Weapon {
	public:
		virtual ~Hammer();

		static void init();
		static const std::map<const std::string, const Hammer*>& hammers();

	protected:
		Hammer(const std::string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Hammer* hammer);

	private:
		static std::map<const std::string, const Hammer*> _hammers;

		static void create(const std::string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, bool one_handed, const int& alignment);
	};
}
#endif
