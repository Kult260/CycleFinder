#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <regex>
#include <set>
#include <map>
#include <sstream>
#include <utility>
#include <list>

// Функция, приводящая запись графа на языке Dot к удобному для получения данных формату
void bring_graph_record_into_format_convenient_obtaining_data(std::string& temporary_graph_in_Dot);

// Функция для удаления однострочных комментариев в строке
void remove_comments_in_string(std::string& input_string);

// Функция для удаления части строки, находящейся до первого найденного вхождения указанного символа
void removing_part_of_string_up_to_specified_character(std::string& input_string, const char limit_character);

// Функция для удаления всех вхождений символов в строке, указанных в наборе
void remove_all_occurrences_of_character_in_string(std::string& input_string, std::string characters_to_delete);

// Функция для удаления всех разделителей в строке, кроме указанного пользователем в качестве исключения
void remove_delimiters_in_string(std::string& input_string, char delimiter_being_exception);

// Функция для парсинга текста из строк с парами числовых значений, разделенных указанным разделителем
void create_map_with_vertices_of_graphand_their_numbers(std::string& text_wit_value_pairs, std::string& separator_between_values, std::vector< std::pair<int, int> >& vector_with_value_pairs);

// Функция, для составления словаря с вершинами графа и их порядковыми номерами из списка дуг графа
void fill_map_with_values_and_their_numbers(std::map<int, int>& map_of_graph_vertices_and_their_numbers, std::vector< std::pair<int, int> >& graph_arc_list);

#endif // FUNCTIONS_H
