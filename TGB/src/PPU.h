#ifndef _PPU_H_
#define _PPU_H_

#include <cstdint>

static const int PALLET_SIZE = 4;
static const int TILE_SIZE = 8 * 8;

enum class Color : uint8_t {
	WHITE,
	LIGHT_GREY,
	DARK_GREY,
	BLACK
};

enum class PPUMode : uint8_t {
	H_BLANK,
	V_BLANK,
	OAM_SEARCH,
	PIXEL_TRANS
};
/*
https://gbdev.gg8.se/wiki/articles/Video_Display#LCD_Monochrome_Palettes
Bit 7-6 - Shade for Color Number 3
Bit 5-4 - Shade for Color Number 2
Bit 3-2 - Shade for Color Number 1
Bit 1-0 - Shade for Color Number 0
*/
class Pallet {
private:
	uint8_t id;
	Color colors[PALLET_SIZE];
public:
	Pallet(uint8_t data) {
		setData(data);
	}

	~Pallet() {
		delete[] colors;
	}

	Color* getColors() {
		return colors;
	}

	uint8_t getData() {
		uint8_t data = 0;
		for (int i = 0; i < 4; i++)
		{
			data = (data << 2) + (uint8_t)colors[i];
		}

	}

	void setData(uint8_t data) {

		for (int i = 0; i < 4; i++)
		{
			colors[i] = (Color)((data << 2 * i) & 0xff);
		}
	}
};

/*
https://gbdev.gg8.se/wiki/articles/Video_Display#VRAM_Tile_Data
Byte 0-1  First Line (Upper 8 pixels)
Byte 2-3  Next Line
etc.
*/
class Tile {
private:
	uint8_t id;
	Color colors[TILE_SIZE];
public:
	uint8_t getData() {
		uint8_t data = 0;
		for (int i = 0; i < 4; i++)
		{
			data = (data << 2) + (uint8_t)colors[i];
		}

	}
	void setData(uint8_t data) {

		for (int i = 0; i < 4; i++)
		{
			colors[i] = (Color)((data << 2 * i) & 0xff);
		}
	}

	uint8_t getID() {return id;}
	Color* getColors() {return colors;}
};

/*
https://gbdev.gg8.se/wiki/articles/Video_Display#VRAM_Sprite_Attribute_Table_.28OAM.29
Byte0 Y Position
Byte1 X Position
Byte2 Tile Number
Byte3 Flags
	Bit7   OBJ-to-BG Priority (0=OBJ Above BG, 1=OBJ Behind BG color 1-3)
		(Used for both BG and Window. BG color 0 is always behind OBJ)
	Bit6   Y flip          (0=Normal, 1=Vertically mirrored)
	Bit5   X flip          (0=Normal, 1=Horizontally mirrored)
	Bit4   Palette number  (0=OBP0, 1=OBP1)
*/
class Sprite {
private:
	uint8_t id,x,y;
	Tile* tile;
	bool priority,flipX,flipY,pallet;
public:
	uint8_t getData(uint16_t addr) {
		switch (addr & 0b11)
		{
		case 0: return y;
		case 1: return x;
		case 2: return tile->getID();
		case 3:
			uint8_t data = priority;
			data = (data << 1) + flipY;
			data = (data << 1) + flipX;
			data = (data << 1) + pallet;
			return data;
		}

	}

	void setData(uint16_t addr,uint8_t data) {
		switch (addr&0b11)
		{
		case 0:
			y = data;
			break;
		case 1:
			x = data;
		case 2:
			//TODO: Get Tile from PPU
		case 3:
			pallet = data & 0b0001;
			flipY = (data & 0b0010) >> 1;
			flipX = (data & 0b0100) >> 2;
			priority = (data & 0b1000) >> 3;
		}
	}

	uint8_t getID() {return id;}
	uint8_t getX() { return x; }
	uint8_t getY() {return y;}
	Tile* getTile() {return tile;}
	bool getPriority() {return priority;}
	bool getFlipX() {return flipX;}
	bool getFlipY() {return flipY;}
	bool getPallet() {return pallet;}
};

struct Background {
	uint8_t scrollX,scrollY;
	bool 
		active,
		tileMapAddress; // 0=9800-9BFF, 1=9C00-9FFF
	Pallet pallet;
	Tile map[32 * 32];
};

struct Window {
	uint8_t x, y;
	bool active,
		tileMapAddress; // 0=9800-9BFF, 1=9C00-9FFF
	Tile map[32 * 32];

};

class PPU {
private:
	//PPU
	PPUMode mode;
	bool enableLCD;
	bool doubleHeightSprites;
	Pallet spritePallets[2];

	//VRAM
	Window window;
	Background background;
	bool bgWindowTileData;//(0=8800-97FF, 1=8000-8FFF)
	Tile tiles[0x800];
	Tile* spriteTiles;
	Tile* bgTiles;

	//OAM
	bool showSprites;
	Sprite sprites[40];
public:
	PPU();
	~PPU();

	void WriteVRAM(uint16_t address, uint8_t data);
	uint8_t ReadVRAM(uint16_t address);

	void WriteOAM(uint16_t address, uint8_t data);
	uint8_t ReadOAM(uint16_t address);

	void Write(uint16_t address, uint8_t data);
	uint8_t Read(uint16_t address);
};

#endif // !_PPU_H_
