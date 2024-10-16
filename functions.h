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

// Функция для заполнения списка смежности номерами вершин графа
void fill_adjacency_list_of_graph_with_vertex_numbers(
    const std::map<int, int>& map_of_graph_vertices_and_their_numbers,
    const std::vector<std::pair<int, int>>& graph_arc_list,
    std::vector<std::list<int>>& graph_adjacency_list);

// Функция для замены вершин на их порядковые номера в списках смежности
std::vector< std::list<int> > replace_vertices_with_their_serial_numbers_in_adjacency_lists(std::map<int, int>& map_of_graph_vertices_and_their_numbers, std::vector< std::list<int> > graph_adjacency_list);

// Функция для замены номеров вершин на соответствующие вершины в списках смежности
std::vector< std::list<int> > replace_vertex_numbers_with_corresponding_vertices_in_adjacency_lists(std::map<int, int>& map_of_graph_vertices_and_their_numbers, std::vector< std::list<int> > graph_adjacency_list_with_vertex_numbers);

// Функция, выполняющая покраску для связей из соответствующих циклов в указанном графе
void perform_coloring_for_vertices_from_the_corresponding_cycles_in_graph(std::string& graph_in_Dot, const std::vector<std::list<int>>& list_with_simple_cycles);

// Функция для удаления всех символов пропусков, кроме символа переноса строки, не затрагивая однострочные комментарии
void remove_whitespace_except_new_line(std::string& graph_in_Dot);

// Функция для составления списка связей, которые повторяются в нескольких циклах
std::set< std::pair<int, int> > find_repeated_arc_in_cycles(const std::vector< std::list<int> >& list_with_simple_cycles);

// Функция для поиска конца подстроки, которая не является однострочным комментарием
size_t searching_for_substring_that_is_not_comment(const std::string& str, const std::string& substr, size_t start_pos);

#endif // FUNCTIONS_H
