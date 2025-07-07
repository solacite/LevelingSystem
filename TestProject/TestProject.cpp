// TestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;


// stats: username, level, exp
// STR AGI STA PER INT
void displayPlayerInfo(vector<string> stats) {
    int section = 0;
    for (size_t i = 0; i < stats.size(); ++i) {
        if (stats[i] == "---") {
            section++;
            continue;
        }
        size_t pos = stats[i].find(' ');
        if (pos != string::npos) {
            stats[i] = stats[i].substr(pos + 1);
        }
    }
    int level = stoi(stats[1]);
    int expToNextLevel = 100 + (level * 100);
    cout << "user: " << stats[0] << endl;
    cout << "level: " << stats[1] << endl;
    cout << "exp: (" << stats[2] << "/" << expToNextLevel << ")" << endl;
}

void initialize() {
    ofstream MyFile("stats.txt");
    ifstream MyReadFile("stats template.txt");
    string line;
    while (getline(MyReadFile, line)) {
        MyFile << line << endl;
	}
    MyFile.close();
}

vector<string> readFile(string filename) {
    vector<string> lines;
    ifstream MyReadFile(filename);
    string line;
    while (getline(MyReadFile, line)) {
        //cout << line << endl;
        lines.push_back(line);
    }
    MyReadFile.close();
    return lines;
}

vector<string> readPlayerInfo() {
    vector<string> stats = readFile("stats.txt");
    
    if (stats.empty()) {
        cout << "Initializing system..." << endl;
        initialize();
        cout << "Complete.";
        stats = readPlayerInfo();
	}
    return stats;
}

int main() {
    string reply;
    string username;

    while (true) {
        vector<string> playerInfo = readPlayerInfo();
        displayPlayerInfo(playerInfo);
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