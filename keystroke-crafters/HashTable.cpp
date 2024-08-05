#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <vector>
#include "httplib.h"

using namespace std;

struct Node{
  string data;
  Node* next;
  Node* prev;
  Node(string a){
    data = a;
    next = nullptr;
    prev = nullptr;
  }
};
class HashTable{
  private:
  int size;
  int capacity;
  vector<Node*> words;
  float loadFactor;
  float maxLoad;
  public:
  HashTable(){
      size = 0;
      capacity = 10;
      loadFactor = 0.0;
      maxLoad = 0.80;
      for (int i = 0; i < capacity; ++i) {
          words.push_back(nullptr);
      }
  };
    int hashFunction(string a){
        //hash function
        int index = 0;
        for(int i = 0; i < a.size(); i++){
            index += (int)a[i];
        }
        //reduce by %capacity
        index %= capacity;
        return index;
    }
  void updateLoad(){
    loadFactor = size/capacity;
    if(loadFactor >= maxLoad){ //rehash
        int oldCap = capacity;
        capacity *= 2;
        vector<Node*> newWords;
        for(int i = 0; i < capacity; i++){
            newWords.push_back(nullptr);
        }
        for(int i = 0; i < oldCap; i++){
            Node* current = words[i];
            while (current != nullptr){
                int index = hashFunction(current->data);
                if(newWords[index] == nullptr){
                    newWords[index] = new Node(current->data);
                } else{
                    Node* temp = newWords[index];
                    while (temp->next != nullptr) {
                        temp = temp->next;
                    }
                    temp->next = new Node(current->data);
                    temp->next->prev = temp;
                }
                current = current->next;
            }
        }
        words = newWords;
    }
  }

  void addElement(string a){
    int index = hashFunction(a);
    //try to add to that index in words, manage collsions if they happen
    if(words[index] == nullptr){
        words[index] = new Node(a);
    } else{
        Node* temp = words[index];
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = new Node(a);
        temp->next->prev = temp;
    }
    size++;
    updateLoad();
  }
  void initializeHash(){ //currently only adds first 10 words of database
        ifstream file("keystroke-crafters/google-books-common-words.txt");
        if (!file) {
            cout << "Failed to open file" << endl;
            return;
        }
        string line, word;
        getline(file, line);
        for(int i = 0; i < 95000; i++){
            word = line.substr(0,line.find("\t"));
            if(word.size() > 4){
                addElement(word);
            }
            getline(file, line);
        }
    }
  string getElement(){ //get random element from the hash table, removed it
      string a;
    //get random element
      random_device rd;
      mt19937 gen(rd());
      uniform_int_distribution<> dis(0, capacity);
      int num = dis(gen);
      while(words[num] == nullptr){
          num+=115; //increment at a high amount to ensure a greater variety of words
          num %= capacity;
      }
      Node* temp = words[num];
      a = temp->data;
      //remove random element in hash
      if (temp->prev != nullptr) {
          temp->prev->next = temp->next;
      } else {
          words[num] = temp->next;
      }
      if (temp->next != nullptr) {
          temp->next->prev = temp->prev;
      }
      delete temp;
      return a;
  }
};
