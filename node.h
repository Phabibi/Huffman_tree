#include <iostream>
#include <ostream>

class Node{

private:
  Node* left = NULL;
  Node* right = NULL;

  int freq; 
  int bit =0;
  char ch;
  //only used for joint_prob
  char ch2;

public: 
  //constructor for an empty node
  
  Node() {this->left = NULL; this->right = NULL;};

  Node(int f, char c) {this->ch = c; this->freq = f;  this->left = NULL; this->right=NULL; };

  //constructor for joint probabilities
  Node(int f, char c1, char c2) {this->freq = f; this->ch = c1; this->ch2 = c2; this->left = NULL; this->right = NULL;}; 

  //Setter and Getters
  //
  void set_left(Node* l) {this->left = l;};
  void set_right(Node* r) {this->right = r;};
  void set_bit(int b) {this->bit = b;};
  void set_freq(int f) {this->freq = f;};
  void set_ch(char ch) {this->ch = ch;};
  void set_ch_pair(char c1, char c2) {this->ch = c1; this->ch2 = c2;};

  Node* get_left() const {return this->left;};
  Node* get_right() const {return this->right;};
  
  int get_bit() const {return this->bit;};
  int get_freq() const {return this->freq;};
  char get_ch() const{return this->ch;};
  char get_ch2() const{return this->ch2;};

  //return the pair
  std::pair<char,char> get_ch_pair() const{return {this->ch, this->ch2};};


};

// This class is mad so that we can compare two Nodes with eachoether

class Node_compare{

public:
  //overloading the Greater class to compare for the minheap based on the frequency 
  int operator() (const Node &n1,  const Node &n2)
  {
    return n1.get_freq() > n2.get_freq(); 
  } 

};

//This class is used to overwrite the hash code for joint probabilies
//with this struct we can haver the
class hash_pair {
  
public:
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};


// over writting == for two pairs, since P(a,b) == p(b,a) 
// this will bring the run time from o(n^2) to O(n) since we are hashing the probabilities
// thus we dont need to go through our list again to calculate the two probabilites  to construct the probabillity set ,it will only need to get done once O(n)
//

//bool operator == (const std::pair<char,char>& l, const std::pair<char,char>& r)
//  {
//
//       return (l.first == r.second && l.second == r.second);
//  }

