#include "functions.h"
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
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QString>
#include <QDebug>

// Функция для нахождения всех позиций начала однострочных комментариев
std::vector<size_t> find_comment_positions(const std::string& input_str)
{
    std::vector<size_t> positions;

    //Найти начало однострочного комментария в строке
    size_t pos = input_str.find("//");

    // Пока в исходной строке еще есть однострочные комментарии
    while (pos != std::string::npos)
    {
        // Добавить в список позиций однострочных комментариев новую найденную позицию
        positions.push_back(pos);

        // Найти начало следующего однострочного комментария в еще не проверенной части строки
        pos = input_str.find("//", pos + 2);
    }

    return positions;
}

// Функция для проверки принадлежности определенной части строки к однострочному комментарию
bool is_part_of_single_line_comment(const std::string& input_str, size_t end_pos)
{
    // Найти позиции всех однострочных комментариев, присутствующих в строке
    std::vector<size_t> comment_positions = find_comment_positions(input_str);

    // Считать, что начало подстроки, которую нужно проверить, является ее концом
    size_t start_pos = end_pos;

    // Пока не достигнут символ переноса строки и начальная позиция не является началом исходной строки и комментарием
    while ((input_str[start_pos] != '\n' && input_str[start_pos] != '/') && start_pos != 0)
    {
        //Сместиться по строке левее на один символ
        start_pos--;
    }

    // Для каждого комментария из списка позиций со всеми однострочными комментариями
    for (size_t comment_pos : comment_positions)
    {
        // Найти конец для текущего комментария
        size_t comment_end = input_str.find('\n', comment_pos);

        // Если конец комментария найден
        if (comment_end == std::string::npos)
        {
            // Считать, что позиция конца комментария равна длине исходной строки
            comment_end = input_str.length();
        }

        // Если часть строки, которую требуется проверить, находится в комменарии
        if (start_pos >= comment_pos && end_pos <= comment_end)
        {
            // Считать, что строка принадлежит комментарию
            return true;
        }
    }

    return false;
}

// Функция для поиска конца подстроки, которая не является однострочным комментарием
size_t searching_for_substring_that_is_not_comment(const std::string& input_str, const std::string& substr, size_t start_pos)
{
    size_t begin_substr_pos = input_str.find(substr, start_pos);

    // Находим позицию, на которой кончается подстрока
    size_t end_pos = begin_substr_pos + substr.size();

    // Считаем, что изначально подстрока является частью однострочного комментария
    bool comment_flag = true;

    // Иначе если начало подстроки было найдено
    if (begin_substr_pos != std::string::npos)
    {
        // Проверяем подстроку на принадлежность к однострочному комментарию
        comment_flag = is_part_of_single_line_comment(input_str, end_pos);

        // Если подстрока является частью комментария и ее конец был найден
        if (comment_flag)
        {
            // Продолжить поиск подстроки, которая не является однострочным комментарием в еще непроверенной части текста
            searching_for_substring_that_is_not_comment(input_str, substr, end_pos);
        }

        // Иначе считать конец подстроки найденным
        else
        {
            return end_pos;
        }
    }

    else
    {
        return std::string::npos;
    }
}

// Функция для составления списка связей, которые повторяются в нескольких циклах
std::set< std::pair<int, int> > find_repeated_arc_in_cycles(const std::vector< std::list<int> >& list_with_simple_cycles)
{
    std::map<std::pair<int, int>, int> edge_count; // список с парами и количеством их в различных циклах
    std::set< std::pair<int, int> > repeated_edges; // множество многократно повторяющихся пар

    // Для каждого цикла из списка
    for (const auto& current_cycle : list_with_simple_cycles)
    {
        // Сохранить первый элемент цикла
        auto second_element = current_cycle.begin();
        auto first_element = second_element;

        // Перейти к следующему элементу цикла
        ++second_element;

        // Пока не достигнут конец цикла
        while (second_element != current_cycle.end())
        {
            std::pair<int, int> edge = { *first_element, *second_element }; // пара для текущей связи

            // Считать, что пара один раз встретилась в этом цикле и увеличить счетчик для этой пары
            ++edge_count[edge];

            // Перейти к следующей паре
            first_element = second_element;
            ++second_element;
        }
    }

    // Для каждой пары связей из словаря с парами связей и их количеством в различных циклах
    for (const auto& current_pair : edge_count)
    {
        // Если связь встречается более одного раза в различных циклах
        if (current_pair.second > 1)
        {
            // Добавить связь в множество повторяющихся связей
            repeated_edges.insert(current_pair.first);
        }
    }

    return repeated_edges;
}

// Функция для удаления всех символов пропусков, кроме символа переноса строки, не затрагивая однострочные комментарии в описании графа
void remove_whitespace_except_new_line(std::string& graph_in_Dot)
{
    bool in_comment_flag = false; // Флаг, указывающий, находимся ли мы в однострочном комментарии
    std::string new_graph_in_Dot;     // Строка для хранения результата

    // Для каждого символа входной строки
    for (size_t num_of_current_char = 0; num_of_current_char < graph_in_Dot.size(); ++num_of_current_char)
    {

        // Если найдено начало однострочного комментария
        if (!in_comment_flag && num_of_current_char + 1 < graph_in_Dot.size() && graph_in_Dot[num_of_current_char] == '/' && graph_in_Dot[num_of_current_char + 1] == '/')
        {

            // Считать, что мы в комментарии
            in_comment_flag = true;

            // Добавить заголовок комментария в строку
            new_graph_in_Dot += graph_in_Dot[num_of_current_char];
            new_graph_in_Dot += graph_in_Dot[num_of_current_char + 1];

            //Перейти к содержимому однострочного комментария
            ++num_of_current_char;
        }

        // Иначе если конец однострочного комментария
        else if (in_comment_flag && graph_in_Dot[num_of_current_char] == '\n')
        {
            // Считать, что мы не в комментарии
            in_comment_flag = false;

            // Добавить символ переноса строки в результат
            new_graph_in_Dot += graph_in_Dot[num_of_current_char];
        }

        // Иначе если текущий символ внутри однострочного комментария или является символом-пропуском или является символом переноса
        else if (in_comment_flag || graph_in_Dot[num_of_current_char] == '\n' || !isspace(graph_in_Dot[num_of_current_char]))
        {
            // Добавить текущий символ в результат
            new_graph_in_Dot += graph_in_Dot[num_of_current_char];
        }
    }

    // Добавить удаленный пробел между ключевым словом и именем графа
    new_graph_in_Dot.replace(new_graph_in_Dot.find("digraph"), 7, "digraph ");

    // Обновить исходную строку
    graph_in_Dot = new_graph_in_Dot;
}

// Функция, выполняющая покраску для связей из соответствующих циклов в указанном графе
void perform_coloring_for_vertices_from_the_corresponding_cycles_in_graph(std::string& graph_in_Dot, const std::vector<std::list<int>>& list_with_simple_cycles)
{
    std::vector< std::string > color_list = { "#FF00FF", "#800080", "#FF0000", "#800000", "#FFFF00",  "#808000",  "#00FF00",  "#008000",  "#00FFFF",
                                           "#008080",  "#0000FF",  "#000080",  "#D2691E",  "#FF4500", "#00FF7F", "#DEB887"};

    //Удалить все разделители из описания графа, не меняя содержимого однострочных комментариев
    remove_whitespace_except_new_line(graph_in_Dot);

    // Составить список связей, которые принадлежат нескольким простым циклам одновременно
    std::set< std::pair<int, int> > repeated_arc = find_repeated_arc_in_cycles(list_with_simple_cycles);

    // Для каждой связи (дуги) из списка с повторяющимися связями
    for (const auto& current_arc : repeated_arc)
    {
        // Создать подстроку для поиска связи из списка
        std::string substring_with_arc = std::to_string(current_arc.first) + "->" + std::to_string(current_arc.second);

        // Найти связь и место для вставки метки о цвете в описании графа
        size_t place_insert = searching_for_substring_that_is_not_comment(graph_in_Dot, substring_with_arc, 0);

        //Вставить метку о цвете на найденную позицию
        graph_in_Dot.insert(place_insert, "[color=\"#808080\"]");
    }


    //Считать, номер цвета из списка цветов еще не определенным
    size_t number_current_color = -1;

    //Для каждого цикла из списка простых циклов
    for (const auto& current_cycle : list_with_simple_cycles)
    {
        // Сохранить первый элемент цикла
        auto second_element = current_cycle.begin();
        auto first_element = second_element;

        // Перейти к следующему элементу цикла
        ++second_element;

        // Если номер цвета не вышел за пределы списка
        if (color_list.size()-1 != number_current_color)
        {
            // Выбрать следующий доступный номер цвета
            number_current_color++;
        }
        // Иначе
        else
        {
            // Установить номер цвета на первый цвет в списке
            number_current_color = 0;
        }

        //Пока текущий цикл графа не пройден полностью
        while (second_element != current_cycle.end())
        {
            // Составить текущую пару (связь) из цикла
            std::pair<int, int> arc = { *first_element, *second_element };

            // Если текущая связь (дуга) не принадлежит списку повторяющихся связей
            if (repeated_arc.find(arc) == repeated_arc.end())
            {
                // Создать подстроку для поиска связи
                std::string substring_with_arc = std::to_string(arc.first) + "->" + std::to_string(arc.second);

                // Найти связь и место для вставки метки о цвете в описании графа
                size_t place_insert = searching_for_substring_that_is_not_comment(graph_in_Dot, substring_with_arc, 0);

                // Вставить метку о цвете на найденную позицию
                graph_in_Dot.insert(place_insert, "[color=\"" + color_list[number_current_color] + "\"]");
            }

            // Перееститься на еще не пройденную связь текущего цикла
            first_element = second_element;
            ++second_element;
        }
    }
}

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

// Функция для замены номеров вершин на соответствующие вершины в списках смежности
std::vector< std::list<int> > replace_vertex_numbers_with_corresponding_vertices_in_adjacency_lists(std::map<int, int>& map_of_graph_vertices_and_their_numbers, std::vector< std::list<int> > graph_adjacency_list_with_vertex_numbers)
{

    std::map<int, int> swapped_map_of_graph_vertices_and_their_numbers;

    // Для каждой пары из словаря с вершинами и их номерами
    for (const auto& current_pair : map_of_graph_vertices_and_their_numbers)
    {
        // Поменять местами вершины и номера
        swapped_map_of_graph_vertices_and_their_numbers[current_pair.second] = current_pair.first;
    }

    // Для каждого списка из списка смежностей, состоящего из номеров вершин
    for (size_t current_list_num = 0; current_list_num < graph_adjacency_list_with_vertex_numbers.size(); ++current_list_num)
    {
        // Для каждой вершины из текущего списка
        for (auto iter = graph_adjacency_list_with_vertex_numbers[current_list_num].begin(); iter != graph_adjacency_list_with_vertex_numbers[current_list_num].end(); iter++)
        {
            // Найти текущую вершину в словаре с вершинами и их номерами
            auto current_vertex_in_map = swapped_map_of_graph_vertices_and_their_numbers.find(*iter);

            // Заменить текущую вершину из списка номером, который ей соответствует
            *iter = current_vertex_in_map->second;
        }
    }

    return graph_adjacency_list_with_vertex_numbers;
}

// Функция, создающая текстовое представление ошибки для вывода в консоль
std::string convert_error_to_str(const Error& error)
{
    std::string error_text;

    //Выбрать текстовое представление ошибки из перечня по ее коду(типу)
    switch (error.type)
    {
    case INVALID_OPEN_FILE:
        error_text = "\nНеверно указан файл с входными данными. Возможно, файл не существует.";
        break;
    case INVALID_KEYWORD:
        error_text = "\nНеверно указанно ключевое слово графа или оно отсутствует. Возможно, допущена ошибка в ключевом слове или оно отсутствует.";
        break;
    case INVALID_HEADER:
        error_text = "\nВо входном файле отсутствует имя графа или оно содержит недопустимые символы.";
        break;
    case INVALID_CURLY_BRACES:
        error_text = "\nГраницы, в которых описан граф, не определены. Возможно, во входном файле отсутствует одна из фигурных скобок или она расположена не на отдельной строке.";
        break;
    case INVALID_SPACE_BETWEEN_HEADING_AND_CURLY_BRACKET:
        error_text = "\nВ промежутке между строкой с ключевым словом и именем графа и строкой с первой фигурной скобкой содержатся лишние символы.";
        break;
    case INVALID_COMMUNICATION_BETWEEN_VERTICES:
        error_text = "\nВ одной из строк, описывающих связи графа, входного файла содержится более двух вершин или содержатся лишние символы.\nСтрока:" + std::to_string(error.line_num) + ".";
        break;
    case INVALID_VERTICE_NAME_OR_SYMBOLS:
        error_text = "\nОписание графа содержит недопустимые символы или недопустимое имя вершины в одной из строк в описании графа.\nСтрока:" + std::to_string(error.line_num) + ".";
        break;
    case SEMOLON_IS_MISSING:
        error_text = "\nВ одной из строк с описанием графа не определено окончание. Возможно, в строке отсутствует символ точки с запятой.\nСтрока:" + std::to_string(error.line_num) + ".";
        break;
    case INVALID_CONNECTION_SIGN:
        error_text = "\nВ графе содержится неопознанная связь. В одной из строк с описанием графа связь между вершинами отсутствует либо указана некорректно.\nСтрока:" + std::to_string(error.line_num) + ".";
        break;
    case INVALID_SYMBOLS_AFTER_GRAPH_DESCRIPTION:
        error_text = "\nПосле описания графа обнаружена лишняя запись. В файле должно содержаться описание только одного графа.";
        break;
    case INVALID_SAVE_FILE:
        error_text = "\nНе удалось сохранить файл для записи результата. Возможно, путь для сохранения или имя файла указаны некорректно.";
        break;
    }

    return error_text;
}

void validate_dot_graph_info(const std::string& dot_info, std::vector<Error>& errors)
{
    // Считать, что ошибки еще не найдены
    bool INVALID_KEYWORD_flag = false, INVALID_HEADER_flag = false, INVALID_CURLY_BRACES_flag = false, INVALID_SPACE_BETWEEN_HEADING_AND_CURLY_BRACKET_flag = false,
        INVALID_COMMUNICATION_BETWEEN_VERTICES_flag = false, INVALID_VERTICE_NAME_OR_SYMBOLS_flag = false, SEMOLON_IS_MISSING_flag = false,
        INVALID_CONNECTION_SIGN_flag = false, INVALID_SYMBOLS_AFTER_GRAPH_DESCRIPTION_flag = false;


    //---------------------------------------------INVAILD_KEYWORD
    std::string temporary_dot_info = dot_info;

    // Удалить однострочные комментарии из исходной строки
    remove_comments_in_string(temporary_dot_info);

    // Создать шаблон для проверки корректности ключевого слова и пространства до него
    static const std::regex correct_keyword(R"rgx(^\s*digraph)rgx");

    // Если текстовое представление графа не соответствует шаблону
    INVALID_KEYWORD_flag = !(std::regex_search(temporary_dot_info, correct_keyword));
    if (INVALID_KEYWORD_flag)
    {
        // Добавить ошибку в список
        errors.push_back({ INVALID_KEYWORD, 0 });
        return;
    }

    //---------------------------------------------INVAILD_HEADER
    temporary_dot_info = dot_info;

    // Удалить однострочные комментарии из исходной строки
    remove_comments_in_string(temporary_dot_info);

    // Создать шаблон для проверки корректности имени графа и пространства до него
    static const std::regex correct_header(R"(^\s*digraph([ |\t|\v|\r|\f])+([A-Za-z_][\w]*))");

    // Если имя графа не соответствует шаблону
    INVALID_HEADER_flag = !(std::regex_search(temporary_dot_info, correct_header));
    if (INVALID_HEADER_flag)
    {
        //Добавить ошибку в список
        errors.push_back({ INVALID_HEADER, 0 });
        return;
    }


    //---------------------------------------------INVALID_CURLY_BRACES
    temporary_dot_info = dot_info;

    // Удалить однострочные комментарии из исходной строки
    remove_comments_in_string(temporary_dot_info);

    int opening_brace = 0;
    int closing_brace = 0;

    for(auto cur_char : temporary_dot_info)
    {
        if(cur_char == '{') opening_brace++;
        else if (cur_char == '}') closing_brace++;
    }

    // Если обнаружено несоответствие
    INVALID_CURLY_BRACES_flag = (opening_brace != 1 || closing_brace != 1);
    if (INVALID_CURLY_BRACES_flag)
    {
        //Добавить ошибку в список
        errors.push_back({ INVALID_CURLY_BRACES, 0 });
        return;
    }

    //---------------------------------------------INVALID_SPACE_BETWEEN_HEADING_AND_CURLY_BRACKET_flag

    // Создать шаблон для проверки пространства между именем графа и открывающей фигурной скобкой
    static const std::regex correct_space_between_heading_and_curly_bracket(R"(^\s*digraph\s+([A-Za-z_][\w]*)\s*\n+\s*[{]\s*\n+\s*)");

    temporary_dot_info = dot_info;

    remove_comments_in_string(temporary_dot_info);

    // Если проверяемое пространство не соответствует шаблону
    INVALID_SPACE_BETWEEN_HEADING_AND_CURLY_BRACKET_flag = !(std::regex_search(temporary_dot_info, correct_space_between_heading_and_curly_bracket));
    if (INVALID_SPACE_BETWEEN_HEADING_AND_CURLY_BRACKET_flag)
    {
        // Добавить ошибку в список
        errors.push_back({ INVALID_SPACE_BETWEEN_HEADING_AND_CURLY_BRACKET, 0 });
        return;
    }


    //---------------------------------------------INVALID_SYMBOLS_AFTER_GRAPH_DESCRIPTION
    temporary_dot_info = dot_info;

    // Удалить однострочные комментарии из текстового представления графа
    remove_comments_in_string(temporary_dot_info);

    // Удалить имя и ключевое слово графа из текстового представления графа
    removing_part_of_string_up_to_specified_character(temporary_dot_info, '}');

    // Создать шаблон для проверки отсутствия символов (кроме пробельных) после закрывающей фигурной скобки
    static const std::regex non_symbols_after_count_description(R"(\}\s*$)");

    // Проверить текстовое представление графа на соответствие шаблону
    INVALID_SYMBOLS_AFTER_GRAPH_DESCRIPTION_flag = !(std::regex_search(temporary_dot_info, non_symbols_after_count_description));

    //Если обнаружено несоответствие
    if (INVALID_SYMBOLS_AFTER_GRAPH_DESCRIPTION_flag)
    {
        // Добавить ошибку в список
        errors.push_back({ INVALID_SYMBOLS_AFTER_GRAPH_DESCRIPTION, 0 });
        return;
    }


    //---------------------------------------------INVALID_VERTICE_NAME_OR_SYMBOLS
    temporary_dot_info = dot_info;

    // Удалить однострочные комментарии из текстового представления графа
    remove_comments_in_string(temporary_dot_info);

    // Удалить закрывающую фигурную скобку и текст до нее из текстового представления графа
    temporary_dot_info = temporary_dot_info.substr(0, temporary_dot_info.find("}", 0));

    // Удалить все точки с запятой из текстового представления графа
    std::string characters_to_delete = ";";
    remove_all_occurrences_of_character_in_string(temporary_dot_info, characters_to_delete);

    std::stringstream temporary_dot_info_stream_1(temporary_dot_info);
    std::string current_substr_of_temporary_dot_info;

    // Считать, что нумерация строк будет начинаться с 1
    int number_of_current_str = 1;

    // Пока в описании графа есть строки и открывающая фигурная скобка не найдена
    while (std::getline(temporary_dot_info_stream_1, current_substr_of_temporary_dot_info) && current_substr_of_temporary_dot_info.find("{") == std::string::npos)
    {
        // Считать строки
        number_of_current_str++;
    }

    // Пока в описании графа есть строки
    while (std::getline(temporary_dot_info_stream_1, current_substr_of_temporary_dot_info))
    {
        // посчитать текущую строку
        number_of_current_str++;

        // Считать, что текщая строка еще не проверена
        INVALID_VERTICE_NAME_OR_SYMBOLS_flag = false;

        // Для каждого символа текущей строки
        for (auto current_char : current_substr_of_temporary_dot_info)
        {
            // Если текущий символ не является допустимым в строке со связями
            if (!isspace(current_char) && !isdigit(current_char) && current_char != '-' && current_char != '>' && INVALID_VERTICE_NAME_OR_SYMBOLS_flag == false)
            {
                // Считать, что ошибка найдена
                INVALID_VERTICE_NAME_OR_SYMBOLS_flag = true;
            }
        }

        // Если ошибка найдена
        if (INVALID_VERTICE_NAME_OR_SYMBOLS_flag)
        {
            // Добавить ошибку в список
            errors.push_back({ INVALID_VERTICE_NAME_OR_SYMBOLS, number_of_current_str });
            return;
        }

    }

    //---------------------------------------------INVALID_COMMUNICATION_BETWEEN_VERTICES, INVALID_CONNECTION_SIGN
    temporary_dot_info = dot_info;

    // Удалить однострочные комментарии из текстового представления графа
    remove_comments_in_string(temporary_dot_info);

    // Удалить закрывающую фигурную скобку и текст до нее из текстового представления графа
    temporary_dot_info = temporary_dot_info.substr(0, temporary_dot_info.find("}", 0));

    // Удалить все точки с запятой из текстового представления графа
    characters_to_delete = ";";
    remove_all_occurrences_of_character_in_string(temporary_dot_info, characters_to_delete);

    std::stringstream temporary_dot_info_stream(temporary_dot_info);

    //Считать, что нумерация строк будет начинаться с 1
    number_of_current_str = 1;

    // Пока в описании графа есть строки и открывающая фигурная скобка не найдена
    while (std::getline(temporary_dot_info_stream, current_substr_of_temporary_dot_info) && current_substr_of_temporary_dot_info.find("{") == std::string::npos)
    {
        // Считать строки
        number_of_current_str++;


    }

    //Переменные для подсчета не пробельных символов строки
    int count_num;
    int count_arr;
    int count_dash;
    bool inNumber;

    // Пока в текстовом описании графа есть строки
    while (std::getline(temporary_dot_info_stream, current_substr_of_temporary_dot_info))
    {
        // Посчитать текущую строку
        number_of_current_str++;

        //Считать не пробельные символы
        count_num = 0;
        count_arr = 0;
        count_dash = 0;
        inNumber = false;
        for (char current_char : current_substr_of_temporary_dot_info) {
            if (isdigit(current_char)) {
                if (!inNumber) {
                    inNumber = true;
                    ++count_num;
                }
            } else {
                inNumber = false;
            }

            if(current_char == '-')
            {
                count_dash++;
            }

            if(current_char == '>')
            {
                count_arr++;
            }
        }

        if(count_num > 2)
        {
            INVALID_COMMUNICATION_BETWEEN_VERTICES_flag = true;

        }

        else if((count_dash > 1 || count_arr > 1) && ((count_dash + count_arr) % 2 == 0 ))
        {
            INVALID_COMMUNICATION_BETWEEN_VERTICES_flag = true;

        }

        else if(count_num == 2 && (((count_dash + count_arr) % 2 != 0) || (count_dash + count_arr == 0)))
        {
            //Считать,  что ошибка найдена
            INVALID_CONNECTION_SIGN_flag = true;
        }


        // Если текущая строка содержит ошибку
        if(INVALID_COMMUNICATION_BETWEEN_VERTICES_flag)
        {
            // Добавить ошибку в список
            errors.push_back({ INVALID_COMMUNICATION_BETWEEN_VERTICES, number_of_current_str });
            return;
        }

        // Если ошибка найдена
        if (INVALID_CONNECTION_SIGN_flag)
        {
            // Добавить ошибку в список
            errors.push_back({ INVALID_CONNECTION_SIGN, number_of_current_str });
            return;
        }

    }

    //---------------------------------------------SEMOLON_IS_MISSING
    temporary_dot_info = dot_info;

    // Удалить однострочные комментарии из текстового представления графа
    remove_comments_in_string(temporary_dot_info);

    // Удалить закрывающую фигурную скобку и текст до нее из текстового представления графа
    temporary_dot_info = temporary_dot_info.substr(0, temporary_dot_info.find("}", 0));

    std::stringstream temporary_dot_info_stream_2(temporary_dot_info);

    // Считать, что нумерация строк будет начинаться с 1
    number_of_current_str = 1;

    // Пока в описании графа есть строки и открывающая фигурная скобка не найдена
    while (std::getline(temporary_dot_info_stream_2, current_substr_of_temporary_dot_info) && current_substr_of_temporary_dot_info.find("{") == std::string::npos)
    {
        // Считать строки
        number_of_current_str++;
    }

    // Пока в описании графа есть строки
    while (std::getline(temporary_dot_info_stream_2, current_substr_of_temporary_dot_info))
    {
        // Посчитать текущую строку
        number_of_current_str++;

        //Считать текущую строку еще не проверенной
        SEMOLON_IS_MISSING_flag = false;

        // Если в текущей строке есть вершина и символ точки с запятой встречается не один раз
        if (current_substr_of_temporary_dot_info.find_first_of("0123456789") != std::string::npos &&
            !(std::count(current_substr_of_temporary_dot_info.begin(), current_substr_of_temporary_dot_info.end(), ';') == 1))
        {
            // Считать, что в текущей строке обнаружена ошибка
            SEMOLON_IS_MISSING_flag = true;
        }

        // Если обнаружена ошибка
        if (SEMOLON_IS_MISSING_flag)
        {
            // Добавить ошибку в список
            errors.push_back({ SEMOLON_IS_MISSING, number_of_current_str });
            return;
        }

    }

}

// Функция для считывания содержимого файла с записью графа по указанному пути
bool read_text_file(const std::string& file_path, std::string& text_of_file, std::vector<Error>& errors)
{

    QFile file(QString::fromStdString(file_path));

    // Если файл не был открыт
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Добавить ошибку в список
        errors.push_back({ INVALID_OPEN_FILE, 0 });
        return false;
    }

    // Считать содержимое файла в строку
    QTextStream in(&file);
    QString qtext_of_file;
    qtext_of_file = in.readAll();
    text_of_file = qtext_of_file.toStdString();

    file.close();
    return true;
}



