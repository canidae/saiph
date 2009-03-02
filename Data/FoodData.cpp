#include "FoodData.h"

using namespace std;

/* initialize static variables */
map<string, FoodData *> FoodData::foods;

FoodData::FoodData(const std::string &name, int cost, int weight, int material, unsigned long long properties, int nutrition, int eat_time, int eat_effects, int resistance_confer_probability) : ItemData(name, cost, weight, FOOD, material, properties), nutrition(nutrition), eat_time(eat_time), eat_effects(eat_effects), resistance_confer_probability(resistance_confer_probability) {
}

void FoodData::addToMap(const std::string &name, FoodData *food) {
	FoodData::foods[name] = food;
	ItemData::addToMap(name, food);
}

void FoodData::create(const std::string &name, int cost, int weight, int material, unsigned long long properties, int nutrition, int eat_time, int eat_effects, int resistance_confer_probability) {
	addToMap(name, new FoodData(name, cost, weight, material, properties, nutrition, eat_time, eat_effects, resistance_confer_probability));
	if (eat_time > 1 && name != "tin" && name != "tin of spinach") {
		string partly_eaten = "partly eaten ";
		partly_eaten.append(name);
		addToMap(partly_eaten, new FoodData(partly_eaten, 0, weight / 2, material, properties, nutrition / 2, eat_time / 2, eat_effects, resistance_confer_probability));
	}
}

void FoodData::init() {
	create("meatball", 5, 1, MATERIAL_FLESH, 0, 5, 1, 0, 0);
	create("meat ring", 5, 1, MATERIAL_FLESH, 0, 5, 1, 0, 0);
	create("meat stick", 5, 1, MATERIAL_FLESH, 0, 5, 1, 0, 0);
	create("egg", 9, 15, MATERIAL_FLESH, 0, 80, 1, EAT_EFFECT_VEGAN, 0);
	create("tripe ration", 15, 10, MATERIAL_FLESH, 0, 200, 2, 0, 0);
	create("huge chunk of meat", 105, 400, MATERIAL_FLESH, 0, 2000, 20 , 0, 0);
	create("kelp frond", 6, 1, MATERIAL_VEGGY, 0, 30, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("eucalyptus leaf", 6, 1, MATERIAL_VEGGY, 0, 30, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("clove of garlic", 7, 1, MATERIAL_VEGGY, 0, 40, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("sprig of wolfsbane", 7, 1, MATERIAL_VEGGY, 0, 40, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("apple", 7, 2, MATERIAL_VEGGY, 0, 50, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("carrot", 7, 2, MATERIAL_VEGGY, 0, 50, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("pear", 7, 2, MATERIAL_VEGGY, 0, 50, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("banana", 9, 2, MATERIAL_VEGGY, 0, 80, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("orange", 9, 2, MATERIAL_VEGGY, 0, 80, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("melon", 10, 5, MATERIAL_VEGGY, 0, 100, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("byte", 17, 5, MATERIAL_VEGGY, 0, 250, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("fortune cookie", 7, 1, MATERIAL_VEGGY, 0, 40, 1, EAT_EFFECT_VEGETARIAN, 0);
	create("candy bar", 10, 2, MATERIAL_VEGGY, 0, 100, 1, EAT_EFFECT_VEGETARIAN, 0);
	create("cream pie", 10, 10, MATERIAL_VEGGY, 0, 100, 1, EAT_EFFECT_VEGETARIAN, 0);
	create("lump of royal jelly", 15, 2, MATERIAL_VEGGY, 0, 200, 1, EAT_EFFECT_VEGETARIAN | EAT_EFFECT_STRENGTH, 0);
	create("pancake", 15, 2, MATERIAL_VEGGY, 0, 200, 2, EAT_EFFECT_VEGETARIAN, 0);
	create("C-ration", 20, 10, MATERIAL_VEGGY, 0, 300, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("K-ration", 25, 10, MATERIAL_VEGGY, 0, 400, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("cram ration", 35, 15, MATERIAL_VEGGY, 0, 600, 3, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("food ration", 45, 20, MATERIAL_VEGGY, 0, 800, 5, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("gunyoki", 45, 20, MATERIAL_VEGGY, 0, 800, 5, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("lembas wafer", 45, 5, MATERIAL_VEGGY, 0, 800, 2, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN, 0);
	create("tin", 5, 10, MATERIAL_VEGGY, 0, 50, 50, 0, 0);
	create("tin of spinach", 5, 10, MATERIAL_VEGGY, 0, 50, 50, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN | EAT_EFFECT_STRENGTH, 0);
}
