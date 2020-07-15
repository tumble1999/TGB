#include "MemoryMap.h"

MemoryMap::MemoryMap(CPU* cpu)
	:cpu(cpu)
{
}

MemoryMap::~MemoryMap()
{
	delete cpu;
}

uint8_t& MemoryMap::operator[](GBuint16 addr)
{
	switch (addr & 0xF000) {
		// BIOS (256b)/ROM0
	case 0x0000:
		if (booting)
		{
			if (addr < 0x0100)
				return bios[addr];
			else if (cpu->get.pc == 0x0100)
				booting = 0;
		}

		return rom[addr];

		// ROM0
	case 0x1000:
	case 0x2000:
	case 0x3000:
		// ROM1 (unbanked) (16k)
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return rom[addr];

		// Graphics: VRAM (8k)
	case 0x8000:
	case 0x9000:
		return videoRam[addr & 0x1fff];

		// External RAM (8k)
	case 0xA000:
	case 0xB000:
		return saveData[addr & 0x1fff];

		// Working RAM (8k)
	case 0xC000:
	case 0xD000:
		return ram[addr & 0x1fff];
		// Working RAM shadow
	case 0xE000:
		return ram[addr & 0x1fff];

		// Working RAM shadow, I/O, Zero-page RAM
	case 0xF000:
		switch (addr & 0x0F00)
		{
			// Working RAM shadow
		case 0x000: case 0x100: case 0x200: case 0x300:
		case 0x400: case 0x500: case 0x600: case 0x700:
		case 0x800: case 0x900: case 0xA00: case 0xB00:
		case 0xC00: case 0xD00:
			return ram[addr & 0x1fff];

			// Graphics: object attribute memory
			// OAM is 160 bytes, remaining bytes read as 0
		case 0xE00:
			if (addr < 0xFEA0)
				return oam[addr & 0x1fff];
			else
				return NULL;

			// Zero-page
		case 0xF00:
			if (addr >= 0xFF80)
			{
				//return MMU._zram[addr & 0x7F];
			}
			else if (addr >= 0xFF40)
			{
				// GPU (64 registers)
				//return GPU.rb(addr);
			}
			else switch (addr & 0x3F)
			{
			//case 0x00: return KEY.rb();
			default: return 0;
			}
		}
	}
}
