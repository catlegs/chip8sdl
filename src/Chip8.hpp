#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <SDL/SDL.h>
#include "Chip8Video.hpp"

class Chip8 {
  private:
    // registers
    Uint8 V[16]; // general purpose registers
    Uint16 I;    // memory addressing register
    Uint16 PC;   // program counter
    Uint8 SP;    // stack pointer
    
    // timers
    Uint8 DT; // delay timer
    Uint8 ST; // sound timer
    
    // memory
    Uint16 stack[16];     // stack 16 levels deep
    Uint8 memory[0x1000]; // 4k of memory
    
    // input
    Uint8 keys[0x10];
    
    // hp48 flags (needed for 2 instructions)
    Uint8 hp48flags[0x8];
    
    // video object
    Chip8Video* vid;
    
    // how many instructions per second this will execute
    unsigned int freq;
    
  public:

    /**
     * Constructor
     *
     * PARAMETERS
     * ----------
     * [required]
     * path - a char array that represents the file path of the program to be loaded
     *
     * [optional]
     * freq - how many instructions per second this core will execute
     * bgColor - the background color in 0xRRGGBB format
     * fgColor - the foreground color in 0xRRGGBB format
     */
    Chip8(const char* path, unsigned int freq, Uint32 bgColor, Uint32 fgColor);
    
    /**
     * Method that checks if a key is pressed.
     * 
     * PARAMETERS
     * ----------
     * keyToCheck - the key to check
     *
     * RETURNS
     * -------
     * true if key is pressed, false if not.
     * If keyToCheck is > 0xF, then false is always returned.
     */
    bool isKeyPressed(Uint8 keyToCheck);
    
    /**
     * Set a key as pressed. If keyToSet > 0xF, then this method does nothing
     *
     * PARAMETERS
     * ----------
     * keyToSet - the key to set as 
     */
    void setKeyAsPressed(Uint8 keyToSet);
    
    /**
     * Clears all of the key presses.
     */
    void clearKeyPresses();
}

#endif // CHIP8_HPP