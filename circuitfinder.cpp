#include "circuitfinder.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

void CircuitFinder::unblock(int vertex_to_unlock)
{
    List_of_blocked_vertices_of_current_cycle_search[vertex_to_unlock - 1] = false;

    while (!List_of_vertex_to_unlock[vertex_to_unlock - 1].empty()) {
        int adjacent_vertex = List_of_vertex_to_unlock[vertex_to_unlock - 1].front();
        List_of_vertex_to_unlock[vertex_to_unlock - 1].pop_front();

        if (List_of_blocked_vertices_of_current_cycle_search[adjacent_vertex - 1]) {
            unblock(adjacent_vertex);
        }
    }
}

bool CircuitFinder::circuit(int vertex, std::vector<NodeList>& list_with_simple_cycles)
{
    bool cycle_found = false;
    Current_path.push_back(vertex);
    List_of_blocked_vertices_of_current_cycle_search[vertex - 1] = true;

    for (int adjacent_vertex : Adjacency_list[vertex - 1]) {
        if (adjacent_vertex == Starting_vertex) {
            output(list_with_simple_cycles);
            cycle_found = true;
        }
        else if (adjacent_vertex > Starting_vertex && !List_of_blocked_vertices_of_current_cycle_search[adjacent_vertex - 1]) {
            cycle_found = circuit(adjacent_vertex, list_with_simple_cycles);
        }
    }

    if (cycle_found) {
        unblock(vertex);
    }
    else {
        for (int adjacent_vertex : Adjacency_list[vertex - 1]) {
            auto current_vertex = std::find(List_of_vertex_to_unlock[adjacent_vertex - 1].begin(), List_of_vertex_to_unlock[adjacent_vertex - 1].end(), vertex);
            if (current_vertex == List_of_vertex_to_unlock[adjacent_vertex - 1].end()) {
                List_of_vertex_to_unlock[adjacent_vertex - 1].push_back(vertex);
            }
        }
    }

    Current_path.pop_back();
    return cycle_found;
}



void CircuitFinder::output(std::vector<NodeList>& list_with_simple_cycles)
{
    // Список для хранения текущего цикла
    NodeList current_cycle;

    // Для каждой вершины из текущего пути
    for (auto current_vertex = Current_path.begin(), last_vertex = Current_path.end(); current_vertex != last_vertex; ++current_vertex)
    {
        // Копировать вершины из текущего пути в цикл
        current_cycle.push_back(*current_vertex);
    }

    // Добавить начальную вершину в конец цикла
    current_cycle.push_back(*Current_path.begin());

    // Добавить цикл в список найденных циклов
    list_with_simple_cycles.push_back(current_cycle);
}


void CircuitFinder::run(std::vector<NodeList>& list_with_simple_cycles)
{
    Current_path.clear();
    Starting_vertex = 1;

    while (Starting_vertex < Number_vertices) {
        for (int current_vertex = Starting_vertex; current_vertex <= Number_vertices; ++current_vertex) {
            List_of_blocked_vertices_of_current_cycle_search[current_vertex - 1] = false;
            List_of_vertex_to_unlock[current_vertex - 1].clear();
        }
        circuit(Starting_vertex, list_with_simple_cycles);
        ++Starting_vertex;
    }
}
