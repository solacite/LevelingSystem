// TestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;


// stats: username, level, exp
void displayStats(vector<string> stats) {
    int level;
    for (size_t i = 0; i < stats.size(); ++i) {
        size_t pos = stats[i].find(' ');
        if (pos != string::npos) {
            stats[i] = stats[i].substr(pos + 1);
        }
    }
    cout << "user: " << stats[0] << endl;
    cout << "level: " << stats[1] << endl;
    cout << "exp: (" << stats[2] << "/100)" << endl;
}

void initialize() {
    ofstream MyFile("stats.txt");
    MyFile << "Username: N/A\n";
    MyFile << "Level: 0\n";
    MyFile << "EXP: 0\n";
    MyFile.close();
}

vector<string> readStatistics() {
    vector<string> stats;
    ifstream readStats("stats.txt");
    string line;
    while (getline(readStats, line)) {
        //cout << line << endl;
        stats.push_back(line);
    }
    readStats.close();
    
    if (stats.empty()) {
        cout << "Initializing system..." << endl;
        initialize();
        cout << "Complete.";
        stats = readStatistics();
	}
    return stats;
}

int main() {
    string reply;
    string username;

    while (true) {
        vector<string> stats = readStatistics();
        displayStats(stats);
        cout << "test";
        cin >> reply;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file