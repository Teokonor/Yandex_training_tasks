#include <iostream>
#include <map>
#include <list>
#include <array>
#include <vector>
#include <fstream>
#include <set>

struct recipe {
    size_t name;
    //bool valid;
    std::map<size_t, size_t> compound;
    recipe(size_t _name, std::vector<size_t> ingredients);
};

recipe::recipe(size_t _name, std::vector<size_t> ingredients) : name(_name)/*, valid(true)*/ {
    for (size_t one : ingredients) {
        /*if (one == name) {
            valid = false;
            break;
        }*/
        auto elem = compound.find(one);
        if (elem == compound.end()) {
            compound[one] = 1;
        }
        else {
            elem->second++;
        }
    }
    if (compound.find(1) == compound.end()) {
        compound[1] = 0;
    }
    if (compound.find(2) == compound.end()) {
        compound[2] = 0;
    }
}

class recipe_list {
    //std::map<size_t, std::array<size_t, 2>> valid_potions;
    std::list<recipe> potions;
    std::vector<std::array<size_t, 2>> all_potions;
    std::vector<std::set<size_t>> include;
    //std::list<size_t> invalid_potions;
    bool possibly(std::array<size_t, 3> request);
public:
    void read_recipes(std::istream& input);
    void analyse_recipes();
    void print_answers_to_requests(std::istream& input, std::ostream& output);
};

void recipe_list::read_recipes(std::istream& input) {
    size_t potions_quantity, ingredients_quantity;
    input >> potions_quantity;
    all_potions.resize(potions_quantity - 2);
    include.resize(potions_quantity - 2);
    for (size_t i = 0; i < potions_quantity - 2; ++i) {
        all_potions[i][0] = all_potions[i][1] = 0;
    }
    for (size_t num = 3; num <= potions_quantity; ++num) {
        input >> ingredients_quantity;
        std::vector<size_t> ingredients(ingredients_quantity);
        for (size_t i = 0; i < ingredients_quantity; ++i) {
            input >> ingredients[i];
            if (ingredients[i] > 2 && include[ingredients[i] - 3].count(num) == 0) {
                include[ingredients[i] - 3].insert(num);
            }
        }
        recipe new_potion(num, ingredients);
        //if (new_potion.compound.size() == 2) {
        //    all_potions[num - 3][0] = new_potion.compound.at(1);
        //    all_potions[num - 3][1] = new_potion.compound.at(2);
        //}
        //else {
        //    potions.push_back(new_potion);
        //}
        potions.push_back(new_potion);
    }
}

void recipe_list::analyse_recipes() {
    size_t changes;
    while (true) {
        changes = 0;
        for (auto pos = potions.begin(); pos != potions.end();) {
            auto current_recipe = pos;
            pos++;
            if (current_recipe->compound.size() == 2) {
                std::array<size_t, 2> ingredientsAB = { current_recipe->compound[1], current_recipe->compound[2] };
                all_potions[current_recipe->name - 3] = ingredientsAB;
                auto setpos = include[current_recipe->name - 3].begin();
                auto setend = include[current_recipe->name - 3].end();
                if (setpos != setend) {
                    for (recipe& changing_recipe : potions) {
                        if (changing_recipe.name == *setpos) {
                            changing_recipe.compound.at(1) += changing_recipe.compound.at(current_recipe->name) * all_potions[current_recipe->name - 3][0];
                            changing_recipe.compound.at(2) += changing_recipe.compound.at(current_recipe->name) * all_potions[current_recipe->name - 3][1];
                            changing_recipe.compound.erase(changing_recipe.compound.find(current_recipe->name));
                            setpos++;
                        }
                        if (setpos == setend) {
                            break;
                        }
                    }
                }
                //for (setpos; setpos != setend; setpos++) {
                //    //recipe& changing_recipe = potions.;
                //}
                potions.erase(current_recipe);
                changes++;
            }
        }
        if (changes == 0) {
            break;
        }
    }
}

//void recipe_list::analyse_recipes() {
//    size_t changes;
//    while (true) {
//        changes = 0;
//        for (auto pos = potions.begin(); pos != potions.end();) {
//            auto current_recipe = pos;
//            pos++;
//            auto ingr_A = current_recipe->compound.begin(), current_ingr = ingr_A, ingr_B = ingr_A;
//            ingr_B++;
//            current_ingr++; current_ingr++;
//            for (current_ingr; current_ingr != current_recipe->compound.end();) {
//                auto next_ingr = current_ingr;
//                next_ingr++;
//                std::array<size_t, 2> founded_valid_component = all_potions[current_ingr->first - 3];
//                if (founded_valid_component[0] != 0 || founded_valid_component[1] != 0) {
//                    ingr_A->second += founded_valid_component[0] * current_ingr->second;
//                    ingr_B->second += founded_valid_component[1] * current_ingr->second;
//                    current_recipe->compound.erase(current_ingr);
//                }
//                current_ingr = next_ingr;
//            }
//            if (current_recipe->compound.size() == 2) {
//                std::array<size_t, 2> ingredientsAB = { current_recipe->compound[1], current_recipe->compound[2] };
//                all_potions[current_recipe->name - 3] = ingredientsAB;
//                potions.erase(current_recipe);
//                changes++;
//            }
//        }
//        if (changes == 0) {
//            break;
//        }
//    }
//}

bool recipe_list::possibly(std::array<size_t, 3> request) {
    std::array<size_t, 2> potion = all_potions[request[2] - 3];
    if (potion[0] == 0 && potion[1] == 0) {
        return false;
    }
    if (potion[0] <= request[0] && potion[1] <= request[1]) {
        return true;
    }
    return false;
}

void recipe_list::print_answers_to_requests(std::istream& input, std::ostream& output) {
    size_t quantity;
    input >> quantity;
    std::array<size_t, 3> request;
    for (size_t num = 0; num < quantity; ++num) {
        for (size_t i = 0; i < 3; ++i) {
            input >> request[i];
        }
        output << /*"answer" << */possibly(request)/* << " "*/;
    }
}

int main()
{
    std::ifstream input_file("input.txt");
    recipe_list my_list;
    my_list.read_recipes(input_file);
    my_list.analyse_recipes();
    my_list.print_answers_to_requests(input_file, std::cout);
    input_file.close();
}