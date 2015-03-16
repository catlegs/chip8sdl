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


#ifndef CHIP8TIMERS_HPP
#define CHIP8TIMERS_HPP

#include <thread>
#include <mutex>
#include "Chip8Common.hpp"

class Chip8Timers {
private:
	std::mutex timerLock;

	u8 DT; // delay timer
	u8 ST; // sound timer
	bool runThread;
	std::thread *timerThread;

public:
	// constructor
	Chip8Timers();

	// destructor
	~Chip8Timers();

	void setDelayTimer(u8 value);
	u8 readDelayTimer();
	void setSoundTimer(u8 value);
	u8 readSoundTimer();

	// checks the status of the timer thread
	bool isStarted();

	void startTimers();
	void stopTimers();

	// method method that will be ran
	//  inside the timer updating thread
	bool updateTimers();
};

#endif
