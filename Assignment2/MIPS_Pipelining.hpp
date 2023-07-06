/**
 * @file MIPS_Pipelining.hpp
 * @author Garv Nagori and Aryan Sharma
 * Starter Code provided by Mallika Prabhakar and Sayam Sethi
 * Part of COL216 Assignment
 */

#ifndef __MIPS_PIPELINING_HPP__
#define __MIPS_PIPELINING_HPP__

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>
#include <boost/tokenizer.hpp>

struct MIPS_Architecture
{
	int registers[32] = {0}, PCcurr = 0, PCnext = 1;
	// std::unordered_map<std::string, std::function<int(MIPS_Architecture &, std::string, std::string, std::string)>> instructions;
	std::unordered_map<std::string, int> registerMap, address;
	static const int MAX = (1 << 20);
	int data[MAX >> 2] = {0};
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
	MIPS_Architecture(std::ifstream &file)
	{
		// instructions = {{"add", &MIPS_Architecture::add}, {"sub", &MIPS_Architecture::sub}, {"mul", &MIPS_Architecture::mul}, {"beq", &MIPS_Architecture::beq}, {"bne", &MIPS_Architecture::bne}, {"slt", &MIPS_Architecture::slt}, {"j", &MIPS_Architecture::j}, {"lw", &MIPS_Architecture::lw}, {"sw", &MIPS_Architecture::sw}, {"addi", &MIPS_Architecture::addi}};

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

		constructCommands(file);
		commandCount.assign(commands.size(), 0);
	} 

	// perform add operation
	// int add(std::string r1, std::string r2, std::string r3)
	// {
	// 	return op(r1, r2, r3, [&](int a, int b)
	// 			  { return a + b; });
	// }

	// perform subtraction operation
	// int sub(std::string r1, std::string r2, std::string r3)
	// {
	// 	return op(r1, r2, r3, [&](int a, int b)
	// 			  { return a - b; });
	// }

	// perform multiplication operation
	// int mul(std::string r1, std::string r2, std::string r3)
	// {
	// 	return op(r1, r2, r3, [&](int a, int b)
	// 			  { return a * b; });
	// }

	// perform the binary operation
	// int op(std::string r1, std::string r2, std::string r3, std::function<int(int, int)> operation)
	// {
	// 	if (!checkRegisters({r1, r2, r3}) || registerMap[r1] == 0)
	// 		return 1;
	// 	registers[registerMap[r1]] = operation(registers[registerMap[r2]], registers[registerMap[r3]]);
	// 	PCnext = PCcurr + 1;
	// 	return 0;
	// }

	// perform the beq operation
	// int beq(std::string r1, std::string r2, std::string label)
	// {
	// 	return bOP(r1, r2, label, [](int a, int b)
	// 			   { return a == b; });
	// }

	// perform the bne operation
	// int bne(std::string r1, std::string r2, std::string label)
	// {
	// 	return bOP(r1, r2, label, [](int a, int b)
	// 			   { return a != b; });
	// }

	// implements beq and bne by taking the comparator
	// int bOP(std::string r1, std::string r2, std::string label, std::function<bool(int, int)> comp)
	// {
	// 	if (!checkLabel(label))
	// 		return 4;
	// 	if (address.find(label) == address.end() || address[label] == -1)
	// 		return 2;
	// 	if (!checkRegisters({r1, r2}))
	// 		return 1;
	// 	PCnext = comp(registers[registerMap[r1]], registers[registerMap[r2]]) ? address[label] : PCcurr + 1;
	// 	return 0;
	// }

	// implements slt operation
	// int slt(std::string r1, std::string r2, std::string r3)
	// {
	// 	if (!checkRegisters({r1, r2, r3}) || registerMap[r1] == 0)
	// 		return 1;
	// 	registers[registerMap[r1]] = registers[registerMap[r2]] < registers[registerMap[r3]];
	// 	PCnext = PCcurr + 1;
	// 	return 0;
	// }

	// perform the jump operation
	// int j(std::string label, std::string unused1 = "", std::string unused2 = "")
	// {
	// 	if (!checkLabel(label))
	// 		return 4;
	// 	if (address.find(label) == address.end() || address[label] == -1)
	// 		return 2;
	// 	PCnext = address[label];
	// 	return 0;
	// }

	// perform load word operation
	// int lw(std::string r, std::string location, std::string unused1 = "")
	// {
	// 	if (!checkRegister(r) || registerMap[r] == 0)
	// 		return 1;
	// 	int address = locateAddress(location);
	// 	if (address < 0)
	// 		return abs(address);
	// 	registers[registerMap[r]] = data[address];
	// 	// PCnext = PCcurr + 1;
	// 	return 0;
	// }

	// perform store word operation
	// int sw(std::string r, std::string location, std::string unused1 = "")
	// {
	// 	if (!checkRegister(r))
	// 		return 1;
	// 	int address = locateAddress(location);
	// 	if (address < 0)
	// 		return abs(address);
	// 	data[address] = registers[registerMap[r]];
	// 	// PCnext = PCcurr + 1;
	// 	return 0;
	// }

	// int locateAddress(std::string location)
	// {
	// 	if (location.back() == ')')
	// 	{
	// 		try
	// 		{
	// 			int lparen = location.find('('), offset = stoi(lparen == 0 ? "0" : location.substr(0, lparen));
	// 			std::string reg = location.substr(lparen + 1);
	// 			reg.pop_back();
	// 			if (!checkRegister(reg))
	// 				return -3;
	// 			int address = registers[registerMap[reg]] + offset;
	// 			if (address % 4 || address < int(4 * commands.size()) || address >= MAX)
	// 				return -3;
	// 			return address / 4;
	// 		}
	// 		catch (std::exception &e)
	// 		{
	// 			return -4;
	// 		}
	// 	}
	// 	try
	// 	{
	// 		int address = stoi(location);
	// 		if (address % 4 || address < int(4 * commands.size()) || address >= MAX)
	// 			return -3;
	// 		return address / 4;
	// 	}
	// 	catch (std::exception &e)
	// 	{
	// 		return -4;
	// 	}
	// }

	// perform add immediate operation
	// int addi(std::string r1, std::string r2, std::string num)
	// {
	// 	if (!checkRegisters({r1, r2}) || registerMap[r1] == 0)
	// 		return 1;
	// 	try
	// 	{
	// 		registers[registerMap[r1]] = registers[registerMap[r2]] + stoi(num);
	// 		PCnext = PCcurr + 1;
	// 		return 0;
	// 	}
	// 	catch (std::exception &e)
	// 	{
	// 		return 4;
	// 	}
	// }

	// checks if label is valid
	// inline bool checkLabel(std::string str)
	// {
	// 	return str.size() > 0 && isalpha(str[0]) && all_of(++str.begin(), str.end(), [](char c)
	// 													   { return (bool)isalnum(c); }) &&
	// 		   instructions.find(str) == instructions.end();
	// }

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
		std::cout << "\nFollowing are the non-zero data values:\n";
		for (int i = 0; i < MAX / 4; ++i)
			if (data[i] != 0)
				std::cout << 4 * i << '-' << 4 * i + 3 << std::hex << ": " << data[i] << '\n'
						  << std::dec;
		std::cout << "\nTotal number of cycles: " << cycleCount << '\n';
		std::cout << "Count of instructions executed:\n";
		for (int i = 0; i < (int)commands.size(); ++i)
		{
			std::cout << commandCount[i] << " times:\t";
			for (auto &s : commands[i])
				std::cout << s << ' ';
			std::cout << '\n';
		}
	}

	// parse the command assuming correctly formatted MIPS instruction (or label)
	void parseCommand(std::string line)
	{
		if(line[line.length()-1] == '\n' | line[line.length()-1] == '\r') line = line.substr(0, line.length()-1);
		// strip until before the comment begins
		line = line.substr(0, line.find('#'));
		std::vector<std::string> command;
		boost::tokenizer<boost::char_separator<char>> tokens(line, boost::char_separator<char>(", \t"));
		for (auto &s : tokens)
			command.push_back(s);
		// empty line or a comment only line
		if (command.empty())
			return;
		else if (command.size() == 1)
		{
			// command[0] = command[0].substr(0, command[0].size() - 1);
			std::string label = command[0].back() == ':' ? command[0].substr(0, command[0].size() - 1) : "?";
			if (address.find(label) == address.end())
				address[label] = commands.size();
			else
				address[label] = -1;
			command.clear();
		}
		else if (command[0].back() == ':')
		{
			std::string label = command[0].substr(0, command[0].size() - 1);
			if (address.find(label) == address.end())
				address[label] = commands.size();
			else
				address[label] = -1;
			command = std::vector<std::string>(command.begin() + 1, command.end());
		}
		else if (command[0].find(':') != std::string::npos)
		{
			int idx = command[0].find(':');
			std::string label = command[0].substr(0, idx);
			if (address.find(label) == address.end())
				address[label] = commands.size();
			else
				address[label] = -1;
			command[0] = command[0].substr(idx + 1);
		}
		else if (command[1][0] == ':')
		{
			if (address.find(command[0]) == address.end())
				address[command[0]] = commands.size();
			else
				address[command[0]] = -1;
			command[1] = command[1].substr(1);
			if (command[1] == "")
				command.erase(command.begin(), command.begin() + 2);
			else
				command.erase(command.begin(), command.begin() + 1);
		}
		if (command.empty())
			return;
		if (command.size() > 4)
			for (int i = 4; i < (int)command.size(); ++i)
				command[3] += " " + command[i];
		command.resize(4);
		commands.push_back(command);
	}

    int ALU(int data1, int data2, int command, int clockCycles){
        int result; bool zero;
        switch (command){
        case 1: // Add operation
            result = data1 + data2;
            break;
        case 2: // Sub operation
            result = data1 - data2;
            break;
        case 3: // Mul operation
            result = data1 * data2;
            break;
        case 4: // Slt operation
            result = data1 < data2;
            break;
        case 5: // beq operation
            result = data1 == data2;
            break;
        case 6: // bne operation
            result = data1 != data2;
            break;
		case 7: // and operation
			result = data1 & data2;
			break;
		case 8: // or operation
			result = data1 | data2;
			break;
		case 9: // nor operation
			result = ~ (data1 | data2);
			break;
		case 10: // sll operation
			result = data1 << data2;
			break;
		case 11: // srl operation
			result = data1 >> data2;
			break;
        default:
            handleExit((exit_code) 3, clockCycles);
            break;
        }
        return result;
    }

	// construct the commands vector from the input file
	void constructCommands(std::ifstream &file)
	{
		std::string line;
		while (getline(file, line))
			parseCommand(line);
		file.close();
	}

    struct IfIdReg{
        std::vector<std::string> instr = {"", "", "", ""};
    } *IfId ;

	void clear(IfIdReg *IfId){ IfId->instr = {"", "", "", ""}; }

    struct IdExReg{
		std::vector<bool> EXcontrol = {0,0}; // 0 - ALUSrc, 1 - RegDst
		std::vector<bool> Mcontrol = {0,0,0}; // 0 - MemWrite, 1 - MemRead, 2 - PCSrc
		std::vector<bool> WBcontrol = {0,0}; // 0 - MemToReg, 1 - RegWrite
        std::string branch = "";
        std::string rt = "";
		std::string rd = "";
        int data1 = 0;
        int data2 = 0;
        int dataIm = 0;
        int ALUOp = 0;
    } *IdEx ;

	void clear(IdExReg *IdEx){
		IdEx->EXcontrol = {0,0}; IdEx->Mcontrol = {0,0,0}; IdEx->WBcontrol={0,0};
		IdEx->branch = ""; IdEx->rd = ""; IdEx->rd = ""; IdEx->data1 = IdEx->data2 = IdEx->dataIm = IdEx->ALUOp = 0;	
	}

    struct ExMReg{
		std::vector<bool> Mcontrol = {0,0}; // 0 - MemWrite, 1 - MemRead, 2 - PCSrc
		std::vector<bool> WBcontrol = {0,0}; // 0 - MemToReg, 1 - RegWrite
        std::string branch = "";
        std::string regDest = "";
		int result = 0;
        int writeData = 0;
		int PCNext = 0;
    } *ExM ;

	void clear(ExMReg *ExM){
		ExM->Mcontrol = {0,0}; ExM->WBcontrol = {0,0}; ExM->branch = ""; ExM->regDest = "";
		ExM->result = ExM->writeData = ExM->result = 0;
	}

    struct MWReg{
		std::vector<bool> WBcontrol = {0,0}; // 0 - MemToReg, 1 - RegWrite
        int memData = 0;
        int result = 0;
        std::string regDest = "$s0";
    } *MW ;

	void clear(MWReg *MW){ MW->WBcontrol = {0,0}; MW->regDest = "$s0"; MW->regDest = MW->result = 0; }

	std::pair<int, int> locateAddress (std::string location){
		if(location.back() == ')'){
			try{
				int lparen = location.find('('), offset = stoi(lparen == 0 ? "0" : location.substr(0, lparen));
				std::string reg = location.substr(lparen + 1);
				reg.pop_back();
				if (!checkRegister(reg)) return std::make_pair(-3, 0);
				int address = registers[registerMap[reg]];
				if (address % 4 || address < int(4 * commands.size()) || address >= MAX) return std::make_pair(-3, 0);
				return std::make_pair(address / 4, offset / 4);
			}
			catch (std::exception &e){ return std::make_pair(-4, 0);}
		}
		try{
			int address = stoi(location);
			if (address % 4 || address < int(4 * commands.size()) || address >= MAX) return std::make_pair(-3, 0);
			return std::make_pair(address / 4, 0);
		}
		catch (std::exception &e){ return std::make_pair(-4, 0);}
	}

	void ControlUnit  (std::vector<std::string> instr, IdExReg* IdEx){
		IdEx->rt = instr[3]; IdEx->rd = instr[1];
		if(instr[0] ==  "add"){
			IdEx->ALUOp = 1; IdEx->branch = ""; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2], instr[3]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[2]]]; IdEx->data2 = registers[registerMap[instr[3]]];
		}
		else if(instr[0] ==  "addi"){
			IdEx->ALUOp = 1; IdEx->branch = ""; IdEx->data2 = 0;
			IdEx->EXcontrol = {1, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[1]]]; IdEx->dataIm = stoi(instr[3]);
		}
		else if(instr[0] ==  "sub"){
			IdEx->ALUOp = 2; IdEx->branch = ""; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2], instr[3]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[2]]]; IdEx->data2 = registers[registerMap[instr[3]]];
		}
		else if(instr[0] ==  "mul"){
			IdEx->ALUOp = 3; IdEx->branch = ""; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2], instr[3]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[2]]]; IdEx->data2 = registers[registerMap[instr[3]]];
		}
		else if(instr[0] ==  "slt"){
			IdEx->ALUOp = 4; IdEx->branch = ""; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2], instr[3]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[2]]]; IdEx->data2 = registers[registerMap[instr[3]]];
		}
		else if(instr[0] ==  "and"){
			IdEx->ALUOp = 7; IdEx->branch = ""; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2], instr[3]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[2]]]; IdEx->data2 = registers[registerMap[instr[3]]];
		}
		else if(instr[0] ==  "or"){
			IdEx->ALUOp = 8; IdEx->branch = ""; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2], instr[3]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[2]]]; IdEx->data2 = registers[registerMap[instr[3]]];
		}
		else if(instr[0] ==  "nor"){
			IdEx->ALUOp = 9; IdEx->branch = ""; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2], instr[3]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[2]]]; IdEx->data2 = registers[registerMap[instr[3]]];
		}
		else if(instr[0] ==  "addi"){
			IdEx->ALUOp = 7; IdEx->branch = ""; IdEx->data2 = 0;
			IdEx->EXcontrol = {1, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[1]]]; IdEx->dataIm = stoi(instr[3]);
		}
		else if(instr[0] ==  "ori"){
			IdEx->ALUOp = 8; IdEx->branch = ""; IdEx->data2 = 0;
			IdEx->EXcontrol = {1, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[1]]]; IdEx->dataIm = stoi(instr[3]);
		}
		else if(instr[0] ==  "sll"){
			IdEx->ALUOp = 10; IdEx->branch = ""; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2], instr[3]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[2]]]; IdEx->data2 = registers[registerMap[instr[3]]];
		}
		else if(instr[0] ==  "srl"){
			IdEx->ALUOp = 11; IdEx->branch = ""; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 0}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2], instr[3]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[2]]]; IdEx->data2 = registers[registerMap[instr[3]]];
		}
		else if(instr[0] ==  "lw"){
			IdEx->ALUOp = 1; IdEx->branch = ""; IdEx->data2 = 0;
			IdEx->EXcontrol = {1, 0}; IdEx->Mcontrol = {0, 1, 0}; IdEx->WBcontrol = {1, 1};
			std::tie(IdEx->data1, IdEx->dataIm) = locateAddress(instr[2]);
		}
		else if(instr[0] == "sw"){
			IdEx->ALUOp = 1; IdEx->branch = ""; IdEx->data2 = 0;
			IdEx->EXcontrol = {1, 0}; IdEx->Mcontrol = {1, 0, 0}; IdEx->WBcontrol = {1, 1};
			std::tie(IdEx->data1, IdEx->dataIm) = locateAddress(instr[2]);
		}
		else if(instr[0] == "beq"){
			IdEx->ALUOp = 5; IdEx->branch = instr[3]; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 1}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[1]]]; IdEx->data2 = registers[registerMap[instr[2]]];
		}
		else if(instr[0] == "bne"){
			IdEx->ALUOp = 6; IdEx->branch = instr[3]; IdEx->dataIm = 0;
			IdEx->EXcontrol = {0, 0}; IdEx->Mcontrol = {0, 0, 1}; IdEx->WBcontrol = {0, 1};
			if (!checkRegisters({instr[1], instr[2]}) || registerMap[instr[1]] == 0){
				IdEx->ALUOp = -1;
				return;
			}
			IdEx->data1 = registers[registerMap[instr[1]]]; IdEx->data2 = registers[registerMap[instr[2]]];
		}
		else IdEx->ALUOp = -4;
    }

	void changeStall(bool *stalled){
		if(stalled[4]) stalled[4] = 0;
		if(stalled[3]){
			stalled[3] = 0; 
			stalled[4] = 1;
		}
		if(stalled[2]){
			stalled[2] = 0; 
			stalled[3] = 1;
		}
		if(stalled[1]){
			stalled[1] = 0;
			stalled[2] = 1;
		}
	}

	void branchStall(bool *stalled, int *brstall, int *brnext){
		if(*brstall == 1){
			stalled[1] = 1;
			*brstall = *brstall+1;
		}
		else if(*brstall == 2){
			stalled[2] = 1;
			*brstall = *brstall+1;
		}
		else if(*brstall == 3){
			stalled[0] = 0; stalled[3] = 1; stalled[4]=1;
			*brstall = 0;
			*brnext = 1;
		}
		if(*brnext == 1){
			stalled[1] = 0;
			*brnext = *brnext+1;
		}
		else if(*brnext == 2){
			stalled[2] = 0;
			*brnext = *brnext+1;
		}
		else if(*brnext == 3){
			stalled[3] = 0;
			*brnext = *brnext+1;
		}
		else if(*brnext == 4){
			stalled[4] = 0;
			*brstall = 0;
		}
		else{ return; }
	}

	void executeCommandsPipelined()
	{
		std::cout<<"Start \n";
		MWReg *MW = new MWReg();
		ExMReg *ExM = new ExMReg();
		IdExReg *IdEx = new IdExReg();
		IfIdReg *IfId = new IfIdReg();
		if (commands.size() >= MAX / 4)
		{
			handleExit(MEMORY_ERROR, 0);
			return;
		}

		int clockCycles = 0;
		bool halfCycle = false; // false corresponds to first half cycle, true corresponds to second half cycle
		bool stalled[5] = {false, false, false, false, false}; // for stalling
		int brstall = 0, brnext = 0;
		std::cout<<commands.size();
		while ( PCcurr < (commands.size()+4) ){
			++clockCycles;
			halfCycle = false;
			// W stage
			if(clockCycles>=5 && (PCcurr < (commands.size()+4)) && !stalled[4]){
				int writeData = (MW->WBcontrol[0]) ? MW->memData : MW->result;
				if(MW->WBcontrol[1]){
					if (!checkRegister(MW->regDest)){
						handleExit((exit_code) 1, clockCycles);
						return;
					}
					registers[registerMap[MW->regDest]] = writeData;
				}
				std::cout<<"W stage \n";
			}

			halfCycle = true;
			// M stage
			if(clockCycles>=4 && (PCcurr < (commands.size()+3)) && !stalled[3]){
				if(ExM->Mcontrol[0]){
					if (ExM->result < 0){
						handleExit( (exit_code) abs(ExM->result), clockCycles);
						return;
					}
					data[ExM->result] = ExM->writeData;
					MW->memData = 0;
				}
				else if(ExM->Mcontrol[1]){
					if (ExM->result < 0){
						handleExit( (exit_code) abs(ExM->result), clockCycles);
						return;
					}
					MW->memData = data[ExM->result];
				}
				MW->WBcontrol = ExM->WBcontrol; MW->result = ExM->result; MW->regDest = ExM->regDest;
				if(ExM->Mcontrol[2] && ExM->result == 1){
		        	if (address.find(ExM->branch) == address.end() || address[ExM->branch] == -1){
                	    handleExit( (exit_code) 2, clockCycles);
                	    return;
                	}
					// PCnext =  address[ExM->branch];
					PCcurr =  address[ExM->branch];
					// clear all pipelining registers and unstall everything
					clear(IfId); clear(IdEx);clear(ExM);
				}
				std::cout<<"M stage \n";
			}

            // Ex Stage
            if(clockCycles>=3 && (PCcurr < (commands.size()+2)) && !stalled[2]){
                int dataR = (IdEx->EXcontrol[0])? IdEx->dataIm : IdEx->data2;
                ExM->result = ALU(IdEx->data1, dataR, IdEx->ALUOp, clockCycles);
				ExM->WBcontrol = IdEx->WBcontrol; ExM->Mcontrol = IdEx->Mcontrol; ExM->branch = IdEx->branch; 
				ExM->regDest = (IdEx->EXcontrol[1])? IdEx->rt : IdEx->rd;
				std::cout<<"Ex stage \n";
			}

            // Id Stage
            if(clockCycles>=2 && (PCcurr < (commands.size()+1)) && !stalled[1]){
				if(IfId->instr[0] == "j"){
					IfId->instr[1] = IfId->instr[1].substr(0, IfId->instr[1].size() - 1);
					if (address.find(IfId->instr[1]) == address.end() || address[IfId->instr[1]] == -1){
						handleExit((exit_code) 2, clockCycles);
						return;
					}
					PCcurr = address[IfId->instr[1]]; changeStall(stalled); PCnext = PCcurr  + 1;
					clear(IfId); clear(IdEx); stalled[1] = stalled[2] = true;
					continue;
				}
				else if((IfId->instr[0] == "beq") || (IfId->instr[0] == "bne")){
					brstall = 1; stalled[0] = true;
				}
                ControlUnit(IfId->instr, IdEx);
				if(IdEx->ALUOp < 0){
					handleExit( (exit_code) abs(IdEx->ALUOp), clockCycles);
					return;
				}
				std::cout<<"Id stage \n";
            }

			if((PCcurr < commands.size()) && !stalled[0]){
				IfId->instr = commands[PCcurr];
				std::cout<<"If stage \n";
				PCcurr++;
			}
			// PCcurr = PCnext;
			// PCnext++;
			if(brstall == 0) changeStall(stalled);
			else if(brstall!=0 || brnext!=0) branchStall(stalled, &brstall, &brnext);
			printRegisters(clockCycles);
		}
		handleExit(SUCCESS, clockCycles);
	}

	// print the register data in hexadecimal
	void printRegisters(int clockCycle)
	{
		std::cout << "Cycle number: " << clockCycle << '\n'
				  << std::hex;
		for (int i = 0; i < 32; ++i)
			std::cout << registers[i] << ' ';
		std::cout << std::dec << '\n';
	}
};

#endif