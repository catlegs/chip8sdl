#include "Chip8Common.hpp"
#include <chrono>
#include <thread>

// invokes a passed in function at the specified times per second until it returns false
void invokeFuncAtInterval(std::function<bool()> func, unsigned long timesPerSecond) {
	// get the duration between the invocations in nanoseconds
	std::chrono::nanoseconds interval(1000000000ULL / timesPerSecond);

	// priming read for the next invocation of the function
	auto nextInvocation = std::chrono::high_resolution_clock::now();

	// infinite loop until the function returns false
	while (func()) {
		// function returned true
		// calculate when the next time to invoke it is
		nextInvocation += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(interval);

		// put the thread to sleep until the nex invocation
		std::this_thread::sleep_until(nextInvocation);
	}
}