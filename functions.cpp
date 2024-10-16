#include "functions.h"
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
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QString>
#include <QDebug>

// Функция для удаления всех разделителей в строке, кроме указанного пользователем в качестве исключения
void remove_delimiters_in_string(std::string& input_string, char delimiter_being_exception)
{
    std::string result_string;

    // Для каждого символа исходной строки
    for (char current_char : input_string)
    {
        // Если текущий символ не является разделителем или текущий символ не равен символу-исключению
        if (!std::isspace(current_char) || current_char == delimiter_being_exception)
        {
            // Добавить текущий символ в результирующую строку
            result_string += current_char;
        }
    }

    // Обновить исходную строку
    input_string = result_string;
}

// Функция для удаления всех вхождений символов в строке, указанных в наборе
void remove_all_occurrences_of_character_in_string(std::string& input_string, std::string characters_to_delete)
{
    // Для каждого символа исходной строки
    for (auto& current_char : characters_to_delete)
    {
        // Удалить все вхождения символов из набора
        input_string.erase(std::remove(input_string.begin(), input_string.end(), current_char), input_string.end());
    }
}

// Функция для удаления части строки, находящейся до первого найденного вхождения указанного символа
void removing_part_of_string_up_to_specified_character(std::string& input_string, const char limit_character)
{
    // Для каждого символа исходной строки
    for (auto current_symbol = input_string.begin(); *current_symbol != limit_character; current_symbol)
    {
        // Удалить часть строки, до указанного текущего символа
        current_symbol = input_string.erase(current_symbol);
    }
}

// Функция для удаления однострочных комментариев в строке
void remove_comments_in_string(std::string& input_string)
{
    // Создать шаблон для поиска комментария
    std::regex commentRegex("//.*");

    // Заменить все подстроки, соответствующие шаблону комментария, на пустые подстроки
    std::string result = std::regex_replace(input_string, commentRegex, "");

    // Обновить исходную строку
    input_string = result;
}

// Функция, приводящая запись графа на языке Dot к удобному для получения данных формату
void bring_graph_record_into_format_convenient_obtaining_data(std::string& temporary_graph_in_Dot)
{
    // Удалить однострочные комментарии из исходной строки
    remove_comments_in_string(temporary_graph_in_Dot);

    // Удалить имя и ключевое слово графа из исходной строки
    removing_part_of_string_up_to_specified_character(temporary_graph_in_Dot, '{');

    // Удалить все точки с запятой и фигурные скобоки из исходной строки
    std::string characters_to_delete = ";{}";
    remove_all_occurrences_of_character_in_string(temporary_graph_in_Dot, characters_to_delete);

    // Удалить все лишние пробелы, кроме символа переноса строки, из исходной строки
    remove_delimiters_in_string(characters_to_delete, '\n');
}

// Функция для парсинга текста из строк с парами числовых значений, разделенных указанным разделителем
void create_map_with_vertices_of_graphand_their_numbers(std::string& text_wit_value_pairs, std::string& separator_between_values, std::vector< std::pair<int, int> >& vector_with_value_pairs)
{
    //Создать строковый поток для исходной строки
    std::stringstream temporary_text_stream(text_wit_value_pairs);

    //Создать временную строку для записи подстрок из потока
    std::string temporary_text;

    // Пока из строкового потока можно получать подстроки
    while (std::getline(temporary_text_stream, temporary_text))
    {
        // Найти позицию указанного разделителя
        size_t separator_position = temporary_text.find(separator_between_values);

        //Найти позицию первого вхождения символа, являющегося числом
        size_t num_position = temporary_text.find_first_of("0123456789");

        //Если позиция разделителя найдена
        if (separator_position != std::string::npos)
        {
            // Считать значение до найденного разделителя из временной строки
            int value1 = std::stoul(temporary_text.substr(0, separator_position));

            // Считать значение после найденного разделителя из временной строки
            int value2 = std::stoul(temporary_text.substr(separator_position + separator_between_values.size()));

            //Записать найденную пару значений в список с парами значений
            vector_with_value_pairs.emplace_back(value1, value2);
        }

        // Иначе если позиция первого заначения найдена
        else if (num_position != std::string::npos)
        {
            // Считать найденное значение из временной строки
            int value1 = std::stoul(temporary_text.substr(num_position));

            //Записать найденное значение в список с парами значений, считая, что второе значение осутствует
            vector_with_value_pairs.emplace_back(value1, 0);
        }
    }
}

// Функция, для составления словаря с вершинами графа и их порядковыми номерами из списка дуг графа
void fill_map_with_values_and_their_numbers(std::map<int, int>& map_of_graph_vertices_and_their_numbers, std::vector< std::pair<int, int> >& graph_arc_list)
{
    // Считать, что подсчет вершин будет начинаться с единицы
    int current_number = 1;

    // Для каждой дуги из спика с дугами графа
    for (const auto& current_arc : graph_arc_list)
    {
        // Добавить в словарь первую вершину в качестве ключа
        map_of_graph_vertices_and_their_numbers[current_arc.first] = 0;

        // Добавить в словарь вторую вершину в качестве ключа
        map_of_graph_vertices_and_their_numbers[current_arc.second] = 0;
    }

    // Удалить "нулевые" вершины из словаря
    map_of_graph_vertices_and_their_numbers.erase(0);

    // Для каждой вершины из словаря
    for (auto& current_vertex : map_of_graph_vertices_and_their_numbers)
    {
        // Присвоить текущей вершине соответствующий порядковый номер
        current_vertex.second = current_number++;
    }
}

// Функция для заполнения списка смежности номерами вершин графа
void fill_adjacency_list_of_graph_with_vertex_numbers(
    const std::map<int, int>& map_of_graph_vertices_and_their_numbers,
    const std::vector<std::pair<int, int>>& graph_arc_list,
    std::vector<std::list<int>>& graph_adjacency_list)
{

    // Найти размер словаря с вершинами и их порядковыми номерами
    int size = map_of_graph_vertices_and_their_numbers.size();

    // Ограничить размер создаваемого списка смежности по размеру словаря (по количеству вершин графа)
    graph_adjacency_list.resize(size);

    // Для каждой дуги из списка дуг
    for (const auto& current_arc : graph_arc_list)
    {
        // Считать первой вершиной первое значение, задающее дугу
        int vertex1 = current_arc.first;

        // Считать второй вершиной второе значение, задающее дугу
        int vertex2 = current_arc.second;

        // Получить первую вершину с ее номером из словаря
        auto vertex_with_num1 = map_of_graph_vertices_and_their_numbers.find(vertex1);

        // Получить вторую вершину с ее номером из словаря
        auto vertex_with_num2 = map_of_graph_vertices_and_their_numbers.find(vertex2);

        // Если найденные элементы словаря не являются одновременно последними
        if (vertex_with_num1 != map_of_graph_vertices_and_their_numbers.end() && vertex_with_num2 != map_of_graph_vertices_and_their_numbers.end())
        {
            // Получить номер первой вершины
            int num1 = vertex_with_num1->second;

            // Получить значение второй вершины
            int num2 = vertex_with_num2->first;

            // Добавить значение вершины назначения (второй) в список смежности для вершины источника (первой)
            graph_adjacency_list[num1 - 1].push_back(num2);
        }
    }
}

// Функция для замены вершин на их порядковые номера в списках смежности
std::vector< std::list<int> > replace_vertices_with_their_serial_numbers_in_adjacency_lists(std::map<int, int>& map_of_graph_vertices_and_their_numbers, std::vector< std::list<int> > graph_adjacency_list)
{
    // Для каждого списка из списка смежности
    for (size_t current_list_num = 0; current_list_num < graph_adjacency_list.size(); ++current_list_num)
    {
        // Для каждой вершины из текущего списка
        for (auto iter = graph_adjacency_list[current_list_num].begin(); iter != graph_adjacency_list[current_list_num].end(); iter++)
        {
            // Найти текущую вершину в словаре с вершинами и их номерами
            auto current_vertex_in_map = map_of_graph_vertices_and_their_numbers.find(*iter);

            // Заменить текущую вершину из списка номером, который ей соответствует
            *iter = current_vertex_in_map->second;
        }
    }

    return graph_adjacency_list;
}
