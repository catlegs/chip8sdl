#ifndef CHIP8_VIDEO_HPP
#define CHIP8_VIDEO_HPP

#include <mutex>
#include "Chip8Common.hpp"

namespace Chip8 {
	enum VideoMode {
		vid_CHIP8 = 0,
		vid_SCHIP,
		// TODO vid_MCHIP (megachip-8 once instructions are supported)
	};
	
	struct ScreenDimensions {
		const unsigned int width;
		const unsigned int height;
	};

	const struct ScreenDimensions Chip8Dimensions = { 64, 32 };
	const struct ScreenDimensions SChipDimensions = { 128, 64 };

	const ScreenDimensions VideoModeDimensions[] = { Chip8Dimensions, SChipDimensions };

	const u8 PIXEL_ON = 1;
	const u8 PIXEL_OFF = 0;

	// interface
	class VideoDriver {
	public:
		virtual ~VideoDriver() { /* do nothing */ }
		// update the actual screen we're viewing with the contents of the buffer
		// buf[0][0] is the upper left corner of the screen
		// buf[mode.width - 1][mode.height - 1] is the lower right corner of the screen
		// the driver should not modify the passed in buffer, simply display it to the device/surface.
		virtual void updateScreen(u8** buf, VideoMode mode) = 0;
	};

	class Video {
	private:
		std::mutex bufferLock;
		VideoDriver* driver;
		VideoMode mode;
		u8** backBuffer;
		u8** displayBuffer;
		void pushBackBufferToDiplayBuffer();
		void allocateBuffers(VideoMode vidMode);
		void destroyBuffers(VideoMode vidMode);


	public:
		Video(VideoDriver* vidDriver, const VideoMode vidMode);
		~Video();

		//inline void setPixelAtPosition(u8 pixVal, unsigned int width, unsigned int height);
		// draw a sprite to the screen, if there was a collision, then return true.
		// if there was no collision, then return false.
		bool drawSprite(u8** spriteData, unsigned int spriteWidth, unsigned int spriteHeight, unsigned int posX, unsigned int posY);
		void displayToScreen();
		// changes the video mode (screen contents are not preserved)
		void changeVideoMode(VideoMode vidMode);
	};
}

#endif
