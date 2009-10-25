#ifndef DATA_QUARTERSTAFF_H
#define DATA_QUARTERSTAFF_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Quarterstaff : public Weapon {
	public:
		virtual ~Quarterstaff();

		static void init();
		static const std::map<const std::string, const Quarterstaff*>& quarterstaves();

	protected:
		Quarterstaff(const std::string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Quarterstaff* quarterstaff);

	private:
		static std::map<const std::string, const Quarterstaff*> _quarterstaves;

		static void create(const std::string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, bool one_handed, const int& alignment);
	};
}
#endif
