#ifndef DATA_LANCE_H
#define DATA_LANCE_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Lance : public Weapon {
	public:
		virtual ~Lance();

		static void init();
		static const std::map<const std::string, const Lance*>& lances();

	protected:
		Lance(const std::string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Lance* lance);

	private:
		static std::map<const std::string, const Lance*> _lances;

		static void create(const std::string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, bool one_handed, const int& alignment);
	};
}
#endif
