#include "Chip8Manager.hpp"

Chip8Manager::Chip8Manager() {
	memoryManager = new Chip8Memory;
}

Chip8Manager::~Chip8Manager() {
	delete memoryManager;
}