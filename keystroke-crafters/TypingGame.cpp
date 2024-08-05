#include "httplib.h"
#include <iostream>
#include <vector>
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
    errorCount = 0;
    wordsaccuracy = 0.0;
    wordsperminute = 0.0;
    words_seen = 0;
    hash.initializeHash();
    for(int i = 0; i < 5; i++){
        words.push(hash.getElement());
    }
};

// Add a new player
void TypingGame::addPlayer(string& name, double typingSpeed, double accuracy) {
    Player newPlayer(name);
    players.push_back(newPlayer);
}
//Start the game
void TypingGame::startGame(){
    errorCount = 0;
    timerStarted = false;
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
//Getting the error count
int TypingGame::getErrorCount(){
    return errorCount;
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
    words_seen ++;
    words.pop();
}
//getnextword function
void TypingGame::getNextWord(){
    string a = hash.getElement();
    words.push(a);
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

void TypingGame::calculator(){
    double minutes = getTime() / 60;
    wordsperminute = words_seen / minutes;
    wordsaccuracy = (words_seen-3.0)/words_seen;
    
}

double TypingGame::getwpm() {
    return wordsperminute;
}

double TypingGame::getaccuracy() {
    return wordsaccuracy;
}
// Sort players based on performance
//void TypingGame::rankPlayersSpeed() {
//    mergeSortPlayers(0, players.size() - 1);
//
//    cout << "Ranked Players (by typing speed):\n";
//    for (const auto& player : players) {
//        cout << "Player ID: " << player.name << ", Typing Speed: " << player.typingSpeed << " WPM\n";
//    }
//}

// Merge sort algorithm to sort players
//void TypingGame::mergeSortPlayers(int left, int right) {
//    if (left < right) {
//        int mid = left + (right - left) / 2; // Find the middle point
//        mergeSortPlayers(left, mid); // Recursively sort first half
//        mergeSortPlayers(mid + 1, right); // Recursively sort the second half
//        merge(left, mid, right); // Merge the sorted halves
//    }
//}

// Merge sort helper function
//void TypingGame::merge(int left, int mid, int right) {
//    int n1 = mid - left + 1; // Size of left subarray
//    int n2 = right - mid; // Size of right subarray
//
//    // temporary sub arrays
//    vector<Player> L(n1);
//    vector<Player> R(n2);
//
//    // Copy data to temporary sub arrays
//    for (int i = 0; i < n1; ++i)
//        L[i] = players[left + i];
//    for (int i = 0; i < n2; ++i)
//        R[i] = players[mid + 1 + i];
//
//    // Merge the temporary sub arrays back into players[left..right]
//    int i = 0, j = 0, k = left;
//
//    // Merge the sub arrays based on typing speed in descending order
//    while (i < n1 && j < n2) {
//        if (L[i].typingSpeed >= R[j].typingSpeed) {
//            players[k] = L[i];
//            ++i;
//        } else {
//            players[k] = R[j];
//            ++j;
//        }
//        ++k;
//    }
//
//    // Copy the remaining elements of L[], if any
//    while (i < n1) {
//        players[k] = L[i];
//        ++i;
//        ++k;
//    }
//
//    // Copy the remaining element of R[], if any
//    while (j < n2) {
//        players[k] = R[j];
//        ++j;
//        ++k;
//    }
//}

//void TypingGame::rankPlayersAccuracy() {
//    heapSort(); // Call heap sort on the entire players vector
//
//    // Print the ranked players based on accuracy
//    cout << "Ranked Players (by accuracy):\n";
//    for (const auto& player : players) {
//        cout << "Player ID: " << player.name << ", Accuracy: " << player.accuracy << "%\n";
//    }
//}

//void TypingGame::heapSort() {
//    int n = players.size();
//
//    // Build heap
//    for (int i = n / 2 - 1; i >= 0; i--)
//        heapify(n, i);
//
//    // One by one extract elements from heap
//    for (int i = n - 1; i >= 0; i--) {
//        // Move current root to end
//        swap(players[0], players[i]);
//
//        // Call max heapify on the reduced heap
//        heapify(i, 0);
//    }
//
//    // Reverse the vector to get descending order
//    reverse(players.begin(), players.end());
//}
//
//void TypingGame::heapify(int n, int i) {
//    int largest = i; // Initialize largest as root
//    int left = 2 * i + 1; // Left child
//    int right = 2 * i + 2; // Right child
//
//    // If left child is larger than root
//    if (left < n && players[left].accuracy > players[largest].accuracy)
//        largest = left;
//
//    // If right child is larger than largest so far
//    if (right < n && players[right].accuracy > players[largest].accuracy)
//        largest = right;
//
//    // If largest is not root
//    if (largest != i) {
//        swap(players[i], players[largest]); // Swap root and largest
//
//        // Recursively heapify the subtree
//        heapify(n, largest);
//    }
//}