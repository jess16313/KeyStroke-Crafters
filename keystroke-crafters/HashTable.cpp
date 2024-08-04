#include <iostream>
#include <random>
#include <string>
#include <fstream>
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
  Node* words[10]; //may have to be a node to do single chain linking for collisions
  int size;
  int capacity;
  float loadFactor;
  float maxLoad;
  public:
  HashTable(){
      size = 0;
      capacity = 10;
      loadFactor = 0.0;
      maxLoad = 0.80;
      for (int i = 0; i < capacity; ++i) {
          words[i] = nullptr;
      }
  };
    int hashFunction(string a){
        //hash function
        int index = 0;
        for(char& c : a){
            index += (int)c;
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
        Node* newWords[capacity];
        for(int i = 0; i < capacity; ++i){
            newWords[i] = nullptr;
        }
        for(int i = 0; i < oldCap; ++i){
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
        ifstream file("google-books-common-words.txt");
        string word;
        getline(file, word);
        for(int i = 0; i < 10; i++){
            word = word.substr(0,word.find(' '));
            addElement(word);
            getline(file, word);
        }
    }
  string getElement(){ //get random element from the hash table, removed it
      string a;
    //get random element
      random_device rd;
      mt19937 gen(rd());
      uniform_int_distribution<> dis(0, capacity);
      int num = dis(gen);
      while(words[num] != nullptr){
          num++;
          num %= capacity;
      }
      Node* temp = words[num];
      a = temp->data;
      //remove random element in hash
      temp = temp->next;
      temp->prev = nullptr;
      return a;
  }
};
