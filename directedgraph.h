#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <list>



// Класс DirectedGraph для работы с ориентированным графом
class DirectedGraph
{
public:

    //Конструктор класса DirectedGraph
    DirectedGraph(const std::string& input_dotGraph) : graph_in_Dot(input_dotGraph)
    {
        //Спарсить описание графа на языке Dot и заполнить поля класса
        parseGraph();
    }

    //Получить список дуг
    const std::vector< std::pair<int, int> >& getGraphArcList() const;

    //Получить представление графа на языке Dot
    const std::string& getGraphInDot() const;

    //Получить список смежности графа
    const std::vector< std::list<int> >& getGraphAdjacencyList() const;

    //Получить список простых циклов графа
    const std::vector< std::list<int> >& getListOfSimpleCycles() const;

    //Найти все простые циклы графа
    void findSimpleGraphCycles();

private:


    std::string graph_in_Dot;                                   //Запись графа на языке Dot
    std::vector< std::pair<int, int> > graph_arc_list;          //Вектор, содержащий дуги графа в виде пар смежных вершин
    std::vector< std::list<int> > graph_adjacency_list;         //Список смежности графа, состоящий из номеров вершин
    std::map<int, int> map_of_graph_vertices_and_their_numbers; //Словарь, содержащий вершины и соответствующие им номера
    std::vector< std::list<int> > list_with_simple_cycles;      // Список, содержащий простые циклы графа

    //Спарсить описание графа на языке Dot и заполнить поля класса
    void parseGraph();

};

#endif // DIRECTEDGRAPH_H
