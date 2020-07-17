#include <iostream>
#include <ostream>
#include <math.h>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include "node.h"

//Author: Parsa Habibi
//Email:phabibi@sfu.ca
//CMPT-365
//HUFFMAN TREE FOR SINGLE AND JOINT PROBABILITEIS

/*   --------------------HELPERS--------------------------*/

//make a template T so you can print floats and ints
template<class T>

void print_map(std::unordered_map<std::pair<char,char> , T , hash_pair> map)
{
  for(auto p : map)
  {
    std::cout << p.first.first << "," <<  p.first.second << " : " << p.second << std::endl; 
  }

}

template<class T>
void print_map(std::unordered_map<char,T> map)
{
  for(auto p : map)
  {
    std::cout << p.first << " : " << p.second << std::endl; 
  }

}

void print_queue(std::priority_queue <Node, std::vector<Node>, Node_compare> pq){

  while(!pq.empty())
  {
    std::cout << pq.top().get_ch() << " : " << pq.top().get_freq() << " -> " << pq.top().get_bit() << std::endl;
    pq.pop();
  }
}

/*   -----------------------------------------------------*/

/* ----------------------- HUFFMAN TREE ------------------*/

//generates the huff man codings and prints the tree 
void generate_huff_code_joint(Node* root, int bits[], int c, std::unordered_map<std::pair<char,char>,std::string, hash_pair>* bit_len)
{

  if(root->get_left())
  {
    bits[c] = 0;
    generate_huff_code_joint(root->get_left(), bits ,c+1, bit_len );
  }

  if(root->get_right())
  {
    bits[c] = 1;
    generate_huff_code_joint(root->get_right(), bits , c+1, bit_len);
  }
  else if(!root->get_right() && !root->get_left()) 
  {
    int count = 0; 
    int seen1 = 0;
    int seen0 = 0;

     std::cout<< root->get_ch() << "," << root->get_ch2() << " : ";
     std::string s = "";
     for(int i = 0 ; i < c; i++){
        std::cout<<bits[i];
        s += std::to_string(bits[i]);
    }
     std::cout << std::endl;
     std::pair<char,char> pair = root->get_ch_pair();
     bit_len->insert({pair,s});
  }
}
void generate_huff_code(Node* root, int bits[], int c, std::unordered_map<char,std::string>* bit_len)
{

  if(root->get_left())
  {
    bits[c] = 0;
    //std::cout<<"left see: " << root->get_left()->get_ch() << std::endl;
    
    //recurse more we have a left
    generate_huff_code(root->get_left(), bits ,c+1, bit_len );
    
  }

  if(root->get_right())
  {
    bits[c] = 1;
    //std::cout<<"right see: " << root->get_right()->get_ch() << " : " << root->get_bit() << std::endl;
    
    //recurese right
    generate_huff_code(root->get_right(), bits , c+1, bit_len);
    
  }

  //BASE CASE
  else if(!root->get_right() && !root->get_left()) 
  {
     std::cout<< root->get_ch() << " : ";
     
     std::string s = "";

     for(int i = 0 ; i < c; i++){
        std::cout<<bits[i];
        s += std::to_string(bits[i]);
    }

     std::cout << std::endl;
     bit_len->insert({ root->get_ch(),s});
  }
}


float generate_huff(std::unordered_map<char,int> first_prob , std::unordered_map<char,float> map)
{

  //make a min Heap with the frequency of each letter as the priority
  std::priority_queue <Node, std::vector<Node>, Node_compare> pq;

  // 1. make first_prob.size() nodes (as many as unique chars in our sentence) insert into the min heap
  // While size is not 1 (we have a single root) 
     // 2. extract the top 2 with lowest frequencies
     // 3. create a new Node and add the frequencies together and insert back in the Heap
     // 4. repeat 2 and 3
  // 5. once the tree is constructed, then do a traverse to assign the bits to each node
 
  //populate the heap
  for (auto p : first_prob)
  {
     Node leaf = Node(p.second, p.first);
     pq.push(leaf); 
  }

  // print_queue(pq);
 
  //keep track of the roots
  int count = 0;
  //std::cout << "the heap size init is : " << pq.size()  << std::endl;
  
  while(pq.size() > 1)
  {
    //pop the top 2 lowest freq
    Node l1 = Node();
    l1 =  pq.top();
    // std::cout << "TOP 1 : " << l1.get_ch() << " : " << l1.get_bit() << std::endl;
    pq.pop();
    
    Node l2 = Node();
    l2 = pq.top();
    //std::cout << "TOP 2 : " << l2.get_ch() << " : " << l2.get_bit() << std::endl;
    pq.pop();

    int new_freq = l1.get_freq() + l2.get_freq();
   
    //make a new internal node
    Node root = Node(new_freq ,'r');
    
    //assign left
    Node* l1n = new Node(l1.get_freq() , l1.get_ch());
    l1n->set_left(l1.get_left());
    l1n->set_right(l1.get_right());

    //assign right
    Node* l2n = new Node(l2.get_freq() , l2.get_ch());
    l2n->set_left(l2.get_left());
    l2n->set_right(l2.get_right());
    
    //insert to HEAP
    root.set_left(l1n);
    root.set_right(l2n);
    root.set_bit(++count);

    //std::cout << "setting left as : " << root.get_left()->get_ch() << " bit : " << root.get_left()->get_bit() << std::endl; 
    //std::cout << "setting right as : " << root.get_right()->get_ch() << " bit : " << root.get_right()->get_bit() << std::endl; 

    pq.push(root);
    // std::cout << "the heap size is : " << pq.size()  << std::endl;
  }
  
  //bit count arr
  int v[32] = {'\0'} ;
  //map to store the bits in
  std::unordered_map<char,std::string> bit_len;
  std::unordered_map<char,int> avg_lens;
 
  
  //print_queue(pq);
 
  //make a new root* to pass to our huffcode 
  Node* root = new Node();
  root->set_left(pq.top().get_left());
  root->set_right(pq.top().get_right());

  //GET THE HUFFING CODE 
  generate_huff_code(root ,v,0, &bit_len);
  float avg_bit = 0.0;

  
  std::cout << " BIT LENGTHS " << std::endl;
  for(auto p : bit_len)
  {
      avg_lens[p.first] = (float)p.second.size();
  }
  avg_bit =0.0;
  std::cout << std::endl;
  print_map(avg_lens);

  for(auto p : avg_lens)
  {
    //we can actually do this in 1 for loop since our original map has the same keys as our bitlength map
     avg_bit += map[p.first] * p.second;
  }

  

  return avg_bit;

}

//Does the exact thing as above but for joint probabilities
float generate_huff_joint(std::unordered_map<std::pair<char,char>,int, hash_pair > second_prob , std::unordered_map<std::pair<char,char>,float,hash_pair> map) {

  std::priority_queue <Node, std::vector<Node>, Node_compare> pq;

  //populate the heap
  for (auto p : second_prob)
  {
     Node leaf = Node(p.second, p.first.first,p.first.second);
     pq.push(leaf); 
  }

 
  //keep track of the roots
  int count = 0;
  while(pq.size() > 1)
  {
    //pop the top 2 lowest freq
    Node l1 = Node();
    l1 =  pq.top();

    pq.pop();
    
    Node l2 = Node();
    l2 = pq.top();
    pq.pop();

    int new_freq = l1.get_freq() + l2.get_freq();
   
    //make a new internal node
    Node root = Node(new_freq ,'r');
    
    //assign left
    Node* l1n = new Node(l1.get_freq() , l1.get_ch(), l1.get_ch2());
    l1n->set_left(l1.get_left());
    l1n->set_right(l1.get_right());

    //assign right
    Node* l2n = new Node(l2.get_freq() , l2.get_ch(), l2.get_ch2());
    l2n->set_left(l2.get_left());
    l2n->set_right(l2.get_right());
    
    //insert to HEAP
    root.set_left(l1n);
    root.set_right(l2n);
    root.set_bit(++count);

    pq.push(root);
  }
  
  //bit count arr
  int v[32] = {'\0'} ;
  std::unordered_map<std::pair<char,char>,std::string, hash_pair> bit_len;
  std::unordered_map<std::pair<char,char>,float,hash_pair> avg_lens;
  
  //make a new root* to pass to our huffcode 
  Node* root = new Node();
  root->set_left(pq.top().get_left());
  root->set_right(pq.top().get_right());

  //GET THE HUFFING CODE 
  generate_huff_code_joint(root ,v,0, &bit_len);
  
  std::cout << std::endl << "BIT LENGTHS" << std::endl;
  std::vector<int> bit_vec;
  float avg_bit = 0.0;

  for(auto p : bit_len)
  {
     std::pair<char,char> pair = {p.first.first,p.first.second};
     avg_lens[pair] = (float)p.second.size();
  
  }
  print_map(avg_lens);
  for(auto p : avg_lens)
  {
    //we can actually do this in 1 for loop since our original map has the same keys as our bitlength map
     std::pair<char,char> pair = {p.first.first , p.first.second};
     avg_bit += map[pair]  * p.second;
  }
  return avg_bit;

}

/* ---------------------------------------------------------------------------- */


/* -------------------- Probabilities and Entropies ---------------------------*/

//calculates the first entropy
float first_entropy(std::unordered_map<char,float> first_prob){
  //first order entropy = Pi log2(1/Pi) where i->N
  float f_order = 0.0;

  for(auto p : first_prob)
  {
     f_order += p.second * log2(1/(p.second));    
  }

  return f_order;

}

//constructs a generic map with the chars and their frequencies
std::unordered_map<char,int>  construct_map(std::string &s){

  //inital map with the frequency of each char as its value
  std::unordered_map<char,int> map;

  for(auto c : s)
  {
    //insert the chars into map with the number of times they occur as map.second
    map[c] += 1;
  }
  print_map(map);
  return map;
}

//constructs the map of single probablities
std::unordered_map<char,float> construct_single_prob(std::unordered_map<char,int> map ,std::string &s)
{ 

  std::unordered_map<char,float> single_prob;

  for(auto p : map)
  {
    single_prob.insert({ p.first, (float) p.second/s.size() });
  } 

  print_map(single_prob); 
  return single_prob;
}

//construct a set for the joint probabilites in the following format { KEY: {char,char} , VAL:P(Char)*P(char)}
std::unordered_map<std::pair<char,char> , int, hash_pair>  construct_joint_set(std::string s)
{
  // we first want to make a map of pairs <char,char> : val using our custom hashing class
  std::unordered_map<std::pair<char,char> ,int ,hash_pair> map; 


  if(s.size() % 2 != 0) 
  {
     throw std::runtime_error("the string does not have even number of chars");
  
  }

  //we want to grab every pair
  int p1 = 0; 
  int p2 = 1;
  
  while(p1 < s.size() && p2 < s.size())
  {
    std::pair<char,char> p = {s[p1] , s[p2] };
    map[p] += 1;
    p1 += 2;
    p2 += 2;
  }

  print_map(map);
  return map;
}

//construct the joint probabilities.
std::unordered_map <std::pair<char,char> , float, hash_pair>
construct_joint_prob(std::unordered_map<std::pair<char,char>, int, hash_pair> map, std::unordered_map<char,float> single_map)
{
  std::unordered_map<std::pair<char,char> , float, hash_pair> joint_prob;

  float n = 0.0;
  //get the size of the string as if the joints represented a single char
  for(auto p : map)
  {
     n += p.second;
  }
  for(auto p : map)
  {
    std::pair<char,char> pair = {p.first.first, p.first.second};
    // treat a pair as a letter 
    joint_prob[pair] = (float)map[pair] / n;; 
  }
  print_map(joint_prob);
  return joint_prob;
}

float second_entropy(std::unordered_map<std::pair<char,char>, float, hash_pair> joint_prob )
{

  float s_entropy = 0.0;
  for(auto p : joint_prob)
  {
    std::pair<char,char> pair = {p.first.first , p.first.second};
    //dont need to do this twice if weve seen it 
    s_entropy += joint_prob[pair] * log2(1/joint_prob[pair]); 
  }

  return s_entropy;
}



/* -----------------------------------------------------------------------------------*/
void output(std::string input)
{
  /* single probabilities*/
  
  std::cout << "Sequence  = " << input << std::endl<<std::endl;

  std::cout << "------------------------------------------------------" << std::endl;
  std::cout << "             SINGLE PROBABILITIES " << std::endl;
  std::cout << "------------------------------------------------------" << std::endl;
  
  std::cout <<std::endl<< "SINGLE  MAP" << std::endl;
  std::unordered_map<char,int> map= construct_map(input);

  std::cout <<std::endl<< "SINGLE PROBABILITIES" << std::endl;
  std::unordered_map<char,float> single_prob = construct_single_prob(map, input);

  float f_entropy = first_entropy(single_prob); 
  std::cout <<std::endl << "First-order entropy:" << f_entropy<< std::endl<< std::endl;
 
  float f_bitlen = generate_huff(map, single_prob);
  std::cout << "Average codeword lengths for Huffman coding : " << f_bitlen << std::endl << std::endl;
  
  /* joint probabilities */
  std::cout << "------------------------------------------------------" << std::endl;
  std::cout << "             JOINT PROBABILITIES " << std::endl;
  std::cout << "------------------------------------------------------" << std::endl;
  
  std::cout <<std::endl<< "JOINT MAP" << std::endl;
  std::unordered_map<std::pair<char,char> , int, hash_pair> joint_map = construct_joint_set(input);

  std::cout <<std::endl<< "JOINT PROBABILITIES" << std::endl;
  std::unordered_map<std::pair<char,char> , float, hash_pair> joint_prob = construct_joint_prob(joint_map, single_prob); 

  float s_entropy = second_entropy(joint_prob); 
  std::cout <<std::endl <<  "SECOND order entropy: " << s_entropy/2<< std::endl << std::endl;

  float s_bitlen = generate_huff_joint(joint_map, joint_prob);
  std::cout << "Average codeword lengths for 2-symbol joint Huffman coding : " << s_bitlen/2 << std::endl << std::endl;
  

  //Generate the first_order_entropy
  //Generate the HuffmanTree and calculate the avg bit_lenght


}


void rand_()
{
  std::string s;
  //generate a random number between 0 -> 35 
  srand(time(NULL));
  int n = (rand()%10)*2;

  //1/4 of times make a joint pair 
  int make_joint = rand()%4;

  for(int i = 0 ; i < n ; i++)
  {
    auto r = rand() % 10; 
    s += 'a' + r;
  } 

  output(s);

  return;

}

void ask_for_input(std::string &s) {

  std::cout<<"enter the sequence of letter, if you would like a randmoly generated sequence, type RANDOM: ";
  std::cin >> s;  
  std::cout << std::endl;

}

int main()
{
  std::string input; 
 
  ask_for_input(input);
  int stop = 1;

  while(stop)
  {
    if(input.size() < 2){ 
      std::cout << "Please enter a sequence that has more than "  << input.size() << std::endl; 
      ask_for_input(input);
    }
    else if(input.size() > 32){
      std::cout << "Please enter a sequence that has less than "  << input.size() << std::endl; 
      ask_for_input(input);

    }
    else
      break;
  } 

  if(input == "RANDOM") {rand_(); return 0;}
  else {output(input);}
  

}
