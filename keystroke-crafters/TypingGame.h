//
// Created by Jess on 8/4/2024.
//
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <queue>
#include "HashTable.cpp"
#include "BST.cpp"
#ifndef KEYSTROKE_CRAFTERS_RANKPLAYER_H
#define KEYSTROKE_CRAFTERS_RANKPLAYER_H
using namespace std;

struct Player {
    string name;
    string id;
    double typingSpeed; // Words per Minute
    double accuracy; // Percentage
    Player(){
        name = "";
        id = "";
        typingSpeed = 0.0;
        accuracy = 0.0;
    }
    Player(string name, string id, double speed, double typeaccuracy){
        this->name = name;
        this->id = id;
        this->typingSpeed = speed;
        this->accuracy = typeaccuracy;
    }
};

// TypingGame Class Definition
class TypingGame    {
private:
    string option;
    queue<string> words; // Words pulled from data structure
    HashTable hash;
    BST bst;
    chrono::steady_clock::time_point startTime;
    Player* currplayer;
    bool timerStarted;
    int errorCount;
    int words_seen;
    double alg_runtime;
    vector<Player> users;
    void mergeSortPlayers(int left, int right);
    void merge(int left, int mid, int right);
    void heapify(int n, int i);
    void heapSort();
    void startTimer();
    void stopTimer();
    double getTime();
    void printQueue();
    void getNextWord();
    double getalg_runtime();
public:
    TypingGame();
    void loadPlayerfromFile();
    void pushPlayertoFile();
    void new_user();
    void authenticate_user();
    void checkWord(string& typedword);
    pair<double,double> calculator();
    void rankPlayersSpeed(); // Sort players based on speed (merge sort)
    void rankPlayersAccuracy(); // Sort players based on accuracy (heap sort)
    bool start_game();
    void reset();

};

#endif //KEYSTROKE_CRAFTERS_RANKPLAYER_H
