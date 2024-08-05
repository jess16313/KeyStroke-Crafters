#include "httplib.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <queue>
#include <chrono>
#include "TypingGame.h"
using namespace std;

// Data Structure Definitions

// Constructor
TypingGame::TypingGame(){
    timerStarted = false;
    words_seen = 0;
    errorCount = 0;
    option;
    if(this->option == "1"){
        hash.initializeHash();
        for(int i = 0; i < 5; i++){
            words.push(hash.getElement());
        }
    } else if(this->option == "2"){
        bst.initializeBST();
        for(int i = 0; i < 5; i++){
            words.push(bst.getElement());
        }
    }

};
//Add a new player
void TypingGame::new_user() {
    string username, id;
    bool goodname,goodid = false;
    while (!goodname) {
        cout << "Enter your name:";
        cin >> username;
        goodname = true;
        for (char c: username) {
            if (!isalpha(c)) {
                cout << "Invalid name entry. Only use letters. Try again";
                goodname = false;
                break;
            }
        }
    }
    while (!goodid) {
        bool uniqueid = true;
        cout << "Enter any set of four numbers, that will be your ID:";
        cin >> id;
        for (const auto& user : users){
            if (user.name == id){
               uniqueid = false;
            }
        }
        if (id.length() != 4 || !std::all_of(id.begin(), id.end(), ::isdigit)) {
            cout << "Invalid id entry, try again";
        }
        else if(!uniqueid){
            cout << "Id is not unique. Enter a new id";
        }
        else{
            goodid = true;
        }
    }
    users.push_back(Player(username, id, 0.0, 0.0)); // Add new player to players vector
    currplayer = &users.back();
}

//Authenticate an old player
void TypingGame::authenticate_user(){
    string username, id;
    Player p;
    cout << "Enter username:";
    cin >> username;
    cout << "Enter ID:";
    cin >> id;
    bool authenticated = false;
    for (auto& user : users){
        if (user.name == username && user.id == id){
            authenticated = true;
            currplayer = &(user);
            break;
        }
    }

    if (authenticated){
        return;
    }
    else{
        string answer;
        while (answer != "1" || answer != "2") {
            cout << "Have u played before if yes try again if no make an account" << endl;

            cin >> answer;
            if (answer == "1") {
                authenticate_user();
            }
            if (answer == "2") {
                new_user();
            }
        }
    }
}
//// Add a new player
//void TypingGame::addPlayer(string& name, double typingSpeed, double accuracy) {
//    Player newPlayer(name);
//    players.push_back(newPlayer);
//}
//Start the game
void TypingGame::reset(){
    timerStarted = false;
    errorCount = 0;
    cout<< "Game has been reset!" << endl;
    start_game();
}
bool TypingGame::start_game(){
    timerStarted = false;
    words_seen = 0;
    errorCount = 0;
    string game_option;
    bool valid_entry = false;
    while (!valid_entry) {
        cout << "Please select an option: \n\t1) Hash Table\n\t2) BST\n\t3) Player Rankings\n\t4) More Information\n\t5)Exit" << endl;
        cin >> game_option;
        if (game_option == "5") {
            cout << "Thank you for playing!" << endl;
            return false;
        } else if (game_option == "4") {
            cout << "Info on Hashtables and BST's" << endl;
        } else if(game_option == "3") {

        }else if (game_option != "1" && game_option != "2") {
            cout << "Invalid selection, try again." << endl;
        }
        else {
            valid_entry = true;
            cout << "To begin the typing game, please turn your caps lock on." << endl;
        }
    }
    valid_entry = false;
    option = game_option;
    if(option == "1"){
        hash.initializeHash();
        for(int i = 0; i < 5; i++){
            words.push(hash.getElement());
        }
    } else if(option == "2"){
        bst.initializeBST();
        for(int i = 0; i < 5; i++){
            words.push(bst.getElement());
        }
    }
    while(errorCount < 3){
        cout << "\nPlease type the first word.\t";
        printQueue();
        string input;
        cin >> input;
        checkWord(input);
        getNextWord();
    }
    pair<double,double> stats;
    stats = calculator();
    if (currplayer->typingSpeed < stats.first){
        cout <<" Congratulations, you have a new highest speed!" << endl;
        currplayer->typingSpeed = stats.first;
    }
    if (currplayer->accuracy < stats.second){
        cout << "Congratulations, you have a new highest accuracy" << endl;
        currplayer->accuracy = stats.second;
    }
    else{
        currplayer->typingSpeed = stats.first;
        currplayer->accuracy = stats.second;
    }

    cout << "Thanks for playing, " << currplayer->name << "! \nYou typed " << currplayer->typingSpeed << " words per minute and had " << currplayer->accuracy << "% accuracy.\nThe algorithm runtime was " << getalg_runtime() << " microseconds." <<endl;
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
        cout << "error count:" << errorCount << endl;
    }
    words_seen++;
    words.pop();
}
//getnextword function
void TypingGame::getNextWord(){
    if(option == "1"){
        auto start = std::chrono::high_resolution_clock::now();
        words.push(hash.getElement());
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std:: micro> alg_runtime = end - start;
    } else if(option == "2"){
        auto start = std::chrono::high_resolution_clock::now();
        words.push(bst.getElement());
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std:: micro> alg_runtime = end - start;
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