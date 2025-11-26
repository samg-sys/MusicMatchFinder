#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <cmath>
using namespace std;

struct Song {
string title;
string artist;
string genre;
int year;
};

struct Movie {
string title;
string genre;
int year;
};
struct Match {
string songTitle;
string songArtist;
string songGenre;
int songYear;
string movieTitle;
string movieGenre;
int movieYear;
vector<string> matchGenres;
double similarityScore;
};


// Función para convertir string a minúsculas y evitar cssos sensisitvos
string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Función para dividir un string por delimitador () si géneros son separados por |)
vector<string> splitGenres(const string& genreStr, char delimiter = '|') {
    vector<string> genres;
    stringstream ss(genreStr);
    string genre;
    while (getline(ss, genre, delimiter)) {
        // eliminar espacios en blanco al inicio y final
        genre.erase(0, genre.find_first_not_of(" \t"));
        genre.erase(genre.find_last_not_of(" \t") + 1);
        if (!genre.empty()) {
            genres.push_back(toLower(genre));
        }
    }
    return genres;
}
// Función para calcular similitud Jaccard (intersección / unión)
double jaccardSimilarity(const vector<string>& set1, const vector<string>& set2) {
    set<string> s1(set1.begin(), set1.end());
    set<string> s2(set2.begin(), set2.end());
    
    int intersection = 0;
    for (const auto& g : s1) {
        if (s2.count(g)) intersection++;
    }
    
    int unionSize = s1.size() + s2.size() - intersection;
    if (unionSize == 0) return 0.0;
    
    return (double)intersection / unionSize;
}

// Función para calcular puntaje de similitud temporal (años cercanos = puntaje mayor)
double getTemporalBonus(int songYear, int movieYear) {
    int yearDiff = abs(songYear - movieYear);
    if (yearDiff <= 2) return 1.15;      // mismo año o ±1-2 años: bonus 15%
    if (yearDiff <= 5) return 1.10;      // ±3-5 años: bonus 10%
    if (yearDiff <= 10) return 1.05;     // ±6-10 años: bonus 5%
    return 1.0;                          // más de 10 años: sin bonus
}

// Función para calcular puntaje de similitud global
double calculateSimilarityScore(const Song& song, const Movie& movie,
                                const vector<string>& songGenres,
                                const vector<string>& movieGenres,
                                const vector<string>& matchGenres) {
    // Base: similitud Jaccard (0-100)
    double jaccardScore = jaccardSimilarity(songGenres, movieGenres) * 100.0;
    
    // Puntaje de coincidencia exacta (cuántos géneros coinciden)
    double exactMatchScore = (double)matchGenres.size() / max(songGenres.size(), movieGenres.size()) * 100.0;
    
    // Promedio ponderado: 70% Jaccard, 30% coincidencia exacta
    double baseScore = (jaccardScore * 0.7) + (exactMatchScore * 0.3);
    
    // Bonus temporal
    double temporalBonus = getTemporalBonus(song.year, movie.year);
    
    // Score final (0-115 puntos máximo)
    double finalScore = baseScore * temporalBonus;
    
    return min(finalScore, 100.0); // capped a 100
}

// Función para encontrar coincidencias y calcular similitud
void matchGenresWithSimilarity(const string& songsFile, const string& moviesFile, const string& outputFile) {
    vector<Song> songs;
    vector<Movie> movies;

    // Leer canciones
    ifstream inSongs(songsFile);
    if (!inSongs.is_open()) {
        cerr << "Error: no se pudo abrir " << songsFile << "\n";
        return;
    }
    string line;
    getline(inSongs, line);
    while (getline(inSongs, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string title, artist, genre, yearStr;
        getline(ss, title, ',');
        getline(ss, artist, ',');
        getline(ss, genre, ',');
        getline(ss, yearStr, ',');
        try {
            int year = stoi(yearStr);
            songs.push_back({title, artist, genre, year});
        } catch (...) {
            continue;
        }
    }
    inSongs.close();

// Función para encontrar coincidencias de género entre canciones y películas
void matchGenres(const string& songsFile, const string& moviesFile, const string& outputFile) {
    vector<Song> songs;
    vector<Movie> movies;

    // Leer canciones limpias
    ifstream inSongs(songsFile);
    if (!inSongs.is_open()) {
        cerr << "Error: no se pudo abrir " << songsFile << "\n";
        return;
    }
    string line;
    getline(inSongs, line); // saltar encabezado
    while (getline(inSongs, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string title, artist, genre, yearStr;
        getline(ss, title, ',');
        getline(ss, artist, ',');
        getline(ss, genre, ',');
        getline(ss, yearStr, ',');
        try {
            int year = stoi(yearStr);
            songs.push_back({title, artist, genre, year});
        } catch (...) {
            continue;
        }
    }
    inSongs.close();

    // Leer películas limpias
    ifstream inMovies(moviesFile);
    if (!inMovies.is_open()) {
        cerr << "Error: no se pudo abrir " << moviesFile << "\n";
        return;
    }
    getline(inMovies, line); // saltar encabezado
    while (getline(inMovies, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string title, genre, yearStr;
        getline(ss, title, ',');
        getline(ss, genre, ',');
        getline(ss, yearStr, ',');
        try {
            int year = stoi(yearStr);
            movies.push_back({title, genre, year});
        } catch (...) {
            continue;
        }
    }
    inMovies.close();

    // Crear mapeo: género -> películas que tienen ese género
    unordered_map<string, vector<Movie>> genreToMovies;
    for (const auto& movie : movies) {
        vector<string> movieGenres = splitGenres(movie.genre);
        for (const auto& genre : movieGenres) {
            genreToMovies[genre].push_back(movie);
        }
    }

// Generar matches
    vector<Match> matches;
    for (const auto& song : songs) {
        vector<string> songGenres = splitGenres(song.genre);
        
        for (const auto& movie : movies) {
            vector<string> movieGenres = splitGenres(movie.genre);
            
            // Encontrar géneros comunes
            vector<string> commonGenres;
            for (const auto& sg : songGenres) {
                for (const auto& mg : movieGenres) {
                    if (sg == mg) {
                        commonGenres.push_back(sg);
                        break;
                    }
                }
            }
            
            // Si hay al menos un género común, crear match
            if (!commonGenres.empty()) {
                double score = calculateSimilarityScore(song, movie, songGenres, movieGenres, commonGenres);
                matches.push_back({
                    song.title, song.artist, song.genre, song.year,
                    movie.title, movie.genre, movie.year,
                    commonGenres, score
                });
            }
        }
    }

    // Ordenar por puntaje descendente
    sort(matches.begin(), matches.end(), [](const Match& a, const Match& b) {
        return a.similarityScore > b.similarityScore;
    });

    // Escribir resultados
    ofstream outMatches(outputFile);
    outMatches << "song_title,song_artist,song_genre,song_year,movie_title,movie_genre,movie_year,similarity_score,match_genres\n";

    for (const auto& match : matches) {
        string genresStr;
        for (size_t i = 0; i < match.matchGenres.size(); i++) {
            genresStr += match.matchGenres[i];
            if (i < match.matchGenres.size() - 1) genresStr += "|";
        }
        
        outMatches << match.songTitle << ","
                   << match.songArtist << ","
                   << match.songGenre << ","
                   << match.songYear << ","
                   << match.movieTitle << ","
                   << match.movieGenre << ","
                   << match.movieYear << ","
                   << fixed << setprecision(2) << match.similarityScore << ","
                   << genresStr << "\n";
    }
    outMatches.close();

    cout << "Matching completado. Resultados guardados en " << outputFile << "\n";
    cout << "Canciones procesadas: " << songs.size() << "\n";
    cout << "Películas procesadas: " << movies.size() << "\n";
    cout << "Coincidencias encontradas: " << matches.size() << "\n";


}
int main() {
ifstream songsFile("data/spotify_tracks.csv"); // dataset original
ofstream cleanSongs("data/songs_clean.csv"); // dataset filtrado
ifstream moviesFile("data/movies_dataset.csv"); // dataset original
ofstream cleanMovies("data/movies_clean.csv"); // dataset filtrado

if (!songsFile.is_open() || !moviesFile.is_open()) {
cerr << "Error: no se pudieron abrir los archivos originales.\n";
return 1;
}

string line, title, artist, genre, yearStr;

cleanSongs << "title,artist,genre,year\n";
getline(songsFile, line); // saltar encabezado
while (getline(songsFile, line)) {
stringstream ss(line);
getline(ss, title, ',');
getline(ss, artist, ',');
getline(ss, genre, ',');
getline(ss, yearStr, ',');

if (yearStr.empty()) continue;
int year = stoi(yearStr);
if (year >= 1970 && year <= 2025)
cleanSongs << title << "," << artist << "," << genre << "," << year << "\n";
} catch (...) {
            continue;
        }
    }
    cleanSongs.close();

cleanMovies << "title,genre,year\n";
getline(moviesFile, line);
while (getline(moviesFile, line)) {
stringstream ss(line);
getline(ss, title, ',');
getline(ss, genre, ',');
getline(ss, yearStr, ',');

if (yearStr.empty()) continue;
int year = stoi(yearStr);
if (year >= 1970 && year <= 2025)
cleanMovies << title << "," << genre << "," << year << "\n";
}catch (...) {
            continue;
        }
    }
    cleanMovies.close();

    songsFile.close();
    moviesFile.close();

    cout << "Filtrado completado correctamente. Archivos generados en /data.\n";

    // Ejecutar matching de géneros
    cout << "\nIniciando matching de géneros...\n";
    matchGenres("data/songs_clean.csv", "data/movies_clean.csv", "data/matches.csv");
// Ejecutar matching con similitud
    cout << "\nIniciando matching de géneros con puntaje de similitud...\n";
    matchGenresWithSimilarity("data/songs_clean.csv", "data/movies_clean.csv", "data/matches.csv");
    return 0;
}