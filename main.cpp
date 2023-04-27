#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
#include <regex>
#include <limits>
#include "graph.h"
using namespace std;

void trim(string& str) {
    // Remove the leading whitespace.
    size_t startpos = str.find_first_not_of(" \t\r\n");
    if (startpos != string::npos) {
        str.erase(0, startpos);
    }
    // Remove the trailing whitespace.
    size_t endpos = str.find_last_not_of(" \t\r\n");
    if (endpos != string::npos) {
        str.erase(endpos + 1);
    }
}


int main() {
    // Open the CSV file for reading.
    ifstream file("gametest100000.csv");

    // Make sure the file was opened successfully.
    if (!file.is_open()) {
        cout << "Failed to open file." << endl;
        return 1;
    }

    // Initialize a map to store the names of the games by genre.
    map<string, vector<string>> games_by_genre;
    map<string, vector<string>> games_with_genre;

    string line;
    int numOfGames = 0; // Keeps track of all the games in the map (only including the games with a genre).
    while (getline(file, line)) {
        // Split the line into columns using regex to handle commas inside quotes.
        vector<string> columns;
        regex re(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)");
        sregex_token_iterator it(line.begin(), line.end(), re, -1);
        sregex_token_iterator end;
        while (it != end) {
            columns.push_back(*it++);
        }

        string name = "";
        string genres = "";
        if (columns.size() >= 2) {
            name = columns[0];
            genres = columns[1];
            trim(genres);
            trim(name);
        }

        size_t pos = genres.find("||");
        // If there is no "||" symbol.
        if((pos == string::npos) && (!genres.empty()) && (!all_of(genres.begin(), genres.end(), ::isspace))) {
            games_with_genre[name] = {genres};
            auto it = games_by_genre.find(genres);
            if (it == games_by_genre.end()) {
                // If the key does not exist, add it to the map with a new vector.
                games_by_genre[genres] = {name};
            } else {
                // If the key already exists, add the game to the existing vector.
                it->second.push_back(name);
            }
            numOfGames++;
        }
            // If the "||" symbol is present.
        else if ((pos != string::npos) && (!genres.empty()) && (!all_of(genres.begin(), genres.end(), ::isspace))) {
            numOfGames++;
            stringstream test(genres);
            string temp;
            while(getline(test, temp, '|')) {
                trim(temp);
                if (!temp.empty()) {
                    auto iter = games_with_genre.find(name);
                    if(iter == games_with_genre.end()) {
                        games_with_genre[name] = {temp};
                    }
                    else {
                        iter->second.push_back(temp);
                    }
                    // Check if the genre key already exists in the map.
                    auto it = games_by_genre.find(temp);
                    if (it == games_by_genre.end()) {
                        // If the key does not exist, add it to the map with a new vector.
                        games_by_genre[temp] = {name};
                    } else {
                        // If the key already exists, add the game to the existing vector.
                        it->second.push_back(name);
                    }
                }
            }
        }
        else {
            continue;
        }
    }

    // Close the file.
    file.close();

    // Menu
    int userInput;
    cout << "----------------------------------------------------------------------------------------------------" << endl;
    cout << "| Welcome to Video Game Finder!                                                                    |" << endl;
    cout << "|                                                                                                  |" << endl;
    cout << "| Let us find your next favorite game for you!                                                     |" << endl;
    cout << "| Just select from the following menu and follow the steps to a list of games we think you'd like! |" << endl;
    cout << "|                                                                                                  |" << endl;
    cout << "| 1: Find me a similar game to ...                                                                 |" << endl;
    cout << "| 2: Find me a similar game based on genre                                                         |" << endl;
    cout << "| 3: Print out all games                                                                           |" << endl;
    cout << "| 4: Quit                                                                                          |" << endl;
    cout << "----------------------------------------------------------------------------------------------------" << endl;
    cout << endl;

    while (userInput != 4) {
        cout << "-> ";
        cin >> userInput;
        cout << endl;

        if (userInput == 1) {
            cout << "Game: ";
            string game;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the cin input buffer.
            getline(cin, game);
            cout << endl;

            set<string> games;
            auto it = games_with_genre.find(game);
            if(it == games_with_genre.end()) {
                cout << game << " is not found in the database." << endl << endl;
            }
            else {
                cout << "Here is a list of games related to " << game << ":" << endl;
                for(int i = 0; i < it->second.size(); i++) {
                    string targetGenre = it->second[i];
                    auto iter = games_by_genre.find(targetGenre);
                    for(int j = 0; j < iter->second.size(); j++) {
                        if(iter->second[j] != game) {
                            games.insert(iter->second[j]);
                        }
                    }
                }
                int count = 0;
                for (auto it = games.begin(); it != games.end(); it++) {
                    cout << count + 1 << ": ";
                    cout << *it << endl;
                    count++;
                    if (count == 5) {
                        break;
                    }
                }
                cout << endl;
            }
        }
        else if (userInput == 2) {
            cout << "Genre: ";
            string genre;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, genre);
            cout << endl;

            auto it = games_by_genre.find(genre);
            if (it == games_by_genre.end()) {
                cout << genre << " is not found in the database." << endl;
            }
            else {
                cout << "Here is a list of games in the genre " << genre << ": " << endl;
                for (int i = 0; i < 5; i++) {
                    cout << i + 1 << ": ";
                    cout << it->second[i] << endl;
                }
            }
            cout << endl;
        }
        else if (userInput == 3) {
            cout << "Here is the duration of a BFS and DFS traversal of the graph: " << endl << endl;

            graph graph(games_by_genre);

            graph.convertGraph(games_by_genre);

            auto start = chrono::high_resolution_clock::now();
            graph.DFS("Pixel Gear", games_by_genre);
            auto end = chrono::high_resolution_clock::now();
            auto DFSTime = chrono::duration_cast<chrono::nanoseconds>(end-start).count();

            start = chrono::high_resolution_clock::now();
            graph.BFS("Pixel Gear", games_by_genre);
            end = chrono::high_resolution_clock::now();
            auto BFSTime = chrono::duration_cast<chrono::nanoseconds>(end-start).count();

            cout << "DFS Duration: " << DFSTime << " nanoseconds" << endl;
            cout << "BFS Duration: " << BFSTime << " nanoseconds" << endl;


            cout << endl;
        }
        else if (userInput == 4) {
            break;
        }
    }
    return 0;
}
