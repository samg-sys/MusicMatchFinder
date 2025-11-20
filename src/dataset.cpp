// dataset.cpp
// Implementación de la lectura CSV muy simple (espera formato: title,genre,year)
// No maneja comillas ni comas dentro de campos; es suficiente para datos limpios.

#include "dataset.h"
#include <fstream>      // ifstream
#include <sstream>      // stringstream
#include <iostream>     // cerr
#include <vector>

std::vector<Song> Dataset::loadSongs(const std::string& filename) {
    std::vector<Song> songs;                 // vector resultado
    std::ifstream file(filename);            // abrir archivo
    if (!file.is_open()) {                   // comprobar apertura
        std::cerr << "No se pudo abrir " << filename << std::endl;
        return songs;                        // devolver vacío si falla
    }

    std::string line;
    // leer línea por línea
    while (std::getline(file, line)) {
        if (line.empty()) continue;          // omitir líneas vacías
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> fields;
        while (std::getline(ss, token, ',')) {
            fields.push_back(token);
        }

        // soporte para encabezado: si el primer campo es "title" lo saltamos
        if (fields.size() == 0) continue;
        if (fields[0] == "title") continue;

        std::string title, genre, yearStr;

        if (fields.size() == 4) {
            // formato: title,artist,genre,year
            title = fields[0];
            genre = fields[2];
            yearStr = fields[3];
        } else if (fields.size() == 3) {
            // formato: title,genre,year
            title = fields[0];
            genre = fields[1];
            yearStr = fields[2];
        } else {
            // formato inesperado: saltar
            continue;
        }
        // conversión segura de año, si falla dejamos 0
        int year = 0;
        try {
            year = std::stoi(yearStr);
        } catch (...) {
            year = 0;
        }

        songs.push_back({title, genre, year}); // agregar al vector
    }

    file.close(); // cerrar archivo
    return songs; // devolver lista
}

std::vector<Movie> Dataset::loadMovies(const std::string& filename) {
    std::vector<Movie> movies;               // vector resultado
    std::ifstream file(filename);            // abrir archivo
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir " << filename << std::endl;
        return movies;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string title, genre, yearStr;

        std::getline(ss, title, ',');
        std::getline(ss, genre, ',');
        std::getline(ss, yearStr, ',');

        int year = 0;
        try {
            year = std::stoi(yearStr);
        } catch (...) {
            year = 0;
        }

        movies.push_back({title, genre, year});
    }

    file.close();
    return movies;
}