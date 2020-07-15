#ifndef _ROM_H_
#define _ROM_H_

#include <cstdint>

class ROM
{
private:
	uint8_t* data;
	uint8_t* swapableData;
	int size;

public:
	ROM(const char* path);
	~ROM();

	void SwitchBank(int bank);
	uint8_t& operator[](int i);

};

#endif // !_ROM_H_
