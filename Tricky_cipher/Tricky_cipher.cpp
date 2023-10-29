#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <map>
#include <fstream>
//#include <stdexcept>

const size_t ASCII_A = 65, ASCII_a = 97, ASCII_0 = 48, alphabet_size = 26;

struct Person {
    std::array<std::string, 3> _name;
    std::array<size_t, 3> _birthday;
    Person(std::array<std::string, 3> name, std::array<size_t, 3> birthday) : _name(name), _birthday(birthday) {}
};

class Parser {
    std::istream& input;
public:
    Parser(std::istream& input_stream) : input(input_stream) {}
    bool all_readed() {
        return input.eof();
    }
    Person read_one();
};

Person Parser::read_one() {
    std::string line;
    std::array<std::string, 3> name;
    std::array<size_t, 3> birthday;
    std::getline(input, line);
    std::stringstream line_str(line);
    for (size_t i = 0; i < 3; ++i) {
        std::getline(line_str, name[i], ',');
    }
    std::string word;
    for (size_t i = 0; i < 3; ++i) {
        std::getline(line_str, word, ',');
        int num = std::stoi(word);
        birthday[i] = num;
    }
    Person pers(name, birthday);
    return pers;
}

size_t different_letters(std::array<std::string, 3>& name) {
    std::map<char, bool> letter_found;
    for (size_t i = 0; i < 3; ++i) {
        for (char letter : name[i]) {
            auto iter = letter_found.find(letter);
            if (iter == letter_found.end() || !iter->second) {
                letter_found[letter] = true;
            }
        }
    }
    return letter_found.size();
}

size_t number_in_alphabet(char letter) {
    size_t number = (size_t)letter, first_letter = ASCII_a;
    if (number < ASCII_A + alphabet_size) {
        first_letter = ASCII_A;
    }
    return number - first_letter + 1;
}

char convert_to_16(size_t number) {
    if (number >= 10) {
        return (char)(number - 10 + ASCII_A);
    }
    return (char)(ASCII_0 + number);
}

std::string encrypt(Person pers) {
    size_t diff_let = different_letters(pers._name);
    size_t bir_sum = pers._birthday[0] / 10 + pers._birthday[0] % 10 + pers._birthday[1] / 10 + pers._birthday[1] % 10;
    size_t first_in_family_name = number_in_alphabet(pers._name[0][0]);
    size_t sum = diff_let + bir_sum * 64 + first_in_family_name * 256;
    std::string code("000");
    for (int i = 2; i >= 0; --i) {
        code[i] = convert_to_16(sum % 16);
        sum /= 16;
    }
    return code;
}

int main()
{
    std::string persons;
    std::ifstream input_file("input.txt");
    std::getline(input_file, persons);
    int quan = std::stoi(persons);
    Parser pars(input_file);
    for (int i = 0; i < quan; ++i) {
        std::cout << encrypt(pars.read_one()) << ' ';
    }
    //while (!pars.all_readed()) {
    //    std::cout << encrypt(pars.read_one()) << ' ';
    //}
    input_file.close();
    return 0;
}