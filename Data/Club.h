#ifndef DATA_CLUB_H
#define DATA_CLUB_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Club : public Weapon {
	public:
		virtual ~Club();

		static void init();
		static const std::map<const std::string, const Club*>& clubs();

	protected:
		Club(const std::string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Club* club);

	private:
		static std::map<const std::string, const Club*> _clubs;

		static void create(const std::string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, bool one_handed, const int& alignment);
	};
}
#endif
