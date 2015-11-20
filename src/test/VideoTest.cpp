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

#include "catch.hpp"
#include "Chip8Common.hpp"
#include "Video.hpp"

// for memcpy
#include <cstring>

using Chip8::Video;
using Chip8::VideoMode;
using Chip8::u8;

// driver implementation that simply copys the buffer
class TestVideoDriver : public Chip8::VideoDriver {
public:
	u8* outputBuf;
	VideoMode mode;

	TestVideoDriver(VideoMode vidMode) {
		const Chip8::ScreenDimensions* dims = &Chip8::VideoModeDimensions[vidMode];
		outputBuf = new u8[dims->width * dims->height];
		mode = vidMode;
	}
	
	~TestVideoDriver() {
		delete[] outputBuf;
	}

	void updateScreen(u8* buf, VideoMode vidMode) {
		const Chip8::ScreenDimensions* dims = &Chip8::VideoModeDimensions[vidMode];
		
		if (mode != vidMode) {
			delete[] outputBuf;
			outputBuf = new u8[dims->width * dims->height];
			mode = vidMode;
		}

		// copy the buffer
		memcpy(outputBuf, buf, sizeof(u8) * dims->width * dims->height);
	}
};

// TODO WRITE TESTS
TEST_CASE("Video/spriteDrawing") {
	TestVideoDriver driver(Chip8::vid_CHIP8);
	Video obj(&driver, Chip8::vid_CHIP8);

	u8 simpleSprite[3 * 3] =
	{
		0, 1, 0,
		1, 0, 1,
		1, 0, 1
	};
	
	SECTION("checkToMakeSureItIsFlushedToScreen") {
		obj.drawSprite(simpleSprite, 3, 3, 12, 15);
		obj.displayToScreen();

		// first row
		REQUIRE(Chip8::getElementAt(driver.outputBuf, Chip8::vid_CHIP8, 12, 15) == 0);
		REQUIRE(Chip8::getElementAt(driver.outputBuf, Chip8::vid_CHIP8, 13, 15) == 1);
		REQUIRE(Chip8::getElementAt(driver.outputBuf, Chip8::vid_CHIP8, 14, 15) == 0);
		// second row
		REQUIRE(Chip8::getElementAt(driver.outputBuf, Chip8::vid_CHIP8, 12, 16) == 1);
		REQUIRE(Chip8::getElementAt(driver.outputBuf, Chip8::vid_CHIP8, 13, 16) == 0);
		REQUIRE(Chip8::getElementAt(driver.outputBuf, Chip8::vid_CHIP8, 14, 16) == 1);
		// third row
		REQUIRE(Chip8::getElementAt(driver.outputBuf, Chip8::vid_CHIP8, 12, 17) == 1);
		REQUIRE(Chip8::getElementAt(driver.outputBuf, Chip8::vid_CHIP8, 13, 17) == 0);
		REQUIRE(Chip8::getElementAt(driver.outputBuf, Chip8::vid_CHIP8, 14, 17) == 1);
	}

}
