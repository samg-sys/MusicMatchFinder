// graph.h
// Cabecera del grafo simple que relaciona géneros de canciones con géneros de películas.
// Usamos unordered_map para mapear un género de canción a una lista de géneros de película.

#ifndef GRAPH_H
#define GRAPH_H

#include <string>               // para std::string
#include <vector>               // para std::vector
#include <unordered_map>        // para std::unordered_map

class Graph {
private:
    // mapa: género de canción -> lista de géneros de película relacionados
    std::unordered_map<std::string, std::vector<std::string>> adjList;

public:
    // agrega una arista del género de canción al género de película
    void addEdge(const std::string& songGenre, const std::string& movieGenre);

    // devuelve lista de géneros de película relacionados a un género dado
    std::vector<std::string> getMoviesByGenre(const std::string& genre);
};

#endif // GRAPH_H