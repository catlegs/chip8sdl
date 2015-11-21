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

	/**
	 * 
	 *
	 * @param buf an array that corresponds to the passed in VideoMode, is in row-major form
	 * @param vidMode the CHIP-8 video mode that dictates the size of the buffer
	 * @param posX the horizontal offset of the element
	 * @param posY the vertical offset of the element
	 * @return the element at the indicated position (mathematically, rowWidth * posY + posX)
	 */
	inline u8 getElementAt(u8* buf, VideoMode vidMode, unsigned int posX, unsigned int posY) {
		const ScreenDimensions* dims = &VideoModeDimensions[vidMode];
		return buf[dims->width * posY + posX];
	}

	/**
	 * 
	 *
	 * @param buf an array that corresponds to the passed in VideoMode, is in row-major form
	 * @param vidMode the CHIP-8 video mode that dictates the size of the buffer
	 * @param posX the horizontal offset of the element to set
	 * @param posY the vertical offset of the element to set
	 * @param val the value to set the element to
	 * @return the value that we set (equivalent to the parameter val)
	 */
	inline u8 setElementAt(u8* buf, VideoMode vidMode, unsigned int posX, unsigned int posY, u8 val) {
		const ScreenDimensions* dims = &VideoModeDimensions[vidMode];
		buf[dims->width * posY + posX] = val;
		return val;
	}

	const u8 PIXEL_ON = 1;
	const u8 PIXEL_OFF = 0;

	/**
	 * Interface for a video driver that should be implemented by the client and injected
	 * into the instance of Video that is used.
	 */
	class VideoDriver {
	public:
		/**
		 * Virtual destructor, can be overridden by implementation if needed.
		 */
		virtual ~VideoDriver() { /* do nothing */ }

		/**
		 * update the actual screen we're viewing with the contents of the buffer
		 * @param buf is a row-major array of size mode.height * mode.width
		 * to access an element in the buffer -
		 * buf[mode.width - 1][mode.height - 1] is the lower right corner of the screen
		 * the driver should not modify the passed in buffer, simply display it to the device/surface.
		 */
		virtual void updateScreen(u8* buf, VideoMode mode) = 0;
	};

	/**
	 * Class for handling video
	 */
	class Video {
	private:
		std::mutex bufferLock;
		VideoDriver* driver;
		VideoMode mode;
		u8* backBuffer;
		u8* displayBuffer;
		void pushBackBufferToDiplayBuffer();
		void allocateBuffers(VideoMode vidMode);
		void destroyBuffers();


	public:
		Video(VideoDriver* vidDriver, const VideoMode vidMode);
		~Video();

		// draw a sprite to the screen, if there was a collision, then return true.
		// if there was no collision, then return false.
		bool drawSprite(u8* spriteData, unsigned int spriteWidth, unsigned int spriteHeight, unsigned int posX, unsigned int posY);
		
		/**
		 * Pushes the current state of the screen to VideoDriver
		 */
		void displayToScreen();

		/**
		 * clears the screen.
		 */
		void clearScreen();

		/**
		 * changes the video mode (screen contents are not preserved)
		 *
		 * @param vidMode the new video mode
		 */
		void changeVideoMode(VideoMode vidMode);
	};
}

#endif
