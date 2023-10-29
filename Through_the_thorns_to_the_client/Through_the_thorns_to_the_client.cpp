#include <iostream>
#include <array>
#include <map>
#include <fstream>
#include <sstream>

const size_t ASCII_0 = 48;

struct log_record {
    size_t id;
    size_t time;
    char event_type;
};

class rockets_worktime final {
    std::map <size_t, long> body;
public:
    void add(log_record new_rec);
    void print_rockets_time(std::ostream& output);
};

void rockets_worktime::add(log_record new_rec) {
    if (body.find(new_rec.id) == body.end()) {
        body[new_rec.id] = 0;
    }
    if (new_rec.event_type == 'C' || new_rec.event_type == 'S') {
        body[new_rec.id] += new_rec.time;
        return;
    }
    if (new_rec.event_type == 'A') {
        body[new_rec.id] -= new_rec.time;
        return;
    }
}

void rockets_worktime::print_rockets_time(std::ostream& output) {
    for (std::pair<size_t, long> time : body) {
        if (time.first != 0) {
            output << time.second << ' ';
        }
    }
}

size_t convert_day_hour_min_to_time(std::array<size_t, 3> day_hour_min) {
    return day_hour_min[0] * 1440 + day_hour_min[1] * 60 + day_hour_min[2];
}

class Parser {
    std::istream& input;
public:
    Parser(std::istream& input_stream) : input(input_stream) {}
    bool all_readed() {
        return input.eof();
    }
    log_record read_one();
};

size_t stost(std::string str) {
    size_t num = 0;
    for (char sym : str) {
        num *= 10;
        num += (size_t)sym - ASCII_0;
    }
    return num;
}

log_record Parser::read_one() {
    std::string line;
    log_record new_rec;
    std::getline(input, line);
    std::stringstream line_str(line);
    std::array<size_t, 3> day_hour_min;
    std::string word;
    for (size_t i = 0; i < 3; ++i) {
        std::getline(line_str, word, ' ');
        day_hour_min[i] = stost(word);
    }
    new_rec.time = convert_day_hour_min_to_time(day_hour_min);
    std::getline(line_str, word, ' ');
    new_rec.id = stost(word);
    std::getline(line_str, word, ' ');
    new_rec.event_type = word[0];
    return new_rec;
}

int main()
{
    std::ifstream input_file("input.txt");
    std::string records;
    std::getline(input_file, records);
    Parser pars(input_file);
    rockets_worktime time_counter;
    while (!pars.all_readed()) {
        time_counter.add(pars.read_one());
    }
    input_file.close();
    time_counter.print_rockets_time(std::cout);
    return 0;
}
