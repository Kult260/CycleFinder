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
