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

#include "Chip8Common.hpp"
#include "Timers.hpp"

namespace Chip8 {
	// constructor
	Timers::Timers() {
		// initialize the timer values
		DT = 0;
		ST = 0;
		// set the thread pointer to null
		timerThread = NULL;
		// thread is NOT running initially
		runThread = false;
		// timers are also NOT paused
		paused = false;
	}

	// destructor
	Timers::~Timers() {
		// stop the timer thread
		stopTimers();
		// thread has completed, no more objects need
		//  to be destroyed.
	}

	void Timers::setDelayTimer(u8 value) {
		timerLock.lock();
		DT = value;
		timerLock.unlock();
	}

	u8 Timers::readDelayTimer() {
		return DT;
	}

	void Timers::setSoundTimer(u8 value) {
		timerLock.lock();
		ST = value;
		timerLock.unlock();
	}

	u8 Timers::readSoundTimer() {
		return ST;
	}

	// checks the status of the timer thread
	bool Timers::isStarted() {
		return runThread;
	}

	void Timers::startTimers() {
		runThread = true;
		if (timerThread == NULL) {
			// spawn thread to run the updateTimers() method
			std::function<bool()> func = std::bind(&Timers::updateTimers, this);
			timerThread = new std::thread(&invokeFuncAtInterval, func, 60L);
		}

	}

	void Timers::stopTimers() {
		// check to see if the thread has been started
		if (runThread) {
			// set the thread to complete.
			runThread = false;

			// wait for the thread to complete
			timerThread->join();
			delete timerThread;
		}
	}

	// method that will be ran
	//  inside the timer updating thread
	bool Timers::updateTimers() {
		

		// only update the timers if they are NOT paused
		if (!paused) {
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
		}
		
		

		// return whether or not thread should keep running
		return runThread;
	}

	void Timers::pauseTimers() {
		paused = true;
	}

	void Timers::unPauseTimers() {
		paused = false;
	}

	bool Timers::isPaused() {
		return paused;
	}

}