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

static void updatePlayerInfo(vector<string> playerInfo, vector<string> playerStats) {
    vector<string> newInfo;
    newInfo.push_back("---"); // First separator
    newInfo.push_back("Username: " + playerInfo[0]);
    newInfo.push_back("Level: " + playerInfo[1]);
    newInfo.push_back("EXP: " + playerInfo[2]);
    newInfo.push_back("---"); // Second separator
    newInfo.push_back("STR: " + playerStats[0]);
    newInfo.push_back("AGI: " + playerStats[1]);
    newInfo.push_back("STA: " + playerStats[2]);
    newInfo.push_back("PER: " + playerStats[3]);
    newInfo.push_back("INT: " + playerStats[4]);
    newInfo.push_back("Points: " + playerStats[5]);
    writeToFile("stats.txt", newInfo);
}

static pair<vector<string>, vector<string>> parsePlayerInfo(const vector<string>& rawPlayerInfo) {
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
                playerInfo.push_back(value);
            }
            else if (section == 2) {
                playerStats.push_back(value);
            }
        }
    }
    return make_pair(playerInfo, playerStats);
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
    int EXP = stoi(playerInfo[2]);
    int expToNextLevel = 100 + (level * 5);

    if (EXP >= expToNextLevel) {
        level++;
        EXP -= expToNextLevel;
        playerInfo[1] = to_string(level);
        playerInfo[2] = to_string(EXP);
        playerStats[5] = to_string(stoi(playerStats[5]) + 3); // Increase points by 3 on level up

        // Now save everything, including updated points
        updatePlayerInfo(playerInfo, playerStats);
	}

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
            cout << "Would you like to allocate points to STR (0), AGI (1), STA (2), PER (3), or INT (4)?\n" << endl;
            cin >> stat;
			cout << "How many points would you like to allocate?\n" << endl;
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
				updatePlayerInfo(playerInfo, playerStats);
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
    cout << "1. Tasks" << endl;
    cout << "2. View player info" << endl;
    cout << "3. Settings" << endl;
    cout << "4. Exit" << endl;
}

static void changeUsername() {
    string newUsername;
    cout << "Enter new username: ";
    cin >> newUsername;

    vector<string> rawPlayerInfo = readFile("stats.txt");
    auto parsed = parsePlayerInfo(rawPlayerInfo);
    vector<string> playerInfo = parsed.first;
    vector<string> playerStats = parsed.second;

    if (playerInfo.size() < 3 || playerStats.size() < 6) {
        cout << "Error: Player info or stats are missing or corrupted." << endl;
        return;
    }

    playerInfo[0] = newUsername; // Update username in memory
    updatePlayerInfo(playerInfo, playerStats); // Write back using the same logic

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

static void tasks() {
    cout << "TASKS" << endl << endl;
    cout << "1. Submit a task" << endl;
    cout << "2. Back to main menu" << endl;
    string choice;
    cout << endl;
    cin >> choice;
    system("cls");
    if (choice == "1") {
        cout << "Task difficulty 1-5?" << endl;
        string difficulty;
        cout << endl;
        cin >> difficulty;
        int expGain = 5 * stoi(difficulty);

        // Parse player info and stats
        vector<string> rawPlayerInfo = readFile("stats.txt");
        auto parsed = parsePlayerInfo(rawPlayerInfo);
        vector<string> playerInfo = parsed.first;
        vector<string> playerStats = parsed.second;

        if (playerInfo.size() < 3 || playerStats.size() < 6) {
            cout << "Error: Player info or stats are missing or corrupted." << endl;
            return;
        }

        // Add expGain to EXP
        int currentExp = stoi(playerInfo[2]);
        currentExp += expGain;
        playerInfo[2] = to_string(currentExp);

        // Save updated info
        updatePlayerInfo(playerInfo, playerStats);

        cout << "\nYou gained " << expGain << " EXP!" << endl;
        this_thread::sleep_for(std::chrono::seconds(1));
        system("cls");
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
        if (choice == "1") {
            tasks();
        }
        if (choice == "2") {
			displayPlayerInfo(rawPlayerInfo, false);
        }
        else if (choice == "3") {
            settings();
        }
        else if (choice == "4") {
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