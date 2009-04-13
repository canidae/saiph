#include "Food.h"
#include "Corpse.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Food *> Food::foods;

Food::Food(const string &name, int cost, int weight, int material, unsigned long long properties, int nutrition, int eat_time, int eat_effects) : Item(name, cost, weight, FOOD, material, properties), nutrition(nutrition), eat_time(eat_time), eat_effects(eat_effects) {
}

void Food::addToMap(const string &name, Food *food) {
	Food::foods[name] = food;
	Item::addToMap(name, food);
}

void Food::create(const string &name, int cost, int weight, int material, unsigned long long properties, int nutrition, int eat_time, int eat_effects) {
	addToMap(name, new Food(name, cost, weight, material, properties, nutrition, eat_time, eat_effects));
	if (eat_time > 1 && name != "tin" && name != "tin of spinach") {
		string partly_eaten = "partly eaten ";
		partly_eaten.append(name);
		addToMap(partly_eaten, new Food(partly_eaten, 0, weight / 2, material, properties, nutrition / 2, eat_time / 2, eat_effects));
	}
}

void Food::init() {
	create("meatball", 5, 1, MATERIAL_FLESH, 0, 5, 1, 0);
	create("meat ring", 5, 1, MATERIAL_FLESH, 0, 5, 1, 0);
	create("meat stick", 5, 1, MATERIAL_FLESH, 0, 5, 1, 0);
	create("egg", 9, 15, MATERIAL_FLESH, 0, 80, 1, EAT_EFFECT_VEGAN);
	create("tripe ration", 15, 10, MATERIAL_FLESH, 0, 200, 2, 0);
	create("huge chunk of meat", 105, 400, MATERIAL_FLESH, 0, 2000, 20 , 0);
	create("kelp frond", 6, 1, MATERIAL_VEGGY, 0, 30, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("eucalyptus leaf", 6, 1, MATERIAL_VEGGY, 0, 30, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("clove of garlic", 7, 1, MATERIAL_VEGGY, 0, 40, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("sprig of wolfsbane", 7, 1, MATERIAL_VEGGY, 0, 40, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("apple", 7, 2, MATERIAL_VEGGY, 0, 50, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("carrot", 7, 2, MATERIAL_VEGGY, 0, 50, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("pear", 7, 2, MATERIAL_VEGGY, 0, 50, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("banana", 9, 2, MATERIAL_VEGGY, 0, 80, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("orange", 9, 2, MATERIAL_VEGGY, 0, 80, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("melon", 10, 5, MATERIAL_VEGGY, 0, 100, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("byte", 17, 5, MATERIAL_VEGGY, 0, 250, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("fortune cookie", 7, 1, MATERIAL_VEGGY, 0, 40, 1, EAT_EFFECT_VEGETARIAN);
	create("candy bar", 10, 2, MATERIAL_VEGGY, 0, 100, 1, EAT_EFFECT_VEGETARIAN);
	create("cream pie", 10, 10, MATERIAL_VEGGY, 0, 100, 1, EAT_EFFECT_VEGETARIAN);
	create("lump of royal jelly", 15, 2, MATERIAL_VEGGY, 0, 200, 1, EAT_EFFECT_VEGETARIAN | EAT_EFFECT_STRENGTH);
	create("pancake", 15, 2, MATERIAL_VEGGY, 0, 200, 2, EAT_EFFECT_VEGETARIAN);
	create("C-ration", 20, 10, MATERIAL_VEGGY, 0, 300, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("K-ration", 25, 10, MATERIAL_VEGGY, 0, 400, 1, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("cram ration", 35, 15, MATERIAL_VEGGY, 0, 600, 3, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("food ration", 45, 20, MATERIAL_VEGGY, 0, 800, 5, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("gunyoki", 45, 20, MATERIAL_VEGGY, 0, 800, 5, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("lembas wafer", 45, 5, MATERIAL_VEGGY, 0, 800, 2, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN);
	create("tin", 5, 10, MATERIAL_VEGGY, 0, 50, 50, 0);
	create("tin of spinach", 5, 10, MATERIAL_VEGGY, 0, 50, 50, EAT_EFFECT_VEGAN | EAT_EFFECT_VEGETARIAN | EAT_EFFECT_STRENGTH);
	create("lichen corpse", 5, 20, MATERIAL_VEGGY, 0, 200, 3, EAT_EFFECT_VEGETARIAN | EAT_EFFECT_VEGAN);
	create("lizard corpse", 5, 10, MATERIAL_FLESH, 0, 40, 3, EAT_EFFECT_CURE_STONING | EAT_EFFECT_REDUCE_STUNNING | EAT_EFFECT_REDUCE_CONFUSION);

	Corpse::init();
}
