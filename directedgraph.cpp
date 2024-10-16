#include "circuitfinder.h"
#include "directedgraph.h"
#include "functions.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <list>
#include <QDebug>


//Получить список дуг
const std::vector< std::pair<int, int> >& DirectedGraph::getGraphArcList() const
{
    return this->graph_arc_list;
}

//Получить представление графа на языке Dot
const std::string& DirectedGraph::getGraphInDot() const
{
    return this->graph_in_Dot;
}

//Получить список смежности графа
const std::vector< std::list<int> >& DirectedGraph::getGraphAdjacencyList() const
{
    return this->graph_adjacency_list;
}

//Получить список простых циклов графа
const std::vector< std::list<int> >& DirectedGraph::getListOfSimpleCycles() const
{
    return this->list_with_simple_cycles;
}

//Найти все простые циклы графа
void DirectedGraph::findSimpleGraphCycles()
{
    std::vector< std::list<int> > graph_adjacency_list_with_vertex_numbers = replace_vertices_with_their_serial_numbers_in_adjacency_lists(this->map_of_graph_vertices_and_their_numbers, this->graph_adjacency_list);
    std::vector< std::list<int> > list_with_simple_cycles_with_vertex_numbers;
    CircuitFinder CF(graph_adjacency_list_with_vertex_numbers);
    CF.run(list_with_simple_cycles_with_vertex_numbers);
    this->list_with_simple_cycles = replace_vertex_numbers_with_corresponding_vertices_in_adjacency_lists(this->map_of_graph_vertices_and_their_numbers, list_with_simple_cycles_with_vertex_numbers);
}

void DirectedGraph::parseGraph()
{
    // Создать копию описания графа на языке Dot для дальнейших преобразований
    std::string temporary_graph_in_Dot = graph_in_Dot;

    // Привести запись графа на языке Dot к удобному для получения данных формату
    bring_graph_record_into_format_convenient_obtaining_data(temporary_graph_in_Dot);

    // Создать список дуг графа
    std::string arrow_between_vertices_in_text = "->";
    create_map_with_vertices_of_graphand_their_numbers(temporary_graph_in_Dot, arrow_between_vertices_in_text, graph_arc_list);

    // Составить словарь с вершинами графа и их порядковыми номерами
    fill_map_with_values_and_their_numbers(map_of_graph_vertices_and_their_numbers, graph_arc_list);

    // Заполнить список смежности номерами вершин графа
    fill_adjacency_list_of_graph_with_vertex_numbers(map_of_graph_vertices_and_their_numbers, graph_arc_list, graph_adjacency_list);


}





