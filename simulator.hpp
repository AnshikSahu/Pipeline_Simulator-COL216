#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "parser.hpp"
#include "requirements.hpp"

using namespace std;

struct Simulator{

    int registers[32] = {0}, PCcurr = 0, PCnext;
    static const int MAX = (1 << 20);
	int data[MAX >> 2] = {0};
	std::unordered_map<int, int> memoryDelta;
    struct Parser* parser;
    struct Pipeline* pipeline;

    Simulator(string filename,int question){
        parser = new_Parser(filename,question);
        int in1,in4,in5;
        bool in2,in3;
        vector<string> in6;
        switch (question)
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            cerr << "Invalid question number" << endl;
            break;
        }
        pipeline = new_Pipeline(in1,in2,in3,in4,in5,in6);
    }

    // execute the commands sequentially (no pipelining)
	void executeCommandsUnpipelined()
	{
		if (commands.size() >= MAX / 4)
		{
			handleExit(MEMORY_ERROR, 0);
			return;
		}

		int clockCycles = 0;
		while (PCcurr < commands.size())
		{
			++clockCycles;
			std::vector<std::string> &command = commands[PCcurr];
			if (instructions.find(command[0]) == instructions.end())
			{
				handleExit(SYNTAX_ERROR, clockCycles);
				return;
			}
			exit_code ret = (exit_code) instructions[command[0]](*this, command[1], command[2], command[3]);
			if (ret != SUCCESS)
			{
				handleExit(ret, clockCycles);
				return;
			}
			++commandCount[PCcurr];
			PCcurr = PCnext;
			printRegistersAndMemoryDelta(clockCycles);
		}
		handleExit(SUCCESS, clockCycles);
	}

	// print the register data in hexadecimal
	void printRegistersAndMemoryDelta(int clockCycle)
	{
		for (int i = 0; i < 32; ++i)
			std::cout << registers[i] << ' ';
		std::cout << '\n';
		std::cout << memoryDelta.size() << ' ';
		for (auto &p : memoryDelta)
			std::cout << p.first << ' ' << p.second << '\n';
		memoryDelta.clear();
	}

};
#endif