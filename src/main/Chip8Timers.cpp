
#include "Chip8Timers.hpp"

	// constructor
Chip8Timers::Chip8Timers() {
		// initialize the timer values
		DT = 0;
		ST = 0;
		// set the thread pointer to null
		timerThread = NULL;
		// thread is NOT running initially
		runThread = false;
	}

	// destructor
Chip8Timers::~Chip8Timers() {
		// stop the timer thread
		stopTimers();
		// thread has completed, no more objects need
		//  to be destroyed.
	}

void Chip8Timers::setDelayTimer(u8 value) {
		timerLock.lock();
		DT = value;
		timerLock.unlock();
	}

u8   Chip8Timers::readDelayTimer() {
		return DT;
	}

void Chip8Timers::setSoundTimer(u8 value) {
		timerLock.lock();
		ST = value;
		timerLock.unlock();
	}

u8   Chip8Timers::readSoundTimer() {
		return ST;
	}

	// checks the status of the timer thread
bool Chip8Timers::isStarted() {
		return runThread;
	}

void Chip8Timers::startTimers() {
		runThread = true;
		if (timerThread == NULL) {
			// spawn thread to run the updateTimers() method
			std::function<bool()> func = std::bind(&Chip8Timers::updateTimers, this);
			timerThread = new std::thread(&invokeFuncAtInterval, func, 60L);
		}

	}

void Chip8Timers::stopTimers() {
		// check to see if the thread has been started
		if (runThread) {
			// set the thread to complete.
			runThread = false;
			// wait for the thread to complete
			timerThread->join();
            delete timerThread;
		}
	}

	// method method that will be ran
	//  inside the timer updating thread
bool Chip8Timers::updateTimers() {
		// lock the timers to modifications
		timerLock.lock();
		// if timers are 0, set to zero
		// else > 0, decrement by 1
		DT = DT == 0 ?
			0 :
			DT - 1;
		ST = ST == 0 ?
			0 :
			ST - 1;
		timerLock.unlock();

		// return whether or not thread should keep running
		return runThread;
	}