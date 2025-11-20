#include <iostream>        // para cout, cin
#include "dataset.h"       // carga de datasets
#include "graph.h"         // grafo de relaciones género -> género
#include "tree.h"          // árbol para organización simple

int main() {
    // Crear instancias de las clases principales
    Dataset data;                      // objeto que lee los CSV
    Graph graph;                       // grafo de relaciones entre géneros
    BinarySearchTree tree;             // árbol para ejemplo/visualización

    std::cout << "=== MUSIC MATCH FINDER ===\n"; // encabezado
    std::cout << "1. Cargar datos\n";            // opción 1
    std::cout << "2. Ver canciones (debug)\n";   // opción 2
    std::cout << "3. Ver películas (debug)\n";   // opción 3
    std::cout << "4. Salir\n";                  // opción 4

    int option = 0;                   // variable para elegir opción
    std::cin >> option;               // leer opción del usuario

    if (option == 1) {
        // Cargar canciones y películas desde archivos CSV dentro de data/
        auto songs = data.loadSongs("data/songs_clean.csv");   // leer canciones limpias
        auto movies = data.loadMovies("data/movies_clean.csv"); // leer películas limpias

        // Mostrar conteo básico
        std::cout << "Datos cargados correctamente!\n";
        std::cout << "Canciones: " << songs.size() << "\n";
        std::cout << "Películas: " << movies.size() << "\n";

        // Ejemplo simple: insertar géneros en el árbol para visualizar
        for (const auto& s : songs) {
            tree.insert(s.title);      // insertar título de canción en árbol
        }
        std::cout << "Árbol (in-order) de títulos de canciones:\n";
        tree.display();               // mostrar árbol en orden
    } else if (option == 2) {
        // En futuras versiones: listar canciones (por ahora pedimos cargar primero)
        std::cout << "Selecciona '1' para cargar datos y luego verlos.\n";
    } else if (option == 3) {
        std::cout << "Selecciona '1' para cargar datos y luego verlos.\n";
    } else {
        std::cout << "Saliendo...\n";
    }

    return 0; // fin del programa
}