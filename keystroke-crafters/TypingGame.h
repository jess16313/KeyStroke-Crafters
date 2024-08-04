//
// Created by Jess on 8/4/2024.
//
#include "httplib.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include "HashTable.cpp"
#ifndef KEYSTROKE_CRAFTERS_RANKPLAYER_H
#define KEYSTROKE_CRAFTERS_RANKPLAYER_H
using namespace std;

struct Player {
    string name;
    double typingSpeed; // Words per Minute
    double accuracy; // Percentage
};

// TypingGame Class Definition
class TypingGame    {
private:
    vector<Player> players; // Player data storage
    queue<string> words; // Words pulled from data structure
    HashTable hash;
    chrono::steady_clock::time_point startTime;
    bool timerStarted;
    int errorCount;

public:
    TypingGame();
    void addPlayer(string& name, double typingSpeed, double accuracy);
    void startGame();
    bool checkWord(const string& typedword);
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
    vector<string> getQueue();
    string getNextWord();
    //return hash.getElement()
    //string nextWord(bst obj)
    //reset function
};

#endif //KEYSTROKE_CRAFTERS_RANKPLAYER_H
