#ifndef CHIP8MANAGER_HPP
#define CHIP8MANAGER_HPP

#include "Chip8Memory.hpp"
class Chip8Manager {
private:
	Chip8Memory* memoryManager;

public:
	Chip8Manager();
	~Chip8Manager();
};

#endif // CHIP8MANAGER_HPP