#include "ROM.h"

#include <fstream>

const auto BANK_SIZE = 0x4000;

ROM::ROM(const char* path)
{
	printf("Opening file %s\n", path);
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		printf("Error opening file %s\n", path);
		exit(1);
	}
	size = file.tellg();
	data = new uint8_t[size];

	file.seekg(0, std::ios::beg);
	file.read((char*)data, size);
	file.close();
	if(size>BANK_SIZE) SwitchBank(1);
}

ROM::~ROM()
{
	delete[] data;
}

void ROM::SwitchBank(int bank)
{
	if(1<bank&&bank>size) swapableData = data + bank*BANK_SIZE;
}

uint8_t& ROM::operator[](int i)
{
	if (i < BANK_SIZE) {
		return (data + i)[0];
	}
	return (swapableData + i)[0];
}
