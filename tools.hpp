#pragma once

#include "config.hpp"

class Tools {
    static void skip_csv_header(ifstream &file);
    static string if_there_is_carriage_value_at_end_of_line_delete_it(string line);

public:
    static vector<vector<string>> read_csv(string file_directory);
    static vector<string> split_words(string elemetns, char divider);
    static string get_file_directory_week(int week_number);
    
    static vector<string> split_two_word(string elements, char divider);

    static void check_question_mark(istringstream &input);

    static string get_input_word(istringstream &input, string input_name);

    static float round(float num);
};