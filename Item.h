#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
public:
	Item(const std::string& text, const int& want = 0);
	Item();

	const std::string& name() const;
	const std::string& name(const std::string &name);
	const int& count() const;
	const int& count(const int& count);
	const int& beatitude() const;
	const int& beatitude(const int& beatitude);
	const bool& greased() const;
	const bool& greased(const bool& greased);
	const bool& fixed() const;
	const bool& fixed(const bool& fixed);
	const int& damage() const;
	const int& damage(const int& damage);
	const bool& unknownEnchantment() const;
	const bool& unknownEnchantment(const bool& unknown_enchantment);
	const int& enchantment() const;
	const int& enchantment(const int& enchantment);
	const std::string& additional() const;
	const std::string& additional(const std::string& additional);
	const int& want() const;
	const int& want(const int& want);

	bool operator==(const Item& i);
	bool operator!=(const Item& i);

private:
	std::string _name;
	int _count;
	int _beatitude;
	bool _greased;
	bool _fixed;
	int _damage;
	bool _unknown_enchantment;
	int _enchantment;
	std::string _additional;
	int _want; // this is used by analyzers and event::WantItem to determine how many we want of this item when looting/dropping
};

std::ostream & operator<<(std::ostream& out, const Item& item);
#endif
