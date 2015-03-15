
#include "catch.hpp"
#include "Chip8Timers.hpp"
#include <iostream>

#include <thread>
#include <chrono>
#include <iostream>

TEST_CASE("Chip8Timers/constructor") {
	Chip8Timers obj;

	SECTION("Delay timer should be zero") {
		u8 result = obj.readDelayTimer();

		REQUIRE(result == 0);
	}

	SECTION("Sound timer should be zero") {
		u8 result = obj.readSoundTimer();

		REQUIRE(result == 0);
	}

	SECTION("Thread should not be started") {
		bool result = obj.isStarted();

		REQUIRE(result == false);
	}
}

TEST_CASE("Chip8Timers/updateTimers") {
	Chip8Timers obj;

	SECTION("timers are set to zero") {
		obj.updateTimers();

		u8 DTresult = obj.readDelayTimer();
		u8 STresult = obj.readSoundTimer();

		REQUIRE(DTresult == 0);
		REQUIRE(STresult == 0);
	}
}