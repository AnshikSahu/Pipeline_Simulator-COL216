#ifndef __SIMULATOR_HPP__
#define __SIMULATOR_HPP__
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#include <boost/tokenizer.hpp>
#include "parser.hpp"
#include "requirements.hpp"
#include "pipeline.hpp"
using namespace std;
struct Simulator
{	
	struct Parser* parser;
	struct Pipeline* pipeline;
	int registers[32] = {0}, PCcurr = 0, PCnext;
	std::unordered_map<std::string, std::function<int(Simulator &, std::string, std::string, std::string)>> instructions;
	std::unordered_map<std::string, int> registerMap, address;
	static const int MAX = (1 << 20);
	int data[MAX >> 2] = {0};
	std::unordered_map<int, int> memoryDelta;
	std::vector<std::vector<std::string>> commands;
	std::vector<int> commandCount;
	enum exit_code
	{
		SUCCESS = 0,
		INVALID_REGISTER,
		INVALID_LABEL,
		INVALID_ADDRESS,
		SYNTAX_ERROR,
		MEMORY_ERROR
	};
	// constructor to initialise the instruction set
	Simulator(std::ifstream &file,int question)
	{
		instructions = {{"add", &Simulator::add}, {"sub", &Simulator::sub}, {"mul", &Simulator::mul}, {"beq", &Simulator::beq}, {"bne", &Simulator::bne}, {"slt", &Simulator::slt}, {"j", &Simulator::j}, {"lw", &Simulator::lw}, {"sw", &Simulator::sw}, {"addi", &Simulator::addi}};
		parser = new Parser(file,question);
	    commands=parser->commands;
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
		address = parser->address;
		commandCount=parser->commandCount;
		switch(question){
			case 1:
				pipeline=new Pipeline(5,false,true,32,0,{"IF","ID","EX","ME","WB"});
				break;
			case 2:
				pipeline=new Pipeline(5,true,true,32,0,{"IF","ID","EX","ME","WB"});
				break;
			case 3:
				pipeline=new Pipeline(9,false,true,32,0,{"IF1","IF2","ID1","ID2","RR","EX","MEM1","MEM2","WB"});
				break;
			case 4:
				pipeline=new Pipeline(9,true,true,32,0,{"IF1","IF2","ID1","ID2","RR","EX","MEM1","MEM2","WB"});
				break;
			default:
				cerr << "Invalid question number" << endl;
				break;
		}
	}
	// perform add operation
	int add(std::string r1, std::string r2, std::string r3)
	{
		return op(r1, r2, r3, [&](int a, int b)
				  { return a + b; });
	}
	// perform subtraction operation
	int sub(std::string r1, std::string r2, std::string r3)
	{
		return op(r1, r2, r3, [&](int a, int b)
				  { return a - b; });
	}
	// perform multiplication operation
	int mul(std::string r1, std::string r2, std::string r3)
	{
		return op(r1, r2, r3, [&](int a, int b)
				  { return a * b; });
	}
	// perform the binary operation
	int op(std::string r1, std::string r2, std::string r3, std::function<int(int, int)> operation)
	{
		if (!checkRegisters({r1, r2, r3}) || registerMap[r1] == 0)
			return 1;
		registers[registerMap[r1]] = operation(registers[registerMap[r2]], registers[registerMap[r3]]);
		PCnext = PCcurr + 1;
		return 0;
	}
	// perform the beq operation
	int beq(std::string r1, std::string r2, std::string label)
	{
		return bOP(r1, r2, label, [](int a, int b)
				   { return a == b; });
	}
	// perform the bne operation
	int bne(std::string r1, std::string r2, std::string label)
	{
		return bOP(r1, r2, label, [](int a, int b)
				   { return a != b; });
	}
	// implements beq and bne by taking the comparator
	int bOP(std::string r1, std::string r2, std::string label, std::function<bool(int, int)> comp)
	{
		if (!checkLabel(label))
			return 4;
		if (address.find(label) == address.end() || address[label] == -1)
			return 2;
		if (!checkRegisters({r1, r2}))
			return 1;
		PCnext = comp(registers[registerMap[r1]], registers[registerMap[r2]]) ? address[label] : PCcurr + 1;
		return 0;
	}
	// implements slt operation
	int slt(std::string r1, std::string r2, std::string r3)
	{
		if (!checkRegisters({r1, r2, r3}) || registerMap[r1] == 0)
			return 1;
		registers[registerMap[r1]] = registers[registerMap[r2]] < registers[registerMap[r3]];
		PCnext = PCcurr + 1;
		return 0;
	}
	// perform the jump operation
	int j(std::string label, std::string unused1 = "", std::string unused2 = "")
	{
		if (!checkLabel(label))
			return 4;
		if (address.find(label) == address.end() || address[label] == -1)
			return 2;
		PCnext = address[label];
		return 0;
	}
	// perform load word operation
	int lw(std::string r, std::string in1, std::string in2)
	{
		std::string location;
		if(std::find(in1.begin(),in1.end(),'$')!=in1.end()){
			location=in1;
		}
		else{
			location=in1+"("+in2+")";
		}
		if (!checkRegister(r) || registerMap[r] == 0)
			return 1;
		cout<<location<<endl;
		int address = locateAddress(location);
		if (address < 0)
			return abs(address);
		cout<<"Hurray"<<endl;
		registers[registerMap[r]] = data[address];
		PCnext = PCcurr + 1;
		return 0;
	}
	// perform store word operation
	int sw(std::string r, std::string in1, std::string in2)
	{
		std::string location;
		if(std::find(in1.begin(),in1.end(),'$')!=in1.end()){
			location=in1;
		}
		else{
			location=in1+"("+in2+")";
		}
		if (!checkRegister(r))
			return 1;
		int address = locateAddress(location);
		if (address < 0)
			return abs(address);
		if (data[address] != registers[registerMap[r]])
			memoryDelta[address] = registers[registerMap[r]];
		data[address] = registers[registerMap[r]];
		PCnext = PCcurr + 1;
		return 0;
	}
	int locateAddress(std::string location)
	{
		if (location.back() == ')')
		{
			try
			{
				int lparen = location.find('('), offset = stoi(lparen == 0 ? "0" : location.substr(0, lparen));
				cout<<"Test"<<lparen<<" "<<offset<<endl;
				std::string reg = location.substr(lparen + 1);
				cout<<reg<<endl;
				reg.pop_back();
				cout<<reg<<endl;
				if (!checkRegister(reg))
					cout<<location<<endl;
					return -3;
				cout<<"YAY"<<endl;
				int address = registers[registerMap[reg]] + offset;
				if (address % 4 || address < int(4 * commands.size()) || address >= MAX)
					cout<<location<<endl;
					return -3;
				return address / 4;
			}
			catch (std::exception &e)
			{
				return -4;
			}
		}
		try
		{
			int address = stoi(location);
			if (address % 4 || address < int(4 * commands.size()) || address >= MAX)
				cout<<location<<endl;
				return -3;
			return address / 4;
		}
		catch (std::exception &e)
		{
			return -4;
		}
	}
	// perform add immediate operation
	int addi(std::string r1, std::string r2, std::string num)
	{
		if (!checkRegisters({r1, r2}) || registerMap[r1] == 0)
			return 1;
		try
		{
			registers[registerMap[r1]] = registers[registerMap[r2]] + stoi(num);
			PCnext = PCcurr + 1;
			return 0;
		}
		catch (std::exception &e)
		{
			return 4;
		}
	}
	// checks if label is valid
	inline bool checkLabel(std::string str)
	{
		return str.size() > 0 && isalpha(str[0]) && all_of(++str.begin(), str.end(), [](char c)
														   { return (bool)isalnum(c); }) &&
			   instructions.find(str) == instructions.end();
	}
	// checks if the register is a valid one
	inline bool checkRegister(std::string r)
	{
		return registerMap.find(r) != registerMap.end();
	}
	// checks if all of the registers are valid or not
	bool checkRegisters(std::vector<std::string> regs)
	{
		return std::all_of(regs.begin(), regs.end(), [&](std::string r)
						   { return checkRegister(r); });
	}
	/*
		handle all exit codes:
		0: correct execution
		1: register provided is incorrect
		2: invalid label
		3: unaligned or invalid address
		4: syntax error
		5: commands exceed memory limit
	*/
	void handleExit(exit_code code, int cycleCount)
	{
		std::cout << '\n';
		switch (code)
		{
		case 1:
			std::cerr << "Invalid register provided or syntax error in providing register\n";
			break;
		case 2:
			std::cerr << "Label used not defined or defined too many times\n";
			break;
		case 3:
			std::cerr << "Unaligned or invalid memory address specified\n";
			break;
		case 4:
			std::cerr << "Syntax error encountered\n";
			break;
		case 5:
			std::cerr << "Memory limit exceeded\n";
			break;
		default:
			break;
		}
		if (code != 0)
		{
			std::cerr << "Error encountered at:\n";
			for (auto &s : commands[PCcurr])
				std::cerr << s << ' ';
			std::cerr << '\n';
		}
		pipeline->print_pipeline();
	}

	// execute the commands sequentially (no pipelining)
	void executeCommands()
	{
		if (commands.size() >= MAX / 4)
		{
			handleExit(MEMORY_ERROR, 0);
			return;
		}

		int clockCyclesUnpipelined = 0;
		while (PCcurr < commands.size())
		{
			++clockCyclesUnpipelined;
			std::vector<std::string> &command = commands[PCcurr];
			if (instructions.find(command[0]) == instructions.end())
			{
				handleExit(SYNTAX_ERROR, clockCyclesUnpipelined);
				return;
			}
			exit_code ret = (exit_code) instructions[command[0]](*this, command[1], command[2], command[3]);
			if (ret != SUCCESS)
			{
				handleExit(ret, clockCyclesUnpipelined);
				return;
			}
			++commandCount[PCcurr];
			parser->parametric_commands[PCcurr]->value=registers[registerMap[command[1]]];
			pipeline->run_command(parser->parametric_commands[PCcurr]);
			pipeline->save();
			if(command[0]=="beq" || command[0]=="bne" || command[0]=="j"){
				pipeline->insert_halt(parser->parametric_commands[PCcurr]);
			}
			PCcurr = PCnext;
			printRegistersAndMemoryDelta(clockCyclesUnpipelined);
		}
		parser->print_commands();
		handleExit(SUCCESS, clockCyclesUnpipelined);
	}

	// print the register data in hexadecimal
	void printRegistersAndMemoryDelta(int clockCycle)
	{
		for (int i = 0; i < 32; ++i)
			std::cout << registers[i] << ' ';
		std::cout << '\n';
		std::cout << memoryDelta.size() << ' ';
		for (auto &p : memoryDelta)
			std::cout << (p.first)*4 << ' ' << p.second << '\n';
		memoryDelta.clear();
	}
};

#endif
