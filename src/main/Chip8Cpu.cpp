/*
Copyright (c) 2015 Ross Keenum (rosskeenum@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Chip8Cpu.hpp"

Chip8Cpu::Chip8Cpu(Chip8Memory &memManager, Chip8Timers &timManager, u16 frequency) {
	// zero out the registers
	for (int i = 0; i < NumberOfRegs; ++i) {
		V[i] = 0;
	}

	// clear the stack
	for (int i = 0; i < StackDepth; ++i) {
		stack[i] = 0;
	}

	// set the memory addressing register
	I = 0;

	// PC starts at 0x200
	PC = 0x200;

	// stack pointer is initially set at 0
	SP = 0;

	// set the cpu frequency
	freq = frequency;
	
	// set the other managers that this object
	// holds a reference to.
	memoryManager = &memManager;
	timerManager = &timManager;
}

u8 Chip8Cpu::readRegister(u8 n) {
	return n < NumberOfRegs ?
		V[n] :
		0;
}

u16 Chip8Cpu::readMemAddrReg() {
	return I;
}

u16 Chip8Cpu::readProgCounter() {
	return PC;
}

u8 Chip8Cpu::readStackPointer() {
	return SP;
}

u16 Chip8Cpu::readAddressOnStack() {
	// TODO error handling
	return SP == 0 || SP > StackDepth ?
		0 :
		stack[SP - 1];
}

void Chip8Cpu::pushToStack(u16 addr) {
	// TODO error handling
	stack[SP] = addr;
	++SP;
}

u16 Chip8Cpu::popFromStack() {
	// TODO error handling
	--SP;
	return stack[SP];
}

void Chip8Cpu::executeInstruction() {
	// TODO implement
	
	// read the instruction that the program counter is pointing to
	u16 instruction = memoryManager->readWord(PC);

	// retrieve the most significant nibble
	u8 msn = getHighNibble(getHighByte(instruction));

	// on whether or not to increment the program counter
	bool incProgramCounter = true;

	switch (msn) {
	case 0x0:
		if (getLowNibble(getHighByte(instruction)) == 0) {

			u8 lsb = getLowByte(instruction);

			switch (lsb) {
			case 0xE0:
				//00E0 - CLS
				//Clear the display.
				// TODO call video manager for this
				break;

			case 0xEE:
				//00EE - RET
				//Return from a subroutine.
				//The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
				PC = popFromStack();
				// DO increment the PC after this instruction, since stored address is previous address
				//  before calling the subroutine, else the routine would get called infinitely
				break;

			default:
				// INVALID
				// TODO error handling

			}
		}
		else {
			//0nnn - SYS addr
			//Jump to a machine code routine at nnn.
			//
			//This instruction is only used on the old computers on which Chip - 8 was originally implemented.It is ignored by modern interpreters.

			// intentionally blank.
		}
		break;

	case 0x1:
		// 1nnn - JP addr
		// Jump to location nnn.
		// The interpreter sets the program counter to nnn.
		PC = instruction & 0xFFF;

		// do not increment the program counter
		incProgramCounter = false;
		break;

	case 0x2:
		// 2nnn - CALL addr
		// Call subroutine at nnn.
		// The interpreter increments the stack pointer, then puts the current PC on the top of the stack.The PC is then set to nnn.
		pushToStack(PC);
		
		PC = instruction & 0xFFF;

		incProgramCounter = false;
		break;

	case 0x3:
		//3xkk - SE Vx, byte
		//Skip next instruction if Vx = kk.
		//The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
		u8 reg = getLowNibble(getHighByte(instruction));
		u8 byteVal = getLowByte(instruction);

		if (V[reg] == byteVal) {
			PC += 2;
		}
		break;

	case 0x4:
		//
		//4xkk - SNE Vx, byte
		//Skip next instruction if Vx != kk.
		//The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
		u8 reg = getLowNibble(getHighByte(instruction));
		u8 byteVal = getLowByte(instruction);

		if (V[reg] != byteVal) {
			PC += 2;
		}
		break;

	case 0x5:
		//5xy0 - SE Vx, Vy
		//Skip next instruction if Vx = Vy.
		//The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.

		// assuming least significant nibble is 0 for now (should this be ignored?)
		u8 reg1 = getLowNibble(getHighByte(instruction));
		u8 reg2 = getHighNibble(getLowByte(instruction));

		if (V[reg1] == V[reg2]) {
			PC += 2;
		}
		break;

	case 0x6:
		//6xkk - LD Vx, byte
		//Set Vx = kk.
		//The interpreter puts the value kk into register Vx.
		u8 reg = getLowNibble(getHighByte(instruction));
		u8 byteVal = getLowByte(instruction);

		V[reg] = byteVal;
		break;

	case 0x7:
		//7xkk - ADD Vx, byte
		//Set Vx = Vx + kk.
		u8 reg = getLowNibble(getHighByte(instruction));
		u8 byteVal = getLowByte(instruction);

		V[reg] = V[reg] + byteVal;
		break;

	case 0x8:
		break;

	case 0x9:
		//9xy0 - SNE Vx, Vy
		//Skip next instruction if Vx != Vy.
		//The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.

		// assuming least significant nibble is 0 for now (should this be ignored?)
		u8 reg1 = getLowNibble(getHighByte(instruction));
		u8 reg2 = getHighNibble(getLowByte(instruction));

		if (V[reg1] != V[reg2]) {
			PC += 2;
		}
		break;

	case 0xA:
		//Annn - LD I, addr
		//Set I = nnn.
		//The value of register I is set to nnn.
		I = instruction & 0xFFF;
		break;

	case 0xB:
		//Bnnn - JP V0, addr
		//Jump to location nnn + V0.
		//The program counter is set to nnn plus the value of V0.
		u16 addr = instruction & 0xFFF;
		PC = addr + u16(V[0]);
		incProgramCounter = false;
		break;

	case 0xC:
		//Cxkk - RND Vx, byte
		//Set Vx = random byte AND kk
		//The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk.The results are stored in Vx.See instruction 8xy2 for more information on AND.
		break;

	case 0xD:
		// Dxyn - DRW Vx, Vy, nibble
		// Display n - byte sprite starting at memory location I at(Vx, Vy), set VF = collision.
		//  The interpreter reads n bytes from memory, starting at the address stored in I.
		//  These bytes are then displayed as sprites on screen at coordinates(Vx, Vy).
		//  Sprites are XORed onto the existing screen.If this causes any pixels to be erased,
		//  VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside
		//  the coordinates of the display, it wraps around to the opposite side of the screen.See instruction
		//  8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip - 8 screen and sprites.
		//    TODO call video manager for this
		break;

	







	case 0xE:
		break;

	case 0xF:
		u8 lsb = getLowByte(instruction);

		switch (lsb) {

		case 0x07:
			//Fx07 - LD Vx, DT
			//Set Vx = delay timer value.
			//
			//The value of DT is placed into Vx.

			break;

		case 0x0A:
			//
			//Fx0A - LD Vx, K
			//Wait for a key press, store the value of the key in Vx.
			//
			//All execution stops until a key is pressed, then the value of that key is stored in Vx.

			break;

		case 0x15:
			//
			//Fx15 - LD DT, Vx
			//Set delay timer = Vx.
			//
			//DT is set equal to the value of Vx.

			break;

		case 0x18:
			//
			//Fx18 - LD ST, Vx
			//Set sound timer = Vx.
			//
			//ST is set equal to the value of Vx.

			break;

		case 0x1E:
			//
			//Fx1E - ADD I, Vx
			//Set I = I + Vx.
			//
			//The values of I and Vx are added, and the results are stored in I.
			break;
		case 0x29:
			//
			//Fx29 - LD F, Vx
			//Set I = location of sprite for digit Vx.
			//
			//The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.See section 2.4, Display, for more information on the Chip - 8 hexadecimal font.

			break;

		case 0x33:
			// Fx33 - LD B, Vx
			// Store BCD representation of Vx in memory locations I, I + 1, and I + 2.
			//
			// The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I + 1, and the ones digit at location I + 2.
			u8 reg = getLowNibble(getHighByte(instruction));
			u8 hundreds = V[reg] / 100;
			u8 tens = (V[reg] % 100) / 10;
			u8 ones = V[reg] % 10;

			memoryManager->writeByte(I, hundreds);
			memoryManager->writeByte(I + 1, tens);
			memoryManager->writeByte(I + 2, ones);

			break;

		case 0x55:
		//
		//Fx55 - LD[I], Vx
		//Store registers V0 through Vx in memory starting at location I.
		//
		//The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
		
			break;

		case 0x65:
		//
		//Fx65 - LD Vx, [I]
		//Read registers V0 through Vx from memory starting at location I.
		//
		//The interpreter reads values from memory starting at location I into registers V0 through Vx.
		
			break;

		default:
			// TODO error handling
	
		}

	}

	// increment the program counter
	if (incProgramCounter) {
		PC += 2;
	}
}







//
//
//

//

//Adds the value kk to the value of register Vx, then stores the result in Vx.
//
//8xy0 - LD Vx, Vy
//Set Vx = Vy.
//
//Stores the value of register Vy in register Vx.
//
//
//8xy1 - OR Vx, Vy
//Set Vx = Vx OR Vy.
//
//Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
//
//
//8xy2 - AND Vx, Vy
//Set Vx = Vx AND Vy.
//
//Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0.
//
//
//8xy3 - XOR Vx, Vy
//Set Vx = Vx XOR Vy.
//
//Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
//
//
//8xy4 - ADD Vx, Vy
//Set Vx = Vx + Vy, set VF = carry.
//
//The values of Vx and Vy are added together.If the result is greater than 8 bits(i.e., > 255, ) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
//
//
//8xy5 - SUB Vx, Vy
//Set Vx = Vx - Vy, set VF = NOT borrow.
//
//If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
//
//
//8xy6 - SHR Vx{ , Vy }
//Set Vx = Vx SHR 1.
//
//If the least - significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
//
//
//8xy7 - SUBN Vx, Vy
//Set Vx = Vy - Vx, set VF = NOT borrow.
//
//If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
//
//
//8xyE - SHL Vx{ , Vy }
//Set Vx = Vx SHL 1.
//
//If the most - significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
//
//

//
//Ex9E - SKP Vx
//Skip next instruction if key with the value of Vx is pressed.
//
//Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
//
//
//ExA1 - SKNP Vx
//Skip next instruction if key with the value of Vx is not pressed.
//
//Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
//
//

