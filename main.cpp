#include <httplib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <mutex>
#include "RankPlayer.cpp" // Ensure this file is in the correct directory and properly implemented
#include "json.hpp" // Include the nlohmann/json library

using json = nlohmann::json;
using namespace std;

struct Player {
    int id;
    double typingSpeed; // Words per Minute
    double accuracy; // Percentage
};

// TypingGame Class Definition
class TypingGame {
private:
    vector<Player> players; // Player data storage
    map<int, string> words; // Words dataset
    int nextId; // To keep track of the next player ID
    vector<string> wordList; // List of words to type
    int currentWordIndex; // Index of the current word

public:
    TypingGame() : nextId(1), currentWordIndex(0) {
        // Initialize word list
        wordList = {"example", "typing", "words", "game"};
    }

    void addPlayer(const string& name, double typingSpeed, double accuracy) {
        Player newPlayer = { nextId++, typingSpeed, accuracy };
        players.push_back(newPlayer);
    }

    string getNextWord() {
        if (currentWordIndex >= wordList.size()) {
            currentWordIndex = 0; // Reset index if it exceeds the word list size
        }
        return wordList[currentWordIndex++];
    }

    void handleTyping(const string& typedWord, double& speed, double& accuracy) {
        // Calculate typing speed and accuracy
        speed = typedWord.length() * 12.0; // Simplified WPM calculation
        accuracy = 100.0; // Simplified accuracy calculation (placeholder)
    }

    // Methods for sorting players by speed and accuracy
    void rankPlayersSpeed();
    void rankPlayersAccuracy();
    void mergeSortPlayers(int left, int right);
    void merge(int left, int mid, int right);
    void heapify(int n, int i);
    void heapSort();
};

bool checkKey(string input, string output) {
    if (input.size() != output.size()) {
        return false;
    }
    for (int i = 0; i < input.size(); i++) {
        if (input[i] != output[i]) {
            return false;
        }
    }
    return true;
}

void reset(int &eCount, float &wCount) {
    // Reset the board to initial conditions
    eCount = 0;
    wCount = 0;
}

string nextWord() {
    // Placeholder function for generating the next word
    string a = "example";
    return a;
}

int main() {
    TypingGame game;
    mutex gameMutex; // Mutex for thread safety

    httplib::Server svr;

    svr.Post("/start-game", [&](const httplib::Request& req, httplib::Response& res) {
        auto username = req.get_param_value("username");

        double defaultTypingSpeed = 0.0;
        double defaultAccuracy = 0.0;

        {
            lock_guard<mutex> guard(gameMutex);
            game.addPlayer(username, defaultTypingSpeed, defaultAccuracy);
        }
        res.set_redirect("/gamepage.html");
    });

    svr.Get("/start-typing", [&](const httplib::Request& req, httplib::Response& res) {
        string word;
        {
            lock_guard<mutex> guard(gameMutex);
            word = game.getNextWord();
        }
        json response = {{"word", word}};
        res.set_content(response.dump(), "application/json");
    });

    svr.Post("/submit-typing", [&](const httplib::Request& req, httplib::Response& res) {
        auto j = json::parse(req.body);
        string typedWord = j["word"];

        double speed, accuracy;
        {
            lock_guard<mutex> guard(gameMutex);
            game.handleTyping(typedWord, speed, accuracy);
        }

        string nextWord;
        {
            lock_guard<mutex> guard(gameMutex);
            nextWord = game.getNextWord();
        }

        json response = {{"nextWord", nextWord}};
        res.set_content(response.dump(), "application/json");
    });

    // Serve static files (like the game page)
    svr.set_base_dir(".");

    cout << "Server started at http://localhost:8080" << endl;
    svr.listen("localhost", 8080);

    return 0;
}
