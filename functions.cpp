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
