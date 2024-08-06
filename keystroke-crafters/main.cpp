#include <iostream>
#include <string>
#include "TypingGame.h"

int main() {
    string user_option, game_option;
    TypingGame game;
    Player p;
    bool valid_entry = false;
    while (!valid_entry) {
        cout << "Welcome to Keytroke Crafters! Have you played before? (Enter a number)\n\t1) Yes\n\t2) No\n\t3) Exit\n";
        cin >> user_option;
        if (user_option == "1") {
            cout << "Amazing! Log in. ";
            game.authenticate_user();
            valid_entry = true;
        } else if (user_option == "2") {
            cout << "Welcome! Create a name and ID. ";
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
        cout << "Would you like to play again?\n\t1) Yes\n\t2) No\n\t3) Player Rankings\n\t4) More Information" << endl;
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