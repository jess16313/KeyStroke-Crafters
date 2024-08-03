#include <iostream>

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
}
class HashTable{
  private:
  Node words[10]; //may have to be a node to do single chain linking for collisions
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
  };
  void updateLoad(){
    loadFactor = size/capacity;
    //if loadFactor >= maxLoad rehash
  }
  void addElement(string a){
    //hash function
    int index = 0;
    for(char& c : a){
      index += (int)c;
    }
    //reduce by %10
    index %= 10;
    //try to add to that index in words, manage collsions if they happen
    if(words[index] == nullptr){
      words[index] = new Node(a);
    } else{
      Node* temp = words[index];
      while(temp != nullptr){
        temp = temp->next;
        if(temp == nullptr){
          temp = new Node(a);
          break;
        }
      }
    }
    size++;
    updateLoad();
  }
  string getElement(){ //get random element from the hash table, removed it 
    string a;
    return a;
  }
};
