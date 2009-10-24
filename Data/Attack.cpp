#include "Attack.h"

using namespace data;

/* constructors/destructor */
Attack::Attack(const int& type, const int& damage_type, const int& dices, const int& sides) : _type(type), _damage_type(damage_type), _dices(dices), _sides(sides), _min_damage(_dices), _avg_damage((_dices * _sides + _dices) / 2), _max_damage(_dices * _sides) {
}

Attack::~Attack() {
}

/* public methods */
const int& Attack::type() const {
	return _type;
}

const int& Attack::damageType() const {
	return _damage_type;
}

const int& Attack::dices() const {
	return _dices;
}

const int& Attack::sides() const {
	return _sides;
}

const int& Attack::minDamage() const {
	return _min_damage;
}

const int& Attack::avgDamage() const {
	return _avg_damage;
}

const int& Attack::maxDamage() const {
	return _max_damage;
}

