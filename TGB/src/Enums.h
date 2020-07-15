#ifndef _ENUMS_H_
#define _ENUMS_H_

enum class Flag : unsigned int {
	FLAG_C = 4u,	//Carry (cy)[NC] 
	FLAG_H,//HAlf Carry (h) (BCD)
	FLAG_N,	//Subtract (n) (BCD)
	FLAG_Z		//Zero (zf) [NZ]
};

#endif // !_ENUMS_H_
