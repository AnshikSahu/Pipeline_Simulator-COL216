#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
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

vector<string> constructCommands(ifstream &file)
	{
		string line;
        vector<string> l;
		while (getline(file, line))
			l.push_back(line);
		file.close();
    return l;
	};

int main(int start_state,string bhr_start,string bhr_start1){
  unordered_map<string,Data> our_map;
//   Predictions nikaalo sab cases me 
  vector<string> l= constructCommands(branchtrace);
  for (int i=0;i<l.size();i++){
    string x = l[i].substr(0, l[i].find(" "));
    string str= hex_to_binary(x);
    str = str.substr(str.size() - 15);
     if (our_map.find(x) != our_map.end()) {
        // this is not +1 check
        our_map[x].number=(our_map[x].number+1)%4;
        if (our_map[x].number<2){
            our_map[x].text={our_map[x].text[-1],"0"};
        }
        else{
            our_map[x].text={our_map[x].text[-1],"1"};
        }
    } else {
        our_map[x].number=(start_state+1)%4;
        if (our_map[x].number<2){
            our_map[x].text={bhr_start1,"0"};
        }
        else{
            our_map[x].text={bhr_start1,"1"};
        }}
    }
    return 0;
};
