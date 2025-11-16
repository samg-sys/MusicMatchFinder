// dataset.h
// Cabecera para leer datasets simples desde CSV.
// Definimos estructuras Song y Movie con campos mínimos.

#ifndef DATASET_H
#define DATASET_H

#include <string>
#include <vector>

struct Song {
    std::string title;    // título de la canción
    std::string genre;    // género musical
    int year;             // año de lanzamiento
};

struct Movie {
    std::string title;    // título de la película
    std::string genre;    // género cinematográfico
    int year;             // año de estreno
};

class Dataset {
public:
    // cargan vectores de Song/Movie desde archivos CSV simples
    std::vector<Song> loadSongs(const std::string& filename);
    std::vector<Movie> loadMovies(const std::string& filename);
};

#endif // DATASET_H
