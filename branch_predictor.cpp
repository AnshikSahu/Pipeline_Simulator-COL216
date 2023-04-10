#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

string hex_to_binary(string s)
{
    stringstream ss;
    ss << hex << s;
    unsigned n;
    ss >> n;
    bitset<32> b(n);
    return b.to_string();
}
struct Data {
  int number;
  vector<string> text;
};


int main(int start_state,string bhr_start,string bhr_start1){
  unordered_map<string,Data> our_map;
//   assume l is a vector of strings having hex
//   Predictions nikaalo sab cases me 
  vector<string> l;
  for (int i=0;i<l.size();i++){
    string str= hex_to_binary(l[i]);
    l[i]=str.substr(str.size() - 15);
     if (our_map.find(l[i]) != our_map.end()) {
        our_map[l[i]].number=(our_map[l[i]].number+1)%4;
        if (our_map[l[i]].number<2){
            our_map[l[i]].text={our_map[l[i]].text[-1],"0"};
        }
        else{
            our_map[l[i]].text={our_map[l[i]].text[-1],"1"};
        }
    } else {
        our_map[l[i]].number=(start_state+1)%4;
        if (our_map[l[i]].number<2){
            our_map[l[i]].text={bhr_start1,"0"};
        }
        else{
            our_map[l[i]].text={bhr_start1,"1"};
        }}
    }
    return 0;
};
