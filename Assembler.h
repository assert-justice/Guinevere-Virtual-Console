#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <stdint.h>
#include <unordered_map>
#include <iostream>
#include <stdlib.h>

struct Token
{
	std::string literal;
	int value;
	size_t line;
	enum TokenType
	{
		opcode,
		reg,
		keyword,
		number,
		label,
		identifier,
		string,
		eof,
	};
	TokenType type;
	void print()
	{
		std::string types[] = { "Opcode", "Register", "Keyword", "Number", "Label", "Identifier", "String", "Eof" };
		std::cout << "Token Literal: " << literal << "\tValue: " << value << "\tType: " << types[type] << "\tLine: " << (int)line << std::endl;
	}
};

class Scanner
{
	std::string source = "";
	std::vector<Token> tokens;
	std::unordered_map<std::string, uint8_t>* opcodes;
	std::unordered_map<std::string, uint8_t>* registers;
	std::unordered_set<std::string>* keywords;
	size_t start = 0;
	size_t current = 0;
	size_t line = 1;
	bool hasError = false;
	void scanToken();
	char advance();
	bool isAtEof();
	bool isNumeric(char c);
	bool isReserved(char c);
	std::string getIdentifier();
	void error(std::string message);
	void number();
	void comment();
	void reg();
	void label();
	void identifier();
	void str();
public:
	std::vector<Token>* scan(std::string _source);
	void init(std::unordered_map<std::string, uint8_t>* _opcodes,
		std::unordered_map<std::string, uint8_t>* _registers,
		std::unordered_set<std::string>* _keywords);
};

struct Label
{
	Token* head = nullptr;
	size_t headPos = 0;
	std::vector<Token*> refs;
	std::vector<size_t> refPositions;
};

class Parser
{
	std::vector<Token>* tokens;
	size_t current = 0;
	std::vector<uint8_t> program;
	bool hasError = false;
	Token advance();
	void error(std::string message, Token t);
	void writeNumber(size_t value);
public:
	uint8_t* parse(std::vector<Token>* _tokens);
};

class Assembler
{
	std::string source = "";
	std::vector<uint8_t> program;
	std::unordered_map<std::string, uint8_t> opcodes;
	std::unordered_map<std::string, uint8_t> registers;
	std::unordered_set<std::string> keywords;
	Scanner scanner;
	Parser parser;
public:
	uint8_t* compile(std::string _source);
	Assembler();
};