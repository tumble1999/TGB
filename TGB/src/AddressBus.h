#ifndef ADDRESSBUS_H_
#define _ADDRESSBUS_H_
#include <cstdint>

#include "ROM.h"
#include "GBuint16.h"


static class AddressBus {
private:

	/*
	Memory Map:
	0x0000-0x3FFF	16KB ROM bank 00
	0x4000-0x7FFF	16KB ROM Bank 01~NN

	0x8000-0x9FFF	8KB Video RAM (VRAM)

	0xA000-0xBFFF	8KB External RAM	In cartridge, switchable bank if any

	0xC000-0xCFFF	4KB Work RAM (WRAM) bank 0
	0xD000-0xDFFF	4KB Work RAM (WRAM) bank 1~N	Only bank 1 in Non-CGB mode Switchable bank 1~7 in CGB mode
	0xE000-0xFDFF	Mirror of C000~DDFF (ECHO RAM)	Typically not used
	0xFE00-0xFE9F	Sprite attribute table (OAM)
	0xFEA0-0xFEFF	Not Usable
	0xFF00-0xFF7F	I/O Registers
	0xFF80-0xFFFE	High RAM (HRAM)
	0xFFFF-0xFFFF	Interrupts Enable Register (IE)
	*/

	bool booting;
	class CPU* cpu;
	ROM* bios;
	ROM* rom;

	uint8_t videoRam[0x2000];
	uint8_t* saveData[0x2000];
	uint8_t ram[0x2000];

	uint8_t oam[0xa0];
	uint8_t io[0x80];
	uint8_t hram[0x7f];
	uint8_t interupts;

	const uint8_t invalidData = UINT8_MAX;
	
public:

};

#endif // !_ADDRESS_H_
