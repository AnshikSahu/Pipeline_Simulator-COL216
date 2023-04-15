#ifndef __parser__
#define __parser__

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <exception>
#include <iostream>
#include <boost/tokenizer.hpp>
#include "requirements.hpp"
#include "pipeline.hpp"
#include "configuration.hpp"

using namespace std;

struct Parser
{
    vector< struct Command* > parametric_commands;
    vector<vector<int>> parametrs;
    std:: unordered_map<string,vector<int>> indices;
    std:: unordered_map<string,vector<string>> stage_names;
	struct Pipeline *pipeline;
	std::unordered_map<std::string, int> registerMap, address;
	std::vector<std::vector<std::string>> commands;
	std::vector<int> commandCount;

	Parser(std::ifstream &file, int question)
	{
		for (int i = 0; i < 32; ++i)
			registerMap["$" + std::to_string(i)] = i;
		registerMap["$zero"] = 0;
		registerMap["$at"] = 1;
		registerMap["$v0"] = 2;
		registerMap["$v1"] = 3;
		for (int i = 0; i < 4; ++i)
			registerMap["$a" + std::to_string(i)] = i + 4;
		for (int i = 0; i < 8; ++i)
			registerMap["$t" + std::to_string(i)] = i + 8, registerMap["$s" + std::to_string(i)] = i + 16;
		registerMap["$t8"] = 24;
		registerMap["$t9"] = 25;
		registerMap["$k0"] = 26;
		registerMap["$k1"] = 27;
		registerMap["$gp"] = 28;
		registerMap["$sp"] = 29;
		registerMap["$s8"] = 30;
		registerMap["$ra"] = 31;
		struct Configuration* config = new Configuration(question);
		parametrs = config->parametrs;
		indices = config->indices;
		stage_names = config->stage_names;
		pipeline = config->pipeline;
		constructCommands(file);
		commandCount.assign(commands.size(), 0);
	}

    struct Command* define(std::vector<string> command){
        bool in1=parametrs[0][0]==1;
        int in2=parametrs[0][1];
        vector<int> in4=indices[command[0]];
        vector<int> in5;
        vector<string> command_type1={"add","sub","and","or","slt","mul","and","or"}, command_type2={"addi","andi","ori","sll","srl"};
		vector<string> command_type3={"beq","bne"}, command_type4={"j","jal"};
		int in9;
		std::vector<std::string> subsetVec;
		std::copy_if(command.begin(), command.end(), std::back_inserter(subsetVec), 
			[](const std::string& s) {
				return s.find("$") != std::string::npos;
			}
		);
		if(std::find(command_type1.begin(),command_type1.end(),command[0])!=command_type1.end()){
			in5={registerMap[subsetVec[0]],registerMap[subsetVec[1]],registerMap[subsetVec[2]]};
			in9=-1;
		}
		else if(std::find(command_type2.begin(),command_type2.end(),command[0])!=command_type2.end()){
			in5={registerMap[subsetVec[0]],registerMap[subsetVec[1]],-1};
			in9=stoi(command[3]);
		}
		else if(command[0]=="lw"){
			in5={registerMap[subsetVec[0]],registerMap[subsetVec[1]],-1};
			if(std::find(command[2].begin(),command[2].end(),'$')==command[2].end()){
				in9=stoi(command[2]);
			}
			else{
				in9=0;
			}
		}
		else if(command[0]=="sw"){
			in5={-1,registerMap[subsetVec[0]],registerMap[subsetVec[1]]};
			if(std::find(command[2].begin(),command[2].end(),'$')==command[2].end()){
				in9=stoi(command[2]);
			}
			else{
				in9=0;
			}
		}
		else if(std::find(command_type3.begin(),command_type3.end(),command[0])!=command_type3.end()){
			in5={-1,registerMap[subsetVec[0]],registerMap[subsetVec[1]]};
			in9=-1;
		}
		else if(std::find(command_type4.begin(),command_type4.end(),command[0])!=command_type4.end()){
			in5={-1,-1,-1};
			in9=-1;
		}
		else{
			std:: cerr <<"error in command type"<<std::endl;
		}
        vector<string> in6=stage_names[command[0]];
		vector<int> in3;
		unordered_map<string, int> stagemap;
		for (int i = 0; i < (int)in6.size(); i++) {
            stagemap[in6[i]] = i;
        }
		for(int i=0;i<in6.size();i++){
			in3.push_back(parametrs[1][stagemap[in6[i]]]);
		}
        string in7=command[0];
        struct Command* cmd = new Command(in1,in2,in3,in4,in5,in6,in7,0,in9);
        return cmd;
    }

	void parseCommand(std::string line)
	{
		// strip until before the comment begins
		line = line.substr(0, line.find('#'));
		std::vector<std::string> command;
		boost::tokenizer<boost::char_separator<char>> tokens(line, boost::char_separator<char>(", \t()"));
		for (auto &s : tokens){
			command.push_back(s);
		}
		// empty line or a comment only line
		if (command.empty()){
			return;
		}
		else if (command.size() == 1)
		{
			std::string label = command[0].back() == ':' ? command[0].substr(0, command[0].size() - 1) : "?";
			if (address.find(label) == address.end()){
				address[label] = commands.size();
			}
			else{
				address[label] = -1;
			}
			command.clear();
		}
		else if (command[0].back() == ':')
		{
			std::string label = command[0].substr(0, command[0].size() - 1);
			if (address.find(label) == address.end()){
				address[label] = commands.size();
			}
			else{
				address[label] = -1;
			}
			command = std::vector<std::string>(command.begin() + 1, command.end());
		}
		else if (command[0].find(':') != std::string::npos)
		{
			int idx = command[0].find(':');
			std::string label = command[0].substr(0, idx);
			if (address.find(label) == address.end()){
				address[label] = commands.size();
			}
			else{
				address[label] = -1;
			}
			command[0] = command[0].substr(idx + 1);
		}
		else if (command[1][0] == ':')
		{
			if (address.find(command[0]) == address.end()){
				address[command[0]] = commands.size();
			}
			else{
				address[command[0]] = -1;
			}
			command[1] = command[1].substr(1);
			if (command[1] == ""){
				command.erase(command.begin(), command.begin() + 2);
			}
			else{
				command.erase(command.begin(), command.begin() + 1);
			}
		}
		if (command.empty()){
			return;
		}
		if (command.size() > 4){
			for (int i = 4; i < (int)command.size(); ++i){
				command[3] += " " + command[i];
			}
		}
		command.resize(4);
		commands.push_back(command);
        struct Command* parametric= define(command);
        parametric_commands.push_back(parametric);
	}

	void print_commands()
	{
		for(int i=0;i<(int)commands.size();i++){
			parametric_commands[i]->print_command();
		}
	}

	void constructCommands(std::ifstream &file)
	{
		std::string line;
		while (getline(file, line))
			parseCommand(line);
		file.close();
	}
};
#endif
