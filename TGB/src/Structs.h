#ifndef _STRUCTS_H_
#define _STRUCTS_H_
#include <string>

#define SHORT(A,B) (A << 8u) | B

struct GBuint16 {
	//GBuint16(uint16_t& v) :value(v) {};
	GBuint16(uint16_t v = 0) :value(v) {};
	operator uint16_t() const { return value; }
	GBuint16& operator++(int) {
		(value)++;
		return *this;
	};
	GBuint16& operator--(int) {
		(value)--;
		return *this;
	};
	GBuint16& operator+=(const GBuint16& a)
	{
		value += (a.value);
		return *this; // return the result by reference
	}
	GBuint16& operator-=(const GBuint16& a)
	{
		value -= (a.value);
		return *this; // return the result by reference
	}

	friend bool operator<(GBuint16& a, GBuint16& b) { return (a.value) < (b.value); }
	friend bool operator> (GBuint16& a, GBuint16& b) { return b < a; }
	friend bool operator<=(GBuint16& a, GBuint16& b) { return !(a > b); }
	friend bool operator>=(const GBuint16& a, const GBuint16& b) { return !(a < b); }
	friend bool operator==(const GBuint16& a, const GBuint16& b) { return (a.value) == (b.value); }
	friend bool operator!=(const GBuint16& a, const GBuint16& b) { return !(a == b); }
	union
	{
		struct {
			uint8_t l;
			uint8_t h;
		};
		uint16_t value;
	};
};

struct Registers {
	struct {
		union {
			struct {
				uint8_t f;
				uint8_t a;
			};
			uint16_t af;
		};
	};

	struct {
		union {
			struct {
				uint8_t c;
				uint8_t b;
			};
			uint16_t bc;
		};
	};

	struct {
		union {
			struct {
				uint8_t e;
				uint8_t d;
			};
			uint16_t de;
		};
	};

	struct {
		union {
			struct {
				uint8_t l;
				uint8_t h;
			};
			uint16_t hl;
		};
	};

	GBuint16 sp=0xFFFE;
	GBuint16 pc=0;
};

class CPU;

struct Instruction
{
	const char* opcode = "NOP";
	const char* operands[2]{};
	int paramCount=0;
	char flags[4]{-1};
	void (CPU::*execute)(void)=nullptr;

	void call(CPU* gb) {
		(gb->*execute)();
	}
};

#endif // !_STRUCTS_H_