#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
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
}

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
}

cout << "Filtrado completado correctamente. Archivos generados en /data.\n";
return 0;
}