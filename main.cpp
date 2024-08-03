#include <httplib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <mutex>
#include "RankPlayer.cpp" 
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
    
public:
    TypingGame();
    void addPlayer(int id, double typingSpeed, double accuracy);
    void rankPlayersSpeed(); // Sort players based on speed (merge sort)
    void rankPlayersAccuracy(); // Sort players based on accuracy (heap sort)
    void mergeSortPlayers(int left, int right);
    void merge(int left, int mid, int right);
    void heapify(int n, int i);
    void heapSort();
};
// Constructor
TypingGame::TypingGame() = default;
// Add a new player
void TypingGame::addPlayer(int id, double typingSpeed, double accuracy) {
    Player newPlayer = { id, typingSpeed, accuracy };
    players.push_back(newPlayer);
}
// Sort players based on performance
void TypingGame::rankPlayersSpeed() {
    mergeSortPlayers(0, players.size() - 1);
    cout << "Ranked Players (by typing speed):\n";
    for (const auto& player : players) {
        cout << "Player ID: " << player.id << ", Typing Speed: " << player.typingSpeed << " WPM\n";
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

    svr.Post("/Keystroke-Crafters/gamepage.html", [&](const httplib::Request& req, httplib::Response& res) {
        auto username = req.get_param_value("username");

        double defaultTypingSpeed = 0.0;
        double defaultAccuracy = 0.0;

        {
            lock_guard<mutex> guard(gameMutex);
            game.addPlayer(username, defaultTypingSpeed, defaultAccuracy);
        }
        res.set_redirect("/Keystroke-Crafters/gamepage.html");
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
