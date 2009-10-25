#ifndef DATA_KEY_H
#define DATA_KEY_H

#include <map>
#include <string>
#include "Tool.h"

namespace data {

	class Key : public Tool {
	public:
		virtual ~Key();

		static void init();
		static const std::map<const std::string, const Key*>& keys();

	protected:
		Key(const std::string& name, const int& cost, const int& weight, const int& material, const unsigned long long& properties);

		static void addToMap(const std::string& name, const Key* key);

	private:
		static std::map<const std::string, const Key*> _keys;

		static void create(const std::string& name, const int& cost, const int& weight, const int& material, const unsigned long long& properties);
	};
}
#endif
