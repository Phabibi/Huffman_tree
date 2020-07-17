#include <iostream>
#include <ostream>
#include <math.h>
#include <sstream>
#include <vector>
#include <queue>
#include <map>

/* ------------------------ HELPERS ----------------------------*/
template<class T>
void print_map(std::map<std::string,T> map)
{
  std::cout<< "string                                 code" << std::endl; 
  std::cout<<"---------------------------------------------" <<  std::endl;

  for(auto p : map)
  {
    std::cout << p.first << "                             " << p.second << std::endl; 
  }

}

void ask_for_input(std::string &s) {

  std::cout<<"enter the sequence of letter , If you would like randomly generated inputs, type RANDOM: ";
  std::cin >> s;  
}

/* --------------------------------------------------------------*/
void custom(std::string s)
{

  std::cout << std::endl << std::endl << " Sequence: " << s << std::endl;
  //we should use a map since its sorted
  std::map<std::string,int> dict;
  int code_count = 0;
  if(s.size() < 1) throw std::runtime_error("The input is too small");

  std::string first;
  first += s[0];
  int i = 1;

  for(auto c : s)
  {
    std::string new_s;
    new_s += c;
    //i cant find you so imma add a cuont
    if(dict.find(new_s) == dict.end())
    {
       dict[new_s] = code_count;
    }
  }
  for(auto p : dict)
  {
       dict[p.first] += code_count;
       code_count ++;
  }

  //we need to this again cause I want their codes to be added in order 

  std::cout << " OUTPUT : " << std::endl;
  while(i < s.size())
  {
    std::string c;
    c += s[i];
    if(dict.find(first+c) != dict.end())
    {
      first = first+c;
    }

    else
    {
      std::cout << " " << dict[first];
      std::string next = first + c;
      dict[next] = code_count; 
      code_count ++;
      first = c;
    }
    i++;
  }
  std::cout << " " <<  dict[first] << std::endl;
  print_map(dict);
}

void rand_()
{
  std::string s;
  //generate a random number between 0 -> 35 
  srand(time(NULL));
  int n = rand()%35;

  for(int i = 0 ; i < n ; i++)
  {
    auto r = rand() % 26; 
    s += 'a' + r;
  } 

  custom(s);

  return;

}


int main()
{
  std::string s; 
  ask_for_input(s);

  if(s == "RANDOM")
  {
    rand_();
    return 0;
  }
 
  custom(s);

}
