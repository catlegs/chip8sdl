#include "Chip8Common.hpp"
#include "Video.hpp"

#include <cstring>

namespace Chip8 {
	// private methods
	void Video::allocateBuffers(VideoMode vidMode) {
		// grab the dimensions for this video mode
		const ScreenDimensions* dims = &VideoModeDimensions[vidMode];
		backBuffer = allocate2dBuffer(dims->width, dims->height);
		displayBuffer = allocate2dBuffer(dims->width, dims->height);
	}

	void Video::destroyBuffers(VideoMode vidMode) {
		// grab the dimensions for this video mode
		const ScreenDimensions* dims = &VideoModeDimensions[vidMode];
		destroy2dBuffer(backBuffer, dims->width);
		destroy2dBuffer(displayBuffer, dims->width);
	}

	void Video::pushBackBufferToDiplayBuffer() {
		bufferLock.lock();
		const ScreenDimensions* dims = &VideoModeDimensions[mode];
		copy2dBuffer(backBuffer, displayBuffer, dims->width, dims->height);
		bufferLock.unlock();
	}

	Video::Video(VideoDriver* vidDriver, VideoMode vidMode) {
		driver = vidDriver;
		mode = vidMode;
		allocateBuffers(mode);
	}
	
	Video::~Video() {
		destroyBuffers(mode);
	}

	//inline void setPixelAtPosition(u8 pixVal, unsigned int width, unsigned int height);
	// draw a sprite to the screen, if there was a collision, then return true.
	// if there was no collision, then return false.
	bool Video::drawSprite(u8** spriteData, unsigned int spriteWidth, unsigned int spriteHeight, unsigned int posX, unsigned int posY) {
		bool collision = false;
		for (unsigned int colPos = posX; colPos < posX + spriteWidth; ++colPos) {
			for (unsigned int rowPos = posY; rowPos < posY + spriteHeight; ++rowPos) {
				u8* ptr = &backBuffer[colPos][rowPos];
				u8 oldVal = *ptr;
				*ptr = oldVal ^ spriteData[colPos][rowPos];
				if (oldVal != *ptr) {
					collision = true;
				}
			}
		}
		return collision;
	}

	void Video::displayToScreen() {
		pushBackBufferToDiplayBuffer();

		// send the display buffer to the driver
		driver->updateScreen(displayBuffer, mode);
	}

	void Video::changeVideoMode(VideoMode vidMode) {
		bufferLock.lock();
		destroyBuffers(mode);
		allocateBuffers(vidMode);
		mode = vidMode;
		bufferLock.unlock();
	}
}
