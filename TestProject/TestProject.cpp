// TestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
using namespace std;


// This function writes player info to a file.
static void writeToFile(string filename, vector<string> playerInfo) {
    ofstream MyFile(filename);
    for (const auto& line : playerInfo) {
        MyFile << line << endl;
    }
    MyFile.close();
}

// stats: username, level, exp
// STR AGI STA PER INT
static void displayPlayerInfo(vector<string> rawPlayerInfo, bool displayType) {
	// If displayType is true, display the player info for a menu. Otherwise, display both player info and stats
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
    cout << "Username: " << playerInfo[0] << endl;
    cout << "Level: " << playerInfo[1] << endl;
    cout << "EXP: (" << playerInfo[2] << "/" << expToNextLevel << ")" << endl;

    cout << endl;
    
    if (!displayType) {
        cout << "| STR: " << playerStats[0] << endl;
        cout << "| AGI: " << playerStats[1] << endl;
        cout << "| STA: " << playerStats[2] << endl;
        cout << "| PER: " << playerStats[3] << endl;
        cout << "| INT: " << playerStats[4] << endl;
        cout << "\nPoints: " << playerStats[5] << endl;

        cout << "\n1. Allocate Points" << endl;
        cout << "2. Back to main menu" << endl;
        string choice;
        cout << endl;
        cin >> choice;
        system("cls");
        if (choice == "1") {
            string stat;
            int points;
            cout << "Would you like to allocate points to STR (0), AGI (1), STA (2), PER (3), or INT (4)?" << endl;
            cin >> stat;
			cout << "How many points would you like to allocate?" << endl;
            cin >> points;
            if (points > stoi(playerStats[5])) {
                cout << "You do not have enough points to allocate." << endl;
                this_thread::sleep_for(std::chrono::seconds(1));
                system("cls");
                return;
            }
            else {
				playerStats[stoi(stat)] = to_string(stoi(playerStats[stoi(stat)]) + points);
                playerStats[5] = to_string(stoi(playerStats[5]) - points);
                vector<string> newFileContent;
                size_t i = 0;

                // Section 0: Copy lines until first "---"
                while (i < rawPlayerInfo.size() && rawPlayerInfo[i] != "---") {
                    newFileContent.push_back(rawPlayerInfo[i]);
                    ++i;
                }
                if (i < rawPlayerInfo.size()) {
                    newFileContent.push_back("---");
                }
                ++i;

                // Section 1: Player info (with keys)
                for (size_t j = 0; j < playerInfo.size(); ++j) {
                    string key = rawPlayerInfo[i + j].substr(0, rawPlayerInfo[i + j].find(' ') + 1);
                    newFileContent.push_back(key + playerInfo[j]);
				}

                i += playerInfo.size();
                if (i < rawPlayerInfo.size()) {
                    newFileContent.push_back("---");
                }
                ++i;

                // Section 2: Player stats (with keys)
                for (size_t j = 0; j < playerStats.size(); ++j) {
                    string key = rawPlayerInfo[i + j].substr(0, rawPlayerInfo[i + j].find(' ') + 1);
                    newFileContent.push_back(key + playerStats[j]);
                }

                writeToFile("stats.txt", newFileContent);
                this_thread::sleep_for(std::chrono::seconds(1));
				system("cls");
            }
        }
        else if (choice == "2") {
            return; // Go back to main menu
        }
    }
}

static void initialize() {
    ofstream MyFile("stats.txt");
    ifstream MyReadFile("stats template.txt");
    string line;
    while (getline(MyReadFile, line)) {
        MyFile << line << endl;
	}
    MyFile.close();

    ofstream MyFile2("tasks.txt");
    ifstream MyReadFile2("tasks template.txt");
    string line2;
    while (getline(MyReadFile2, line2)) {
        MyFile2 << line2 << endl;
    }
    MyFile2.close();
}

// This function reads a file and returns its content as a vector of strings.
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

// This function reads player info from the file and initializes it if the file is empty.
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

static void displayMenu() {
	cout << "1. Open quest log" << endl;
    cout << "2. View tasks" << endl;
    cout << "3. View player info" << endl;
    cout << "4. Settings" << endl;
    cout << "5. Exit" << endl;
}

static void changeUsername() {
    string newUsername;
    cout << "Enter new username: ";
    cin >> newUsername;

    vector<string> rawPlayerInfo = readFile("stats.txt");
    for (size_t i = 0; i < rawPlayerInfo.size(); ++i) {
        size_t pos = rawPlayerInfo[i].find("Username: ");
        if (pos != string::npos) {
            rawPlayerInfo[i] = "Username: " + newUsername;
        }
    }
	writeToFile("stats.txt", rawPlayerInfo);
    cout << "Username changed to: " << newUsername << endl;
    this_thread::sleep_for(std::chrono::seconds(1));
    system("cls");
}

static void settings() {
    cout << "SETTINGS" << endl << endl;
    cout << "1. Change username" << endl;
    cout << "2. Back to main menu" << endl;
    string choice;
    cout << endl;
    cin >> choice;
    system("cls");
    if (choice == "1") {
        changeUsername();
    }
    else if (choice == "2") {
        return; // Go back to main menu
    }
}

int main() {
    string choice;
    string username;

    while (true) {
        vector<string> rawPlayerInfo = readPlayerInfo();
        displayPlayerInfo(rawPlayerInfo, true);

        displayMenu();
        cout << endl;
		cin >> choice;
        system("cls");
        if (choice == "3") {
			displayPlayerInfo(rawPlayerInfo, false);
        }
        else if (choice == "4") {
            settings();
        }
        else {
			return 0; // Exit the program
        }
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