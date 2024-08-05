#include <iostream>
//#include <mutex>
#include <string>
#include "httplib.h"
#include "TypingGame.h"
//#include "json.hpp"

int main() {
    string name, option;
    cout << "Please enter a username: " << endl;
    cin >> name;
    cout << "Please select an option: \n\t1) Hash Table\n\t2) BST\n\t3) Exit" << endl;
    cin >> option;
    if(option == "3") {
        cout << "Thank you for playing!" << endl;
        return 0;
    } else if(option != "1" || option != "2"){
        cout << "Invalid selection, exiting now." << endl;
        return 0;
    }
    TypingGame game(option);
    cout << "To begin the typing game, please turn your caps lock on." << endl;
    while(game.getErrorCount() < 3){
        game.printQueue();
        cout << "\nPlease type the first word.\t";
        string input;
        cin >> input;
        game.checkWord(input);
        game.getNextWord();
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
