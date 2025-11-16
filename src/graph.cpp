// graph.cpp
// Implementación mínima del grafo.

#include "graph.h"               // incluir la cabecera
#include <algorithm>             // para find (opcional)

void Graph::addEdge(const std::string& songGenre, const std::string& movieGenre) {
    // Añadir movieGenre a la lista de songGenre (no comprobamos duplicados para simplicidad)
    adjList[songGenre].push_back(movieGenre);
}

std::vector<std::string> Graph::getMoviesByGenre(const std::string& genre) {
    // Si no existe la clave, unordered_map crea una entrada vacía al acceder con []
    return adjList[genre];