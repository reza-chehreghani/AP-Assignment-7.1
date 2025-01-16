#include "tools.hpp"

vector<vector<string>> Tools::read_csv(string file_directory) {
    ifstream file(file_directory);
    vector<vector<string>> elements;

    skip_csv_header(file);

    string line;
    while (getline(file, line)) {
        line = if_there_is_carriage_value_at_end_of_line_delete_it(line);
        elements.push_back(split_words(line, ','));
    }

    file.close();
    return elements;
}

string Tools::if_there_is_carriage_value_at_end_of_line_delete_it(string line) {
    if (line[line.length() - 1] == '\r')
        return line.substr(0, line.length() - 1);
    return line;
}

vector<string> Tools::split_words(string line_, char divider) {
    istringstream line(line_);
    vector<string> elements;
    
    string element;
    while (getline(line, element, divider))
        elements.push_back(element);
    
    return elements;
}

void Tools::skip_csv_header(ifstream &file) {
    string line;
    getline(file, line);
}

string Tools::get_file_directory_week(int week_number) {
    return WEEKS_STATS_FILE_DIRECTORY + to_string(week_number + 1) + ".csv";
}

vector<string> Tools::split_two_word(string elements, char divider) {
    size_t pos_delimiter = elements.find(divider);
    return vector<string>{elements.substr(0, pos_delimiter), elements.substr(pos_delimiter + 1)};
}

void Tools::check_question_mark(istringstream &input) {
    char question_mark;
    input >> question_mark;
    if (question_mark != DELEMETER_BETWEEN_ORDER_AND_ARGUMENT)
        throw BadRequest("there is no question mark");
}

string Tools::get_input_word(istringstream &input, string input_name) {
    string INPUT_WORD, input_word;

    if (!(input >> INPUT_WORD >> input_word))
        throw BadRequest("invalid argument");

    if (INPUT_WORD != input_name)
        throw BadRequest("wrong arqument");

    return input_word;
}

float Tools::round(float num) {
    // return static_cast<int>(num * pow(10.f, NUM_OF_DECIMAL_PLACE)) / pow(10.f, NUM_OF_DECIMAL_PLACE);
    ostringstream num_ss;
    num_ss << fixed << setprecision(NUM_OF_DECIMAL_PLACE) << num;
    return stof(num_ss.str());
}