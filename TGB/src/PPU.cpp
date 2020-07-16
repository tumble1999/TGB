#include "PPU.h"

PPU::PPU()
{
}

PPU::~PPU()
{
}
//VRam: 0x8000 - 0x9fff
void PPU::WriteVRAM(uint16_t address, uint8_t data)
{
}

uint8_t PPU::ReadVRAM(uint16_t address)
{
	return uint8_t();
}

//OAM:0xFE00 - 0xFEA0
void PPU::WriteOAM(uint16_t address, uint8_t data)
{
	uint8_t index = (address & 0xFC)>>2;
	if (index >= 40) return;
	sprites->setData(data);
}

uint8_t PPU::ReadOAM(uint16_t address)
{
	uint8_t index = address & 0xFC;
	if (index >= 40) return NULL;
	return sprites[index].getData(address);
}

//PPU FF40 - FF4F
void PPU::Write(uint16_t address, uint8_t data)
{
}

uint8_t PPU::Read(uint16_t address)
{
	switch (address & 0xf)
	{
	case 0://LCD Control
		uint8_t data = enableLCD;//7
		data = (data << 1) + window.tileMapAddress;//6
		data = (data << 1) + window.active;//5
		data = (data << 1) + bgWindowTileData;//4
		data = (data << 1) + background.tileMapAddress;//3
		data = (data << 1) + doubleHeightSprites;//2
		data = (data << 1) + showSprites;//1
		data = (data << 1) + background.active;//0
		return data;
	case 1://LCDC Status
		uint8_t status = 0;
		status = (status << 1);
		status = (status << 1);
		status = (status << 1);
		status = (status << 1);
		status = (status << 1);
		status = (status << 1);
		status = (status << 1) + mode;
		return status;
	case 2://ScrollY
		return background.scrollY;
	case 3://ScrollX
		return background.scrollX;
	case 4://LCDC Ycoord
	case 5://LY Compare
	case 6://DMA Transfer and start address
	case 7://BG Palette
	case 8://Sprite Pallette 0
		return spritePallets[0].getData();
	case 9://Sprite Pallette 1
		return spritePallets[1].getData();
	case 10://WindowX
		return window.x;
	case 11://WindowY
		return window.y;
	}
	return 0xff;
}
