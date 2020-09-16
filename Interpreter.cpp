#include "Interpreter.h"
#include <stdlib.h>
#include <iostream>
#include "Opcodes.h"

constexpr int BANK_SIZE = 65536;
constexpr int NUM_BANKS = 1;
constexpr int RAM_SIZE = BANK_SIZE * NUM_BANKS;
constexpr int PAL = 9;
constexpr int VRAM = 10;
constexpr int OSET = 11;
constexpr int STAT = 12;
constexpr int ACC = 13;
constexpr int SP = 14;
constexpr int IP = 15;
constexpr int MAX_LAG = 65536;

void Interpreter::run()
{
	if (rom == nullptr)
	{
		error("No program loaded");
		return;
	}
	running = true;
	if (hasError) return;
	for (size_t i = 0; i < MAX_LAG; i++)
	{
		cycle();
		if (!running) return;
	}
	error("Excessive lag detected. Are you in an infinite loop?");
}

void Interpreter::cycle()
{
	//std::cout << "Instruction " << registers[IP] << std::endl;
	/*if (registers[IP] >= romSize)
	{
		error("Execution reached end of program."); 
		return;
	}*/
	uint8_t op = rom[registers[IP]];
	switch (op)
	{
	case Opcodes::NOOP:
		break;
	case Opcodes::DUMP:
		dump();
		break;
	case Opcodes::VBL:
		running = false;
		break;
	case Opcodes::LIT:
		lit();
		break;
	case Opcodes::CLR:
		clear();
		break;
	case Opcodes::JMP:
		jmp();
		break;
	case Opcodes::JGZ:
		jgz();
		break;
	case Opcodes::JLZ:
		jlz();
		break;
	case Opcodes::JEZ:
		jez();
		break;
	case Opcodes::JNZ:
		jnz();
		break;
	case Opcodes::POP:
		pop();
		break;
	case Opcodes::PUSH:
		push();
		break;
	case Opcodes::INC:
		inc();
		break;
	case Opcodes::DEC:
		dec();
		break;
	default:
		std::cout << "Opcode not recognized: " << (int)op << " at instruction " << registers[IP] << std::endl;
		error("Check your program");
		break;
	}
	registers[IP]++;
}

uint32_t Interpreter::getVbuffer()
{
	return (uint32_t)registers[VRAM] + (uint32_t)registers[OSET] * BANK_SIZE;
}

void Interpreter::ipush(uint8_t value)
{
	ram[registers[SP]] = value;
	registers[SP]++;
}

uint8_t Interpreter::ipop()
{
	registers[SP]--;
	return ram[registers[SP]];
}

void Interpreter::ipush16(uint16_t value)
{
	uint8_t low = (uint8_t)value;
	uint8_t high = (uint8_t)(value >> 8);
	ipush(high);
	ipush(low);
}

uint16_t Interpreter::ipop16()
{
	uint8_t low = ipop();
	return makeUint16(ipop(), low);
}

uint16_t Interpreter::makeUint16(uint8_t x, uint8_t y)
{
	uint16_t val = x;
	return (val << 8) + (uint16_t)y;
}

uint8_t Interpreter::getArg()
{
	registers[IP]++;
	return rom[registers[IP]];
}

void Interpreter::mov()
{
	uint8_t source = getArg();
	uint8_t dest = getArg();
	registers[dest] = registers[source];
}

void Interpreter::mul()
{
	registers[ACC] *= ipop16();
}

void Interpreter::add()
{
	registers[ACC] += ipop16();
}

void Interpreter::sub()
{
	registers[ACC] -= ipop16();
}

void Interpreter::neg()
{
	registers[ACC] = -(int16_t)registers[ACC];
}

void Interpreter::pop()
{
	uint8_t temp = ipop();
	uint16_t val = makeUint16(ipop(), temp);
	registers[IP]++;
	uint8_t reg = rom[registers[IP]];
	registers[reg] = val;
}

void Interpreter::push()
{
	registers[IP]++;
	uint8_t reg = rom[registers[IP]];
	ipush(registers[reg] >> 8 & 255);
	ipush(registers[reg] & 255);
}

void Interpreter::jmp()
{
	uint16_t val = makeUint16(rom[registers[IP] + 1], rom[registers[IP] + 2]);
	registers[IP] = val - 1;
}

void Interpreter::jez()
{
	registers[IP]++;
	uint8_t val = rom[registers[IP]];
	if (registers[val] == 0) { jmp(); return; }
	registers[IP] += 2;
}

void Interpreter::jlz()
{
	registers[IP]++;
	uint8_t val = rom[registers[IP]];
	if ((int16_t)registers[val] < 0) { jmp(); return; }
	registers[IP] += 2;
	//std::cout << registers[IP] << std::endl;
}

void Interpreter::jgz()
{
	registers[IP]++;
	uint8_t val = rom[registers[IP]];
	if ((int16_t)registers[val] > 0) { jmp(); return; }
	registers[IP] += 2;
}

void Interpreter::jnz()
{
	registers[IP]++;
	uint8_t val = rom[registers[IP]];
	if (registers[val] != 0) { jmp(); return; }
	registers[IP] += 2;
}

void Interpreter::inc()
{
	registers[IP]++;
	uint8_t val = rom[registers[IP]];
	registers[val]++;
}

void Interpreter::dec()
{
	registers[IP]++;
	uint8_t val = rom[registers[IP]];
	registers[val]--;
}

void Interpreter::clear()
{
	uint8_t value = ipop();
	ipop(); // discard high bit
	//std::cout << (int)value << std::endl;
	uint16_t pal = registers[PAL] + value * 3;
	//std::cout << (int)ram[pal] << " " << (int)ram[pal+1] << " " << (int)ram[pal+2] << std::endl;
	uint32_t vbuff = registers[VRAM]; //getVbuffer();
	for (size_t i = 0; i < 64000; i++)
	{
		ram[vbuff + i] = value;
		//std::cout << (int)ram[vbuff + i] << std::endl;
	}
}

void Interpreter::lit()
{
	registers[IP]++;
	ipush(rom[registers[IP]]);
	registers[IP]++;
	ipush(rom[registers[IP]]);
}

void Interpreter::dump()
{
	std::cout << "Registers:" << std::endl;
	for (size_t i = 0; i < 16; i++)
	{
		std::cout << "register " << i << "\t" << registers[i] << std::endl;
	}
	std::cout << "Stack" << std::endl;
	for (size_t i = stackBase; i < registers[SP]; i++)
	{
		std::cout << i - stackBase << "\t:\t" << (unsigned int)ram[i] << std::endl;
	}
}

void Interpreter::error(std::string message)
{
	if (hasError) return;
	hasError = true;
	running = false;
	errorString = message;
	std::cerr << message << std::endl;
}

void Interpreter::loadProgram(uint8_t* _rom)
{
	//std::cout << rom << std::endl;
	//std::cout << (_rom == nullptr) << std::endl;
	rom = _rom;
	reset();
	//std::cout << rom << std::endl;
}

void Interpreter::setVbuffer(unsigned char* data)
{
	// get mode
	int mode = 2;
	// get pallet location
	int pallet = registers[PAL];
	// get vbuffer location
	int vbuffer = registers[VRAM];
	size_t dataIndex = 0;
	for (size_t i = 0; i < 64000; i++)
	{
		uint16_t block = ram[i + vbuffer] * 3;
		data[dataIndex] = ram[pallet + block];
		data[dataIndex + 1] = ram[pallet + block + 1];
		data[dataIndex + 2] =  ram[pallet + block + 2];
		//std::cout << int(block) << " " << (int)data[dataIndex] << " " << (int)data[dataIndex] << " " << (int)data[dataIndex] << std::endl;
		dataIndex += 3;
	}
}

void Interpreter::reset()
{
	registers[VRAM] = 256 * 3;
	stackBase = 256 * 3 + 64000;
	registers[SP] = stackBase;
	size_t palIndex = 0;
	for (size_t i = 0; i < 6; i++)
	{
		for (size_t f = 0; f < 6; f++)
		{
			for (size_t g = 0; g < 6; g++)
			{
				ram[palIndex] = i * 42;
				ram[palIndex + 1] = f * 42;
				ram[palIndex + 2] = g * 42;
				palIndex += 3;
			}
		}
	}
}

Interpreter::Interpreter()
{
	ram = (uint8_t*)calloc(RAM_SIZE, sizeof(uint8_t));
	//reset();
	// Load default values to registers.
	//registers[VRAM] = 256 * 3;
	/*for (size_t i = 0; i < 64000; i++)
	{
		//ram[6 + i] = 15;
		ram[256 * 3 + i] = i % 256;
	}/**/
	//stackBase = 256 * 3 + 64000;
	//registers[SP] = stackBase;
	//ram[1] = 255;
	//ram[3] = 255;
	/*size_t palIndex = 0;
	for (size_t i = 0; i < 6; i++)
	{
		for (size_t f = 0; f < 6; f++)
		{
			for (size_t g = 0; g < 6; g++)
			{
				ram[palIndex] = i * 42;
				ram[palIndex + 1] = f * 42;
				ram[palIndex + 2] = g * 42;
				palIndex += 3;
			}
		}
	}
	/*for (size_t i = 0; i < 256*3; i+=3)
	{
		std::cout << i / 3 << " : " << (int)ram[i] << " " << (int)ram[i + 1] << " " << (int)ram[i + 2] << std::endl;
	}/**/
	/*uint16_t a = 1025;
	uint8_t b = a >> 8 & 255;
	uint8_t c = a & 255;
	//std::cout << (int)b << " " << (int)c << std::endl;
	/*rom = (uint8_t*)calloc(100, 1);
	rom[0] = Opcodes::LIT;
	rom[1] = 0;
	rom[2] = 255;
	rom[3] = Opcodes::POP;
	rom[4] = ACC; // saves the value to acc
	rom[5] = Opcodes::JLZ;
	rom[6] = 0;
	rom[7] = 16; // jumps to end
	rom[8] = Opcodes::DEC;
	rom[9] = Opcodes::PUSH;
	rom[10] = ACC;
	rom[11] = Opcodes::CLEAR;
	rom[12] = Opcodes::VBL;
	rom[13] = Opcodes::JMP;
	rom[14] = 0;
	rom[15] = 5;
	rom[16] = Opcodes::JMP;
	rom[17] = 0;
	rom[18] = 0;
	romSize = 100;*/
}

Interpreter::~Interpreter()
{
	free(ram);
}