#pragma once
#include <stdint.h>
#include <string>
class Interpreter
{
	uint8_t* ram;
	uint8_t* rom = nullptr;
	size_t romSize = 0;
	uint16_t registers[256];
	uint16_t stackBase = 0;
	bool running = false;
	bool hasError = false;
	std::string errorString = "";
	void error(std::string message);
	void cycle();
	void dump();
	void lit();
	void jmp();
	void jez();
	void jgz();
	void jlz();
	void jnz();
	void dec();
	void inc();
	void pop();
	void push();
	void mov();
	void mul();
	void add();
	void sub();
	void neg();
	void clear();
	void ipush(uint8_t value);
	uint8_t ipop();
	void ipush16(uint16_t value);
	uint16_t ipop16();
	uint8_t getArg();
	uint32_t getVbuffer();
	uint16_t makeUint16(uint8_t x, uint8_t y);
	void reset();
public:
	void run();
	void loadProgram(uint8_t* _rom);
	void setVbuffer(unsigned char* data);
	Interpreter();
	~Interpreter();
};

