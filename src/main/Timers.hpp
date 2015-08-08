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


#ifndef CHIP8_TIMERS_HPP
#define CHIP8_TIMERS_HPP

#include <thread>
#include <mutex>
#include <atomic>
#include "Chip8Common.hpp"

namespace Chip8 {
	class Timers {
	private:
		std::mutex timerLock;

		u8 DT; // delay timer
		u8 ST; // sound timer
		std::atomic_bool runThread;
		std::atomic_bool paused; // whether or not the timers are paused

		std::thread *timerThread;

	public:
		Timers();
		~Timers();

		void setDelayTimer(u8 value);
		u8 readDelayTimer();
		void setSoundTimer(u8 value);
		u8 readSoundTimer();

		// checks the status of the timer thread
		bool isStarted();
		bool isPaused();

		void startTimers();
		void stopTimers();

		void pauseTimers();
		void unPauseTimers();

		// method method that will be ran
		//  inside the timer updating thread
		bool updateTimers();
	};
}

#endif
