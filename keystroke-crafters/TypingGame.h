//
// Created by Jess on 8/4/2024.
//
#include "httplib.h"
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
    double typingSpeed; // Words per Minute
    double accuracy; // Percentage
    Player(string a){
        name = a;
        typingSpeed = 0.0;
        accuracy = 0.0;
    }
};

// TypingGame Class Definition
class TypingGame    {
private:
    string option;
    vector<Player> players; // Player data storage
    queue<string> words; // Words pulled from data structure
    HashTable hash;
    BST bst;
    chrono::steady_clock::time_point startTime;
    bool timerStarted;
    int errorCount;
    int words_seen;
    double wordsperminute;
    double wordsaccuracy;

public:
    TypingGame(string option);
    void addPlayer(string& name, double typingSpeed, double accuracy);
    void startGame();
    void checkWord(string& typedword);
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
    void printQueue();
    void getNextWord();
    void calculator();
    double getwpm();
    double getaccuracy();
    //reset function
};

#endif //KEYSTROKE_CRAFTERS_RANKPLAYER_H
