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
