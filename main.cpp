#include <iostream>
#include <string>
#include <mutex>
#include "httplib.h"
#include "RankPlayer.cpp"
#include "HashTable.cpp" 
#include "json.hpp" // Include the nlohmann/json library

using json = nlohmann::json;
using namespace std;
int main() {
    TypingGame game;
    mutex gameMutex; // Mutex for thread safety

    httplib::Server svr;

    // Endpoint for handling the form submission and redirecting to the game page
    svr.Post("/start-game", [&](const httplib::Request& req, httplib::Response& res) {
        auto username = req.get_param_value("username");

        double defaultTypingSpeed = 0.0;
        double defaultAccuracy = 0.0;

        {
            lock_guard<mutex> guard(gameMutex);
            game.addPlayer(username, defaultTypingSpeed, defaultAccuracy);
        }
        res.set_redirect("/Keystroke-Crafters/gamepage.html");
    });

    svr.Post("/start-typing", [&](const httplib::Request& req, httplib::Response& res) {
        json j;
        j["words"] = game.getInitialWords();
        res.set_content(j.dump(), "application/json");
    });

    svr.Post("/submit-word", [&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        string typedWord = body["typedWord"];
        string currentWord = body["currentWord"];

        bool isCorrect = game.checkWord(typedWord, currentWord);
        json j;
        if (isCorrect) {
            j["nextWord"] = game.getNextWord();
        }
        j["isCorrect"] = isCorrect;
        res.set_content(j.dump(), "application/json");
    });

    // Serve static files
    svr.set_base_dir(".");

    cout << "Server started at http://localhost:8080" << endl;
    svr.listen("localhost", 8080);

    return 0;
}