#include <iostream>
#include <map>
#include <array>
#include <fstream>

class request_answerer {
    std::array<std::map<size_t, size_t>, 2> info;
    void integrate(size_t map_number);
public:
    void add_order(std::array<size_t, 3> order);
    void read_orders(std::istream& input);
    size_t request(std::array<size_t, 3> request_info);
    void print_answers(std::istream& input, std::ostream& output);
};

void request_answerer::add_order(std::array<size_t, 3> order) {
    if (info[0].find(order[0]) == info[0].end()) {
        info[0][order[0]] = order[2];
    }
    else {
        info[0][order[0]] += order[2];
    }
    if (info[1].find(order[1]) == info[1].end()) {
        info[1][order[1]] = order[1] - order[0];
    }
    else {
        info[1][order[1]] += order[1] - order[0];
    }
}

size_t request_answerer::request(std::array<size_t, 3> request_info) {
    size_t type = request_info[2] - 1;
    auto start = info[type].equal_range(request_info[0]);
    if (start.first == info[type].end()) {
        //std::cout << "nullstart";
        return 0;
    }
    auto end = info[type].equal_range(request_info[1]);
    if (end.second == info[type].begin()) {
        //std::cout << "nullend";
        return 0;
    }
    if (end.first == end.second) {
        //std::cout << "nonequal";
        end.first--;
    }
    if (start.first == info[type].begin()) {
        return end.first->second;
    }
    start.first--;
    //size_t answer = end.second->second - start.second->second;
    //std::cout << end.first->second << "-" << start.first->second << "=";
    return end.first->second - start.first->second;
}

void request_answerer::integrate(size_t map_number) {
    size_t sum = 0;
    for (auto pos = info[map_number].begin(); pos != info[map_number].end(); pos++) {
        sum += pos->second;
        pos->second = sum;
        //std::cout << "[" << pos->first << ", " << pos->second << "] ";
    }
}

void request_answerer::read_orders(std::istream& input) {
    size_t orders;
    std::array<size_t, 3> one;
    input >> orders;
    for (size_t i = 0; i < orders; ++i) {
        input >> one[0] >> one[1] >> one[2];
        add_order(one);
    }
    integrate(0);
    integrate(1);
}

void request_answerer::print_answers(std::istream& input, std::ostream& output) {
    size_t requests;
    std::array<size_t, 3> one;
    input >> requests;
    for (size_t i = 0; i < requests; ++i) {
        input >> one[0] >> one[1] >> one[2];
        output << request(one) << ' ';
    }
}

int main()
{
    std::ifstream input_file("input.txt");
    request_answerer answerer;
    answerer.read_orders(input_file);
    answerer.print_answers(input_file, std::cout);
    input_file.close();
}