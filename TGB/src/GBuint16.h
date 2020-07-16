#ifndef _GBUINT16_H_
#define _GBUINT16_H_
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


#endif // !_GBUINT16_H_