#include "Chip8Common.hpp"
#include "Video.hpp"

#include <cstring>

namespace Chip8 {
	// private methods
	void Video::allocateBuffers(VideoMode vidMode) {
		// grab the dimensions for this video mode
		const ScreenDimensions* dims = &VideoModeDimensions[vidMode];
		unsigned int bufSize = dims->width * dims->height;
		backBuffer = new u8[bufSize];
		displayBuffer = new u8[bufSize];
		memset(backBuffer, 0, sizeof(u8) * bufSize);
		memset(displayBuffer, 0, sizeof(u8) * bufSize);
	}

	void Video::destroyBuffers() {
		delete[] backBuffer;
		delete[] displayBuffer;
	}

	void Video::pushBackBufferToDiplayBuffer() {
		bufferLock.lock();
		const ScreenDimensions* dims = &VideoModeDimensions[mode];
		memcpy(displayBuffer, backBuffer, sizeof(u8) * dims->width * dims->height);
		bufferLock.unlock();
	}

	Video::Video(VideoDriver* vidDriver, VideoMode vidMode) {
		driver = vidDriver;
		mode = vidMode;
		allocateBuffers(mode);
	}
	
	Video::~Video() {
		destroyBuffers();
	}

	//inline void setPixelAtPosition(u8 pixVal, unsigned int width, unsigned int height);
	// draw a sprite to the screen, if there was a collision, then return true.
	// if there was no collision, then return false.
	bool Video::drawSprite(u8* spriteData, unsigned int spriteWidth, unsigned int spriteHeight, unsigned int posX, unsigned int posY) {
		const ScreenDimensions* dims = &VideoModeDimensions[mode];
		bool collision = false;
		
		bufferLock.lock();
		u8* spritePtr = spriteData;
		u8* ptr = &backBuffer[dims->width * posY + posX];
		for (unsigned int i = 0; i < spriteHeight; ++i) {
			for (unsigned int j = 0; j < spriteWidth; ++j) {
				u8 oldVal = *ptr;
				*ptr = (*ptr) ^ (*spritePtr);

				if ((*ptr) == 0 && (*spritePtr) != 0) {
					collision = true;
				}
				// increment the pointers by one
				++spritePtr;
				++ptr;
			}
			// go to the next column
			ptr += dims->width - spriteWidth;
		}
		bufferLock.unlock();

		return collision;
	}

	void Video::displayToScreen() {
		pushBackBufferToDiplayBuffer();

		// send the display buffer to the driver
		driver->updateScreen(displayBuffer, mode);
	}

	void Video::changeVideoMode(VideoMode vidMode) {
		bufferLock.lock();
		destroyBuffers();
		allocateBuffers(vidMode);
		mode = vidMode;
		bufferLock.unlock();
	}
}
