#include <iostream>
//#include <mutex>
#include <string>
#include "httplib.h"
#include "TypingGame.h"
//#include "json.hpp"

int main() {
    string user_option, game_option;
    TypingGame game;
    Player p;
    bool valid_entry = false;
    while (!valid_entry) {
        cout << "Welcome to Keytroke Crafters! Have you played before? (Enter a number)\n\t1)Yes\n\t2)No\n\t3)Exit\n";
        cin >> user_option;
        if (user_option == "1") {
            cout << "Amazing! Log in.";
            game.authenticate_user();
            valid_entry = true;
        } else if (user_option == "2") {
            cout << "Welcome! Create a name and ID.";
            game.new_user();
            valid_entry = true;
        } else if (user_option == "3") {
            return 0;
        } else {
            cout << "Invalid entry, try again" << endl;
        }
    }
    if (!game.start_game()){
        return 0;
    }
    string option2;
    while (game.start_game()) {
        cout << "Would you like to play again?\n\t1)Yes\n\t2)No\\n\\t3) Player Rankings\\n\\t4) More Information" << endl;
        cin >> option2;
        if (option2 == "1") {
            game.reset();
            valid_entry = true;
        } else if (option2 == "2") {
            cout << "Thank you for playing!" << endl;
            return 0;
        } else if (option2 == "3") {
            game.rankPlayersAccuracy();
            game.rankPlayersSpeed();
        } else if (option2 == "4") {
            cout <<"info on hashtables and bst's";
        }else {
                cout << "Invalid entry. Try again." << endl;
            }
    }
    return 0;
}

//using json = nlohmann::json;
//using namespace std;
//
//#define HTML_DIR "./public"
//
//int main() {
//    TypingGame game;
//    mutex gameMutex; // Mutex for thread safety
//
//    httplib::Server svr;
//
//    // Serve static files
//    svr.set_mount_point("/", HTML_DIR);
//
//    // Endpoint for handling the form submission and redirecting to the game page
//    svr.Post("/start-game", [&](const httplib::Request& req, httplib::Response& res) {
//        auto username = req.get_param_value("username");
//        cout << "We hit username " << username << endl;
//
//        double defaultTypingSpeed = 0.0;
//        double defaultAccuracy = 0.0;
//
//        {
//            lock_guard<mutex> guard(gameMutex);
//            game.addPlayer(username, defaultTypingSpeed, defaultAccuracy);
//        }
//
//        res.set_redirect("/gamepage.html");
//        cout << "We hit game page"<< endl;
//    });
//
//    svr.Post("/start-typing", [&](const httplib::Request& req, httplib::Response& res) {
//        json j;
//        j["words"] = game.getQueue();
//        res.set_content(j.dump(), "application/json");
//    });
//
//    svr.Post("/submit-word", [&](const httplib::Request& req, httplib::Response& res) {
//        auto body = json::parse(req.body);
//        string typedWord = body["typedWord"];
//        string currentWord = body["currentWord"];
//
//        bool isCorrect = game.checkWord(typedWord);
//        json j;
//        if (isCorrect) {
//            j["nextWord"] = game.getNextWord();
//        }
//        j["isCorrect"] = isCorrect;
//        res.set_content(j.dump(), "application/json");
//    });
//
//    cout << "Server started at http://localhost:8080" << endl;
//    svr.listen("localhost", 8080);
//
//    return 0;
//}
