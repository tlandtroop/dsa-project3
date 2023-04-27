#include <iostream>
#include <array>
#include <chrono>
#include <vector>
#include <map>
#include <string>
using namespace std;

class graph {
private:
    bool** matrix; // Adjacency Matrix
    int size; // Total number of games
    map<string, int> nameToIndex; // Gives each genre and game an index starting at 0

public:
    graph(map<string, vector<string>>& map);
    ~graph();
    void convertGraph(const map<string, vector<string>>& map);
    void BFS(string start, const map<string, vector<string>> &gameMap);
    void DFS(string start, const map<string, vector<string>> &gameMap);
    string getGameName(int index, const map<string, vector<string>> &gameMap);
};
