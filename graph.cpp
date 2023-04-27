#include "graph.h"
#include <queue>
#include <stack>
using namespace std;

// Constructor
graph::graph(map<string, vector<string>>& map) {
    // Make the private variable size equal to the parameter size
    size = map.size();
    // Create a 2D array based on the number of games in the map
    matrix = new bool*[size];
    for(int i = 0; i < size; i++) {
        matrix[i] = new bool[size];
        for(int j = 0; j < size; j++) {
            matrix[i][j] = false;
        }
    }
}

// Destructor
graph::~graph() {
    for(int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


void graph::convertGraph(const map<string, vector<string>> &genresMap) {
    // Start the index off at one
    int index = 0;
    for (auto genre : genresMap) {
        nameToIndex[genre.first] = index;
        index++;
    }

    for(auto genre : genresMap) {
        int i = nameToIndex[genre.first];
        for(auto game : genre.second) {
            // Check if the game has already been given a number
            // auto check = nameToIndex.find(genre.second);
            // if(check == gameIndex.end()) {
            //   gameIndex[it->second[i]] = index;
            //   index++;
            // }
            int j = nameToIndex[game];
            matrix[i][j] = true;
            matrix[j][i] = true;
        }
    }

    // for(auto it = genresMap.begin(); it != genresMap.end(); it++) {
    // }
}

void graph::BFS(string start, const map<string, vector<string>> &gameMap) {
    int startIndex = nameToIndex[start];
    bool* visited = new bool[size];

    for (int i = 0; i < size; i++) {
        visited[i] = false;
    }

    queue<int> q;
    visited[startIndex] = true;
    q.push(startIndex);

    while (!q.empty()) {
        int front = q.front();
        q.pop();
        auto games = gameMap.find(getGameName(front, gameMap));
        if (games != gameMap.end()) {
            for (auto game : games->second) {
                if (matrix[front][nameToIndex[game]]) {
                    continue;
                    //cout << game << ", ";
                }
            }
        }
        for (int i = 0; i < size; i++) {
            if (matrix[front][i] && !visited[i]) {
                visited[i] = true;
                q.push(i);
            }
        }
    }
    delete[] visited;
}

void graph::DFS(string start, const map<string, vector<string>> &gameMap) {
    bool* visited = new bool[size];

    for (int i = 0; i < size; i++) {
        visited[i] = false;
    }

    stack<int> st;
    int startIndex = nameToIndex[start];
    visited[startIndex] = true;
    st.push(startIndex);

    while (!st.empty()) {
        int top = st.top();
        st.pop();
        auto games = gameMap.find(getGameName(top, gameMap));
        if (games != gameMap.end()) {
            for (auto game : games->second) {
                if (matrix[top][nameToIndex[game]]) {
                    continue;
                }
            }
        }
        for (int i = 0; i < size; i++) {
            if (matrix[top][i] && !visited[i]) {
                visited[i] = true;
                st.push(i);
            }
        }
    }
    delete[] visited;
}

string graph::getGameName(int index, const map<string, vector<string>> &gameMap) {
    for (auto genre : nameToIndex) {
        if (genre.second == index) {
            return genre.first;
        }
        if (gameMap.find(genre.first) != gameMap.end()) {
            auto games = gameMap.at(genre.first);
            for (auto game : games) {
                if (nameToIndex.at(game) == index) {
                    return game;
                }
            }
        }
    }
    return "";
}
