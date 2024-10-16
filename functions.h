#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structs.h"
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

#endif // FUNCTIONS_H
