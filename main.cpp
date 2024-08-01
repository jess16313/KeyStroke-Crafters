#include <iostream>
#include <string>
//#include BST, Hash Table, and Rank Player files
using namespace std;

bool checkKey(string input, string output){
  if(input.size() != output.size()){
    return false;
  }
  for(int i = 0; i < input.size(); i++) {
    if(input[i] != output[i}){
      return false;
    }
  }
  return true;
}

void reset(int &eCount, float &wCount){
  //reset the board to return to initial conditions
  eCount = 0;
  wCount = 0;
}

string nextWord(){ //need to have 2 types, one with a BST parameter and 1 with a hash table parameter
  string a;
  return a;
}

int main() {
  bool running = true;
  while(running){
    bool gameOver = false;
    int errorCount = 0;
    float wordCount = 0;
    pair <string, vector<float>> player;
    cout << "Enter your name" << endl;
    string name;
    cin >> name;
    cout << "Please select which data structure you would like to use.\n\t1. BST\n\t2. Hash Table\n\t3. Exit" << endl;
    string option;
    cin >> option;
    player.first = name;
    if(option == "1"){
      //define a bst object
      //create the game board
    } else if(option == "2"){
      //define a hash table object
      //create the game board
    } else if(opiton == "3"){
      running = false;
      break;
    }
    while(!gameOver){
        if(errorCount == 3){
          gameOver = true;
          //player.second push back their time taken
          player.second.push_back(wordCount/(wordCount+3));
          reset(errorCount, wordCount);
          break;
        }
        string output; //set equal to the root of the node
        string input;
        cin << input;
        if(!checkKey(input, output)){
          errorCount++;
        } else{
          wordCount++;
        }
        //get new word for output
    }
    //results screen
    //add player to heap and sort (call function from inserted Rank Player file
    //display results
    //option to start again
  }
  return 0;
}
