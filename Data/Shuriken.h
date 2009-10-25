#ifndef DATA_SHURIKEN_H
#define DATA_SHURIKEN_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Shuriken : public Weapon {
	public:
		virtual ~Shuriken();

		static void init();
		static const std::map<const std::string, const Shuriken*>& shurikens();

	protected:
		Shuriken(const std::string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Shuriken* shuriken);

	private:
		static std::map<const std::string, const Shuriken*> _shurikens;

		static void create(const std::string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, bool one_handed, const int& alignment);
	};
}
#endif
