#include "readconfig.hpp"

using namespace std;
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
int main(){
    std::unordered_map<std::string, std::vector<std::string>> stage_names;
    std::vector<std::vector<int>> parameters;
    std::unordered_map<std::string, std::vector<int>> indices;
    Configuration *config = new Configuration("data.txt");
    config->read_data_from_file();
    stage_names = config->stage_names;
    parameters = config->parameters;
    indices = config->indices;
}