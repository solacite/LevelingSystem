// TestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;


// stats: username, level, exp
// STR AGI STA PER INT
static void displayPlayerInfo(vector<string> rawPlayerInfo) {
    vector<string> playerInfo;
    vector<string> playerStats;
    int section = 0;
    for (size_t i = 0; i < rawPlayerInfo.size(); ++i) {
        if (rawPlayerInfo[i] == "---") {
            section++;
            continue;
        }
        size_t pos = rawPlayerInfo[i].find(' ');
        if (pos != string::npos) {
            string value = rawPlayerInfo[i].substr(pos + 1);
            if (section == 1) {
                playerInfo.push_back(move(value));
			}
            else if (section == 2) {
				playerStats.push_back(move(value));
            }
        }
    }

    int level = stoi(playerInfo[1]);
    int expToNextLevel = 100 + (level * 100);
    cout << "user: " << playerInfo[0] << endl;
    cout << "level: " << playerInfo[1] << endl;
    cout << "exp: (" << playerInfo[2] << "/" << expToNextLevel << ")" << endl;

    cout << endl;
	cout << "STR: " << playerStats[0] << endl;
    cout << "AGI: " << playerStats[1] << endl;
    cout << "STA: " << playerStats[2] << endl;
    cout << "PER: " << playerStats[3] << endl;
    cout << "INT: " << playerStats[4] << endl;
	cout << "\n//\n\n";
}

static void initialize() {
    ofstream MyFile("stats.txt");
    ifstream MyReadFile("stats template.txt");
    string line;
    while (getline(MyReadFile, line)) {
        MyFile << line << endl;
	}
    MyFile.close();
}

static vector<string> readFile(string filename) {
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

static vector<string> readPlayerInfo() {
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
        vector<string> rawPlayerInfo = readPlayerInfo();
        displayPlayerInfo(rawPlayerInfo);
		cout << "What is your username? ";
		cin >> reply;
        system("cls");
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