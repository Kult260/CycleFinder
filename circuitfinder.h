#ifndef CIRCUITFINDER_H
#define CIRCUITFINDER_H


#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

typedef std::list<int> NodeList;

class CircuitFinder
{

private:
    std::vector<NodeList> Adjacency_list;                               // Список смежности для представления графа
    std::vector<int> Current_path;                                      // текущий путь поиска
    std::vector<bool> List_of_blocked_vertices_of_current_cycle_search; // Вектор заблокированных вершин для текущего поиска цикла
    std::vector<NodeList> List_of_vertex_to_unlock;                     // Список, содержащий вершины, которые должны быть разблокированы, если текущая вершина будет разблокирована
    int Starting_vertex;                                                // Начальная вершина для текущей итерации
    int Number_vertices;                                                // Общее количество вершин в графе

    // Разблокировка вершины vertex_to_unlock
    void unblock(int vertex_to_unlock);

    // Поиск циклов, начиная с вершины vertex
    bool circuit(int vertex, std::vector<NodeList>& list_with_simple_cycles);

    // Вывод текущего найденного цикла в список с найденными циклами
    void output(std::vector<NodeList>& list_with_simple_cycles);


public:

    // Конструктор, инициализирующий граф и вспомогательные структуры
    CircuitFinder(const std::vector<NodeList> graph_adjacency_list)
        : Adjacency_list(graph_adjacency_list), List_of_blocked_vertices_of_current_cycle_search(graph_adjacency_list.size(), false), List_of_vertex_to_unlock(graph_adjacency_list.size()), Number_vertices(graph_adjacency_list.size()) {}

    // Запуск алгоритма поиска простых циклов
    void run(std::vector<NodeList>& list_with_simple_cycles);
};

#endif // CIRCUITFINDER_H
