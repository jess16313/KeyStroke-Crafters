#include <httplib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <mutex>
#include <chrono>
#include "RankPlayer.cpp"
#include "HashTable.cpp" 
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
    queue<string> words; // Words pulled from data structure
    HashTable hash;
    chrono::steady_clock::time_point startTime;
    bool timerStarted;
    int errorcount;
    
public:
    TypingGame();
    void addPlayer(string name, double typingSpeed, double accuracy);
    void startGame();
    void checkWord(const string& typedword);
    void rankPlayersSpeed(); // Sort players based on speed (merge sort)
    void rankPlayersAccuracy(); // Sort players based on accuracy (heap sort)
    void mergeSortPlayers(int left, int right);
    void merge(int left, int mid, int right);
    void heapify(int n, int i);
    void heapSort();
    void startTimer();
    void stopTimer();
    double getTime();
    int getErrorCount();
    queue<string> getQueue();
    //string nextWord(hash obj)
    //return hash.getElement()
    //string nextWord(bst obj)
    //reset function
};
// Constructor
TypingGame::TypingGame()  : errorCount(0), timerStarted(false){
    hash.initializeHash();
    HashTable hashTable;
    hashTable.addWord("example");
    hashTable.addWord("typing");
    hashTable.addWord("words");
    hashTable.addWord("game");
    hashTable.addWord("test");
    auto allWords = hashTable.getAllWords();
    for (const auto& word : allWords) {
        words.push(word);
    }
};
// Add a new player
void TypingGame::addPlayer(string name, double typingSpeed, double accuracy) {
    Player newPlayer = { name, typingSpeed, accuracy };
    players.push_back(newPlayer);
}
//Start the game 
void startGame(){
    errorCount = 0;
    timerStarted = false;
}
//Start the timer
void startTimer(){
    startTime = chrono::steady_clock::now();
    timerStarted = true;
}
//Stop the timer
void stopTimer(){
    timerStarted = false;
}
//Get the time it took for the player to make three mistakes
double getTime(){
    if (timerstarted){
        chrono::duration<double> elapsed = chrono::steady_clock::now() - startTime;
        return elapsed.count();
    }
    return 0;
}
//Getting the error count
int getErrorCount(){
    return errorcount;
}
//Check if the word was correctly typed
bool checkWord(const string& typedword){
    if (!timerStarted){
        starttimer();
    }

    if (typedword == words.front()){
        words.pop();
        return true;
    } else{
        errorcount++;
        if (errorcount >= 3){
            stopTimer();
        }
        return false;
    }
}
//getnetwordfunction
string getNextWord(){
    return words.front();
}
//getindexcount funciton
queue<string> getQueue(){
    vector<string> wordQueue;
    queue<string> temp = words;

    while (!temp.empthy()){
        wordQueue.push_back(temp.front());
        temp.pop();
    }
    return wordqueue;
}
// Sort players based on performance
void TypingGame::rankPlayersSpeed() {
    mergeSortPlayers(0, players.size() - 1);
    cout << "Ranked Players (by typing speed):\n";
    for (const auto& player : players) {
        cout << "Player ID: " << player.name << ", Typing Speed: " << player.typingSpeed << " WPM\n";
    }
}
// Merge sort algorithm to sort players
void TypingGame::mergeSortPlayers(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2; // Find the middle point
        mergeSortPlayers(left, mid); // Recursively sort first half
        mergeSortPlayers(mid + 1, right); // Recursively sort the second half
        merge(left, mid, right); // Merge the sorted halves
    }
}
// Merge sort helper function
void TypingGame::merge(int left, int mid, int right) {
    int n1 = mid - left + 1; // Size of left subarray
    int n2 = right - mid; // Size of right subarray
    // temporary sub arrays
    vector<Player> L(n1);
    vector<Player> R(n2);
    // Copy data to temporary sub arrays
    for (int i = 0; i < n1; ++i)
        L[i] = players[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = players[mid + 1 + i];
    // Merge the temporary sub arrays back into players[left..right]
    int i = 0, j = 0, k = left;
    // Merge the sub arrays based on typing speed in descending order
    while (i < n1 && j < n2) {
        if (L[i].typingSpeed >= R[j].typingSpeed) {
            players[k] = L[i];
            ++i;
        } else {
            players[k] = R[j];
            ++j;
        }
        ++k;
    }
    // Copy the remaining elements of L[], if any
    while (i < n1) {
        players[k] = L[i];
        ++i;
        ++k;
    }
    // Copy the remaining element of R[], if any
    while (j < n2) {
        players[k] = R[j];
        ++j;
        ++k;
    }
}
void TypingGame::rankPlayersAccuracy() {
    heapSort(); // Call heap sort on the entire players vector
    // Print the ranked players based on accuracy
    cout << "Ranked Players (by accuracy):\n";
    for (const auto& player : players) {
        cout << "Player ID: " << player.id << ", Accuracy: " << player.accuracy << "%\n";
    }
}
void TypingGame::heapSort() {
    int n = players.size();
    // Build heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(n, i);
    // One by one extract elements from heap
    for (int i = n - 1; i >= 0; i--) {
        // Move current root to end
        swap(players[0], players[i]);
    int largest = i; // Initialize largest as root
    int left = 2 * i + 1; // Left child
    int right = 2 * i + 2; // Right child
    // If left child is larger than root
    if (left < n && players[left].accuracy > players[largest].accuracy)
        largest = left;
    // If right child is larger than largest so far
    if (right < n && players[right].accuracy > players[largest].accuracy)
        largest = right;
    // If largest is not root
    if (largest != i) {
        swap(players[i], players[largest]); // Swap root and largest
        // Recursively heapify the subtree
        heapify(n, largest);
    }
}

int main() {
    TypingGame game;
    mutex gameMutex; // Mutex for thread safety

    httplib::Server svr;

      svr.set_post_routing_handler([](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    });

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
            game.startGame();
            wordQueue = game.getQueue();
            }
        json response = {{"words", wordQueue}};
        cout << "Sending words: " << response.dump() << endl;
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
            elapsedTime = game.getElapsedTime();
            wordQueue = game.getWordQueue();
        }

        json response = {
            {"correct", correct},
            {"words", wordQueue},
            {"errorCount", errorCount},
            {"elapsedTime", elapsedTime}
        };
         cout << "Response: " << response.dump() << endl;
        res.set_content(response.dump(), "application/json");
    });

    // Serve static files (like the game page)
    svr.set_base_dir(".");

    cout << "Server started at http://localhost:8080" << endl;
    svr.listen("localhost", 8080);

    return 0;
}