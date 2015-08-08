#ifndef CHIP8_KEYS_HPP
#define CHIP8_KEYS_HPP

#include <atomic>
#include "Chip8Common.hpp"

namespace Chip8 {
	const int NumberOfKeys = 16;

	class Keys {
	private:
		std::atomic_bool keyStates[NumberOfKeys];
	public:
		Keys();

		void setKeyAsPressed(u8 key);
		void releaseKey(u8 key);
		bool isKeyPressed(u8 key);

	};
}
#endif