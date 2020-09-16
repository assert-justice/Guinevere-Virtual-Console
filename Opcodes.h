#pragma once
#include <stdint.h>

namespace Opcodes
{
	constexpr uint8_t NOOP = 0;
	// Arithmatic
	constexpr uint8_t ADD  = 10; // Pops a value off the stack and adds it to the acc.
	constexpr uint8_t SUB  = 11; // Pops a value off the stack and subtracts it from the acc.
	constexpr uint8_t NEG  = 12; //
	constexpr uint8_t MUL  = 13; //
	constexpr uint8_t DIV  = 14; //
	constexpr uint8_t DIVS = 15; //
	constexpr uint8_t MOD  = 16; //
	constexpr uint8_t MODS = 17; //
	constexpr uint8_t INC = 18; // Increments the value of a given register.
	constexpr uint8_t DEC = 19; // Decrements the value of a given register.

	// Bitwise
	constexpr uint8_t AND = 20; //
	constexpr uint8_t OR  = 21; //
	constexpr uint8_t XOR = 22; //
	constexpr uint8_t NOT = 23; //
	constexpr uint8_t ASL = 24; // Arithmetic shift left. Pops a value from the stack and shifts the contents of a given register left by that value.
	constexpr uint8_t ASR = 25; // Arithmetic shift right. Pops a value from the stack and shifts the contents of a given register right by that value.

	// Stack
	constexpr uint8_t DUP  = 30; // Duplicates the value at the top of the stack and pushes it.
	constexpr uint8_t PUSH = 31; // Pushes the contents of a register to the stack.
	constexpr uint8_t POP  = 32; // Pops the value at the top of a stack and stores it in a register.
	constexpr uint8_t LIT  = 33; // Pushes a literal value to the stack.
	constexpr uint8_t JSR  = 34; // Jump to subroutine. Pushes the current value of the ip to the stack and sets it to a given value.
	constexpr uint8_t RTS  = 35; // Return from subroutine. Pops a value from the stack and sets the ip to that value.
	constexpr uint8_t MOV  = 36; // Move 

	// Memory
	constexpr uint8_t LOAD  = 40; // Accepts a register and pops a value from the stack, then pushes the value to the location plus the offset in memory.
	constexpr uint8_t SAVE  = 41; // Pops a short pointer and a value from the stack and copies the value to the location plus the offset in memory.
	constexpr uint8_t MCPY  = 42; // Pops a length and two pointers from the stack and copies the values from one to the other. Takes the offset into account
	constexpr uint8_t SET   = 43; // Pops a value and a length from the stack and a pointer from a register.

	// Control flow
	constexpr uint8_t JMP = 70; // Unconditonal jump, setting the ip register to a value.
	constexpr uint8_t JEZ = 71; // Conditionally jumps if a given register is zero.
	constexpr uint8_t JLZ = 72; //
	constexpr uint8_t JGZ = 73; //
	constexpr uint8_t JNZ = 74; //

	// Miscelaneous
	constexpr uint8_t VBL   = 50; // Pauses execution until the end of the next vblank.
	constexpr uint8_t DUMP  = 51; // Logs the current state of the console for debugging purposes.
	constexpr uint8_t SAVEG = 52; // Pops a length and a long pointer from the stack and saves the values at that location to the memory card.
	constexpr uint8_t LOADG = 53; // Pops a length and a long pointer from the stack and loads the values at that location from the memory card.
	constexpr uint8_t SAVES = 54; // Saves a snapshot of the console's state to the memory card.
	constexpr uint8_t LOADS = 55; // Loads a snapshot of the console's state from the memory card.
	constexpr uint8_t RESET = 56; // Resets the console.
	constexpr uint8_t HALT  = 56; // Halts execution as if an error was reached.

	// Utility
	constexpr uint8_t CLR   = 60; // Pops a value and sets the entire vbuffer to that value.
	constexpr uint8_t SPR   = 61; // Helper method that draws a sprite. Pops x, y, width, and height values as well as a long pointer and copies it to the vbuffer.
	constexpr uint8_t LOADV = 62; // Pops a length and pointer from the stack and pushes the given number of values from the vbuffer. Takes the offset into account.
	constexpr uint8_t SAVEV = 63; // Pops a length and pointer from the stack and pops that many values, copying them to the vbuffer. Takes the offset into account.
}