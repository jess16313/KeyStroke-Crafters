#include "httplib.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <queue>
#include <chrono>
#include <fstream>
#include "TypingGame.h"
using namespace std;

// Data Structure Definitions

// Constructor
TypingGame::TypingGame(){
    timerStarted = false;
    words_seen = 0;
    errorCount = 0;
    loadPlayerfromFile();

};
//Add a new player
void TypingGame::new_user() {
    std::string username, id;
    bool goodname = false, goodid = false;

    while (!goodname) {
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, username);
        goodname = true;
        for (char c : username) {
            if (!std::isalpha(static_cast<unsigned char>(c)) && c != ' ') {
                std::cout << "Invalid name entry. Only use letters or spaces. Try again." << std::endl;
                goodname = false;
                break;
            }
        }
    }

    while (!goodid) {
        cout << "Enter any set of four numbers, that will be your ID: ";
        cin >> id;
        bool uniqueid = true;

        for (const auto& user : users) {
            if (user.id == id) {
                uniqueid = false;
                break;
            }
        }

        if (id.length() != 4 || !std::all_of(id.begin(), id.end(), ::isdigit)) {
            std::cout << "Invalid ID entry. IDs must be 4 digits long. Try again." << std::endl;
        } else if (!uniqueid) {
            std::cout << "ID is not unique. Enter a new ID." << std::endl;
        } else {
            goodid = true;
        }
    }

    users.push_back(Player(username, id, 0.0, 0.0));
    currplayer = &users.back();
}

void TypingGame::loadPlayerfromFile() {
    ifstream file("players.txt");
    if (!file.is_open()){
        cerr << "You made an oopsie" << endl;
    }
    string name, id;
    double speed, accuracy;
    while (file>> name>> id >> speed >> accuracy){
        std::cout << "Reading player: " << name << " " << id << " " << speed << " " << accuracy << endl;
        users.push_back(Player(name, id, speed, accuracy));
    }
    file.close();
}

void TypingGame::pushPlayertoFile() {
    ofstream file("players.txt");
    if (!file.is_open()) {
        cerr << "error opening this";
        return;
    }
        for (const auto& player: users) {
            file << player.name << " " << player.id << " " << player.typingSpeed << " " << player.accuracy << "\n";
        }

        file.close();
}

//Authenticate an old player
void TypingGame::authenticate_user() {
    std::string username, id;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter ID: ";
    std::cin >> id;

    bool authenticated = false;
    for (auto& user : users) {
        if (user.name == username && user.id == id) {
            authenticated = true;
            currplayer = &user;
            break;
        }
    }

    if (authenticated) {
        std::cout << "Welcome back, " << username << "!" << std::endl;
        return;
    } else {
        std::string answer;
        while (true) {
            std::cout << "Have you played before? If you have, type 1 to try again! If not, type 2 to make an account." << std::endl;
            std::cin >> answer;
            if (answer == "1") {
                authenticate_user(); // Re-enter authentication process
                return;
            } else if (answer == "2") {
                new_user(); // Create new user
                return;
            } else {
                std::cout << "Invalid entry. Please try again." << std::endl;
            }
        }
    }
}

void TypingGame::reset(){
    timerStarted = false;
    errorCount = 0;
    cout<< "Game has been reset!" << endl;
    start_game();
}

bool TypingGame::start_game() {
    timerStarted = false;
    words_seen = 0;
    errorCount = 0;
    std::string game_option;
    bool valid_entry = false;

    while (!valid_entry) {
        std::cout << "Please select an option: \n\t1) Hash Table\n\t2) BST\n\t3) Player Rankings\n\t4) More Information\n\t5) Exit\n";
        std::cin >> game_option;
        if (game_option == "5") {
            std::cout << "Thank you for playing!" << std::endl;
            currplayer = nullptr;
            return false;
        } else if (game_option == "4") {
            std::cout << "Hashtables:\n\tA data structure that stores key and value pairs and allows "
                         "\ndata to be retrieved quickly. Insert/Search/Delete time complexity is O(n) worst case."
                         "\nBinary Search Trees:\n\t A hierarchical data structure where each node has two "
                         "\nsubnodes refered to as leaves. Insert/Search/Delete time complexity is O(n)." << std::endl;
        } else if (game_option == "3") {
            rankPlayersSpeed();
            rankPlayersAccuracy();
        } else if (game_option != "1" && game_option != "2") {
            std::cout << "Invalid selection, try again." << std::endl;
        } else {
            valid_entry = true;
            std::cout << "To begin the typing game, please turn your caps lock on." << std::endl;
        }
    }

    valid_entry = false;
    option = game_option;
    if (option == "1") {
        auto start = std::chrono::high_resolution_clock::now();
        hash.initializeHash();
        for (int i = 0; i < 5; i++) {
            words.push(hash.getElement());
        }
        auto end = std::chrono::high_resolution_clock::now();
        alg_runtime = std::chrono::duration<double, std::micro>(end - start).count();
    } else if (option == "2") {
        auto start = std::chrono::high_resolution_clock::now();
        bst.initializeBST();
        for (int i = 0; i < 5; i++) {
            words.push(bst.getElement());
        }
        auto end = std::chrono::high_resolution_clock::now();
        alg_runtime = std::chrono::duration<double, std::micro>(end - start).count();
    }

    while (errorCount < 3) {
        std::cout << "\nPlease type the first word.\t";
        printQueue();
        std::string input;
        std::cin >> input;
        checkWord(input);
        getNextWord();
    }

    std::pair<double, double> stats = calculator();
    std::cout << "Thanks for playing, " << currplayer->name << "! \nYou typed " << currplayer->typingSpeed << " words per minute and had " << currplayer->accuracy << "% accuracy.\nThe algorithm runtime was " << alg_runtime << " microseconds." << endl;
    if (currplayer->typingSpeed < stats.first) {
        std::cout << "Congratulations, you have a new highest speed!" << endl;
    }
    if (currplayer->accuracy < stats.second) {
        std::cout << "Congratulations, you have a new highest accuracy" << endl;
    }
    currplayer->typingSpeed = stats.first;
    currplayer->accuracy = stats.second;
    pushPlayertoFile(); // Save updated player stats to file
    return true;
}
//Start the timer
void TypingGame::startTimer(){
    startTime = chrono::steady_clock::now();
    timerStarted = true;
}
//Stop the timer
void TypingGame::stopTimer(){
    timerStarted = false;
}
//Get the time it took for the player to make three mistakes
double TypingGame::getTime(){
    if (timerStarted){
        stopTimer();
        chrono::duration<double> elapsed = chrono::steady_clock::now() - startTime;
        return elapsed.count();
    }
    return 0;
}

//Check if the word was correctly typed
void TypingGame::checkWord(string& typedword){
    if (!timerStarted){
        startTimer();
    }

    if (typedword != words.front()){
        errorCount++;
        cout << "Error count:" << errorCount << endl;
    }
    words_seen++;
    words.pop();
}
//getnextword function
void TypingGame::getNextWord(){
    if(option == "1") {
        words.push(hash.getElement());
    }else if(option == "2"){
        words.push(bst.getElement());
     }
}
//getindexcount funciton
queue<string> TypingGame::getQueue(){
    return words;
}
void TypingGame::printQueue(){
    cout << "The words you will be typing are: " << endl;
    queue<string> temp = words;
    for(int i = 0; i < 5; i++){
        cout << temp.front() << " ";
        temp.pop();
    }
    cout << "\n";
}

pair<double,double> TypingGame::calculator(){
    double minutes = getTime() / 60;
    double wordsperminute, wordsaccuracy = 0.0;
    wordsperminute = words_seen / minutes;
    wordsaccuracy = ((words_seen-3.0)/words_seen) * 100;
    return pair(wordsperminute,wordsaccuracy);
}

double TypingGame::getalg_runtime() {
    return alg_runtime;
}

//Sort players based on performance
void TypingGame::rankPlayersSpeed() {
    mergeSortPlayers(0, users.size() - 1);

    cout << "Ranked Players (by typing speed):\n";
    for (const auto& player : users) {
        cout << "Player ID: " << player.name << ", Typing Speed: " << player.typingSpeed << " WPM\n";
    }
}

//Merge sort algorithm to sort users
void TypingGame::mergeSortPlayers(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2; // Find the middle point
        mergeSortPlayers(left, mid); // Recursively sort first half
        mergeSortPlayers(mid + 1, right); // Recursively sort the second half
        merge(left, mid, right); // Merge the sorted halves
    }
}

//Merge sort helper function
void TypingGame::merge(int left, int mid, int right) {
    int n1 = mid - left + 1; // Size of left subarray
    int n2 = right - mid; // Size of right subarray

    // temporary sub arrays
    vector<Player> L(n1);
    vector<Player> R(n2);

    // Copy data to temporary sub arrays
    for (int i = 0; i < n1; ++i)
        L[i] = users[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = users[mid + 1 + i];

    // Merge the temporary sub arrays back into users[left..right]
    int i = 0, j = 0, k = left;

    // Merge the sub arrays based on typing speed in descending order
    while (i < n1 && j < n2) {
        if (L[i].typingSpeed >= R[j].typingSpeed) {
            users[k] = L[i];
            ++i;
        } else {
            users[k] = R[j];
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        users[k] = L[i];
        ++i;
        ++k;
    }

    // Copy the remaining element of R[], if any
    while (j < n2) {
        users[k] = R[j];
        ++j;
        ++k;
    }
}

void TypingGame::rankPlayersAccuracy() {
    heapSort(); // Call heap sort on the entire users vector

    // Print the ranked users based on accuracy
    cout << "Ranked users (by accuracy):\n";
    for (const auto& player : users) {
        cout << "Player ID: " << player.name << ", Accuracy: " << player.accuracy << "%\n";
    }
}

void TypingGame::heapSort() {
    int n = users.size();

    // Build heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(n, i);

    // One by one extract elements from heap
    for (int i = n - 1; i >= 0; i--) {
        // Move current root to end
        swap(users[0], users[i]);

        // Call max heapify on the reduced heap
        heapify(i, 0);
    }

    // Reverse the vector to get descending order
    reverse(users.begin(), users.end());
}

void TypingGame::heapify(int n, int i) {
    int largest = i; // Initialize largest as root
    int left = 2 * i + 1; // Left child
    int right = 2 * i + 2; // Right child

    // If left child is larger than root
    if (left < n && users[left].accuracy > users[largest].accuracy)
        largest = left;

    // If right child is larger than largest so far
    if (right < n && users[right].accuracy > users[largest].accuracy)
        largest = right;

    // If largest is not root
    if (largest != i) {
        swap(users[i], users[largest]); // Swap root and largest

        // Recursively heapify the subtree
        heapify(n, largest);
    }
}