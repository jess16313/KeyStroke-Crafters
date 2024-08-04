#include <iostream>
#include <string>
#include <mutex>
#include "httplib.h"
#include "RankPlayer.cpp"
#include "HashTable.cpp" 
#include "json.hpp" // Include the nlohmann/json library

using json = nlohmann::json;
using namespace std;


// Constructor
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

    // Endpoint for starting the game and fetching the first word
    svr.Get("/start-typing", [&](const httplib::Request& req, httplib::Response& res) {
        vector<string> wordQueue;
        {
            lock_guard<mutex> guard(gameMutex);
            wordQueue = game.getQueue();
            game.startGame(); // Start game when fetching the first word
        }
        json response = {{"words", wordQueue}};
        res.set_content(response.dump(), "application/json");
    });

    // Endpoint for submitting a typed word
    svr.Post("/submit-word", [&](const httplib::Request& req, httplib::Response& res) {
        auto j = json::parse(req.body);
        string typedWord = j["typedWord"];

        bool correct;
        int errorCount;
        double elapsedTime;
        vector<string> wordQueue;
        {
            lock_guard<mutex> guard(gameMutex);
            correct = game.checkWord(typedWord);
            errorCount = game.getErrorCount();
            elapsedTime = game.getTime();
            wordQueue = game.getQueue();
        }

        json response = {
                {"correct", correct},
                {"words", wordQueue},
                {"errorCount", errorCount},
                {"elapsedTime", elapsedTime}
        };
        res.set_content(response.dump(), "application/json");
    });

    // Serve static files (like the game page)
    svr.set_base_dir(".");

    cout << "Server started at http://localhost:8080" << endl;
    svr.listen("localhost", 8080);

    return 0;
}