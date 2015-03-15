#include "catch.hpp"
#include "Chip8Common.hpp"

#include <thread>
#include <chrono>
// test function + values for the function we're testing
bool testFuncBreakVal;
int testFuncCountVal;

bool testFunc() {
	if (testFuncBreakVal) {
		++testFuncCountVal;
	}

	return testFuncBreakVal;
}

void sleepForMillis(long millis) {
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

// using 20 executions per second in these tests
// the first execution is executed right when the thread is created,
//   so we're sleeping for 25ms less in this thread
TEST_CASE("Chip8Common/invokeFuncAtInterval", "[threading]") {
	testFuncBreakVal = true;
	testFuncCountVal = 0;

	SECTION("Should be executed once") {
		std::thread t(&invokeFuncAtInterval, &testFunc, 20L);
		sleepForMillis(25);
		testFuncBreakVal = false;
		t.join();

		REQUIRE(testFuncCountVal == 1);
	}

	SECTION("Should be executed twice") {
		std::thread t(&invokeFuncAtInterval, &testFunc, 20L);
		sleepForMillis(75);
		testFuncBreakVal = false;
		t.join();

		REQUIRE(testFuncCountVal == 2);
	}
}