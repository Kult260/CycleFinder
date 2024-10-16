#include "directedgraph.h"
#include "functions.h"
#include "structs.h"
#include <QCoreApplication>
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
#include <QTextStream>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextStream outStream(stdout);
    outStream.setCodec(QTextCodec::codecForName("cp866")); // Кодировка для запуска в консоли(не Qt): "cp866", (в Qt): "Windows-1251"


    std::string graph_description; // Текстовое представление графа на языке Dot
    std::vector<Error> errors;      // Список для найденных ошибок
    std::string input_file_path;    //Путь к входнному фалу
    std::string output_file_path;   //Путь для сохранения выходного файла


    // Ввод пути с входным файлом
    input_file_path = argv[1];

    // Ввод пути, по которому будет сохранен выходной файл
    output_file_path = argv[2];

    if (argc != 3)
    {
        QString text_error = QString::fromStdString("Программа принимает данные на вход в следующей форме: <аргумент запуска программы> <путь к dot файлу с входными данными> <путь к выходному dot файлу>\n");
        outStream << text_error << Qt::flush;
        return 1;
    }

    // Если не удалось считать содержимое файла
    if (!read_text_file(input_file_path, graph_description, errors))
    {
        // Вывести все найденные ошибки в консоль и завершить выполнение программы
        for (const Error& current_error : errors)
        {
            QString text_error = QString::fromStdString(convert_error_to_str(current_error));
            outStream << text_error << Qt::flush;
        }
        return 1;
    }

    // Проверить полученное текстовое представление графа на языке Dot на корректность данных
    validate_dot_graph_info(graph_description, errors);

    //Если надены ошибки
    if (errors.size() > 0)
    {
        // Вывести все найденные ошибки в консоль и завершить выполнение программы
        for (const Error& current_error : errors)
        {
            QString text_error = QString::fromStdString(convert_error_to_str(current_error));
            outStream << text_error << Qt::flush;
        }
        return 1;
    }

    // Спарсить текстовое содержание графа для получения необходимых данных
    DirectedGraph directed_graph(graph_description);

    // Найти все простые циклы графа
    directed_graph.findSimpleGraphCycles();

    // Если простые циклы не были найдены
    if ((directed_graph.getListOfSimpleCycles()).empty())
    {
        // Вывести сообщение о том, что граф ациклический и завершить выполнение программы
        outStream << QString::fromStdString("\nГраф ациклический и не имеет циклов.\n") << Qt::flush;
        return 0;
    }

    // Покрасить все вершины каждого найденного простого цикла графа
    perform_coloring_for_vertices_from_the_corresponding_cycles_in_graph(graph_description, directed_graph.getListOfSimpleCycles());


    // Если не удалось сохранить текстовое представление графа в выходной файл
    if (!write_text_to_file(output_file_path, graph_description, errors))
    {
        // Вывести все найденные ошибки в консоль и завершить выполнение программы
        for (const Error& current_error : errors)
        {
            QString text_error = QString::fromStdString(convert_error_to_str(current_error));
            outStream << text_error << Qt::flush;
        }
        return 1;
    }

    return 0;
}
