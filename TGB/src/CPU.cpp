#include "CPU.h"

#include <fstream>
#include <string>


CPU::CPU()
	:memory(new AddressBus(this))
{
}

CPU::~CPU()
{
	delete memory;
}

void CPU::update() {
	/*
	number of bytes
	uint8_t/char 1
	uint16_t/short 2
	*/
	printf("$%X: ",reg.pc);
	uint8_t opcode = (*memory)[reg.pc++];
	Instruction instruction = instructions[opcode];
	if (instruction.opcode == Instruction().opcode) {
		printf("0x%X (NOP)\n", opcode);
	}
	else {
		printf("0x$%X (%s)\n",opcode,instruction.opcode);
		instruction.call(this);
		reg.pc += instruction.paramCount;
	}
}

void CPU::PUSH(GBuint16& src)
{
	(*memory)[reg.sp - 1u] = src.h;
	(*memory)[reg.sp - 2u] = src.l;
	reg.sp -= 2;
}

void CPU::POP(GBuint16& src)
{
	src.l = (*memory)[reg.sp];
	src.h = (*memory)[reg.sp+1u];
	reg.sp += 2;
}

void CPU::STOP()
{
	//Halt CPU
}

void CPU::JP(uint16_t addr)
{
	reg.pc = (GBuint16)addr;
}

void CPU::JR(uint16_t addr)
{
	reg.pc += (GBuint16)addr-1;
}

void CPU::CALL(GBuint16 addr)
{
	(*memory)[reg.sp - 1] = reg.pc.h;
	(*memory)[reg.sp - 2] = reg.pc.l;
	reg.pc = addr;
	reg.sp -= 2;

}

void CPU::RST(uint8_t addr)
{
	(*memory)[reg.sp - 1] = reg.pc.h;
	(*memory)[reg.sp - 2] = reg.pc.l;
	reg.pc.h = 0;
	reg.pc.l = addr;
	reg.sp -= 2;

}


void CPU::NOP() {} // 0x0

void CPU::LD_BC_d16() { // 0x1
	GBuint16 data;
	data.h = (*memory)[reg.pc];
	data.l = (*memory)[reg.pc + 1];
	reg.bc = data;
}

void CPU::LD_$BC_A() { // 0x2
	(*memory)[reg.bc] = reg.a;
}

void CPU::INC_BC() { // 0x3
	reg.bc++;
}

void CPU::INC_B() { // 0x4
	reg.b++;
}

void CPU::DEC_B() { // 0x5
	reg.b--;
}
void CPU::LD_B_d8() { // 0x6
	reg.b = (*memory)[reg.pc];
}

void CPU::RLCA() { // 0x7
	//?
}

void CPU::LD_$a16_SP() { // 0x8
	uint8_t address = (*memory)[reg.pc];
	(*memory)[address] = reg.sp.h;
	(*memory)[address+1] = reg.sp.l;
}

void CPU::ADD_HL_BC() { // 0x9
	reg.hl += reg.bc;
}

void CPU::LD_A_$BC() { // 0xa
	reg.a = (*memory)[reg.bc];
}

void CPU::DEC_BC() { // 0xb
	reg.bc--;
}

void CPU::INC_C() { // 0xc
	reg.c++;
}

void CPU::DEC_C() { // 0xd
	reg.c--;
}

void CPU::LD_C_d8() { // 0xe
	reg.c = (*memory)[reg.pc];
}

void CPU::RRCA() { // 0xf
	//?
}

void CPU::STOP_0() { // 0x10
	STOP();
}

void CPU::LD_DE_d16() { // 0x11
	GBuint16 data;
	data.h = (*memory)[reg.pc];
	data.l = (*memory)[reg.pc + 1];
	reg.de = data;
}

void CPU::LD_$DE_A() { // 0x12
	reg.de = reg.a;
}

void CPU::INC_DE() { // 0x13
	reg.de++;
}

void CPU::INC_D() { // 0x14
	reg.d++;
}

void CPU::DEC_D() { // 0x15
	reg.d--;
}

void CPU::LD_D_d8() { // 0x16
	reg.d = (*memory)[reg.pc];
}

void CPU::RLA() { // 0x17
	//?
}

void CPU::JR_r8() { // 0x18
	JR((*memory)[reg.pc]);
}

void CPU::ADD_HL_DE() { // 0x19
	reg.hl += reg.de;
}

void CPU::LD_A_$DE() { // 0x1a
	reg.a = (*memory)[reg.de];
}

void CPU::DEC_DE() { // 0x1b
	reg.de--;
}

void CPU::INC_E() { // 0x1c
	reg.e++;
}

void CPU::DEC_E() { // 0x1d
	reg.e--;
}

void CPU::LD_E_d8() { // 0x1e
	reg.e = (*memory)[reg.pc];
}

void CPU::RRA() { // 0x1f
	//?
}

void CPU::JR_NZ_r8() { // 0x20
	bool NFlag = (reg.f & 0b01000000)>>6;
	if (!CheckFlag(Flag::FLAG_Z)) {
		JR((*memory)[reg.pc]);
	}

}

void CPU::LD_HL_d16() { // 0x21
	GBuint16 data;
	data.h = (*memory)[reg.pc];
	data.l = (*memory)[reg.pc + 1];
	reg.hl = data;
}

void CPU::LD_$HLI_A() { // 0x22
	(*memory)[reg.hl] = reg.a;
	reg.hl++;
}

void CPU::INC_HL() { // 0x23
	reg.hl++;
}

void CPU::INC_H() { // 0x24
	reg.h++;
}

void CPU::DEC_H() { // 0x25
	reg.h--;
}

void CPU::LD_H_d8() { // 0x26
	reg.h = (*memory)[reg.pc];
}

void CPU::DAA() { // 0x27
	//?
}

void CPU::JR_Z_r8() { // 0x28
	if (CheckFlag(Flag::FLAG_Z)) {
		JR((*memory)[reg.pc]);
	}
}

void CPU::ADD_HL_HL() { // 0x29
	reg.hl += reg.hl;
}

void CPU::LD_A_$HLI() { // 0x2a
	reg.a = (*memory)[reg.hl];
	reg.hl++;
}

void CPU::DEC_HL() { // 0x2b
	reg.hl--;
}

void CPU::INC_L() { // 0x2c
	reg.l++;
}

void CPU::DEC_L() { // 0x2d
	reg.l--;
}

void CPU::LD_L_d8() { // 0x2e
	reg.l = (*memory)[reg.pc];
}

void CPU::CPL() { // 0x2f
	//?
}

void CPU::JR_NC_r8() { // 0x30
	if (!CheckFlag(Flag::FLAG_C)) {
		JR((*memory)[reg.pc]);
	}
}

void CPU::LD_SP_d16() { // 0x31
	GBuint16 data;
	data.h = (*memory)[reg.pc];
	data.l = (*memory)[reg.pc + 1];
	reg.sp = data;
}

void CPU::LD_$HLD_A() { // 0x32
	(*memory)[reg.hl] = reg.a;
	reg.hl--;

}

void CPU::INC_SP() { // 0x33
	reg.sp++;
}

void CPU::INC_$HL() { // 0x34
	(*memory)[reg.hl]++;
}

void CPU::DEC_$HL() { // 0x35
	(*memory)[reg.hl]--;

}

void CPU::LD_$HL_d8() { // 0x36
	(*memory)[reg.hl] = (*memory)[reg.pc];

}

void CPU::SCF() { // 0x37
	///?
}

void CPU::JR_C_r8() { // 0x38
	if (CheckFlag(Flag::FLAG_C)) {
		JR((*memory)[reg.pc]);
	}
}

void CPU::ADD_HL_SP() { // 0x39
	reg.hl += reg.sp;
}

void CPU::LD_A_$HLD() { // 0x3a
	reg.a = (*memory)[reg.hl];
	reg.hl--;
}

void CPU::DEC_SP() { // 0x3b
	reg.sp--;
}

void CPU::INC_A() { // 0x3c
	reg.a++;
}

void CPU::DEC_A() { // 0x3d
	reg.a--;
}

void CPU::LD_A_d8() { // 0x3e
	reg.a = (*memory)[reg.pc];
}

void CPU::CCF() { // 0x3f
	//TODO
}

void CPU::LD_B_B() { // 0x40
}

void CPU::LD_B_C() { // 0x41
	reg.b = reg.c;
}

void CPU::LD_B_D() { // 0x42
	reg.b = reg.d;
}

void CPU::LD_B_E() { // 0x43
	reg.b = reg.e;
}

void CPU::LD_B_H() { // 0x44
	reg.b = reg.h;
}

void CPU::LD_B_L() { // 0x45
	reg.b = reg.l;
}

void CPU::LD_B_$HL() { // 0x46
	reg.b = (*memory)[reg.hl];
}

void CPU::LD_B_A() { // 0x47
	reg.b = reg.a;
}

void CPU::LD_C_B() { // 0x48
	reg.c = reg.b;
}

void CPU::LD_C_C() { // 0x49

}

void CPU::LD_C_D() { // 0x4a
	reg.c = reg.d;
}

void CPU::LD_C_E() { // 0x4b
	reg.c = reg.e;
}

void CPU::LD_C_H() { // 0x4c
	reg.c = reg.h;
}

void CPU::LD_C_L() { // 0x4d
	reg.c = reg.l;
}

void CPU::LD_C_$HL() { // 0x4e
	reg.c = (*memory)[reg.hl];
}

void CPU::LD_C_A() { // 0x4f
	reg.c = reg.a;
}

void CPU::LD_D_B() { // 0x50
	reg.d = reg.b;
}

void CPU::LD_D_C() { // 0x51
	reg.d = reg.c;
}

void CPU::LD_D_D() { // 0x52

}

void CPU::LD_D_E() { // 0x53
	reg.d = reg.e;
}

void CPU::LD_D_H() { // 0x54
	reg.d = reg.h;
}

void CPU::LD_D_L() { // 0x55
	reg.d = reg.l;
}

void CPU::LD_D_$HL() { // 0x56
	reg.d = (*memory)[reg.hl];
}

void CPU::LD_D_A() { // 0x57
	reg.d = reg.a;
}

void CPU::LD_E_B() { // 0x58
	reg.e = reg.b;
}

void CPU::LD_E_C() { // 0x59
	reg.e = reg.c;
}

void CPU::LD_E_D() { // 0x5a
	reg.e = reg.d;
}

void CPU::LD_E_E() { // 0x5b

}

void CPU::LD_E_H() { // 0x5c
	reg.e = reg.h;
}

void CPU::LD_E_L() { // 0x5d
	reg.e = reg.l;
}

void CPU::LD_E_$HL() { // 0x5e
	reg.e = (*memory)[reg.hl];
}

void CPU::LD_E_A() { // 0x5f
	reg.e = reg.a;
}

void CPU::LD_H_B() { // 0x60
	reg.h = reg.b;
}

void CPU::LD_H_C() { // 0x61
	reg.h = reg.c;
}

void CPU::LD_H_D() { // 0x62
	reg.h = reg.d;
}

void CPU::LD_H_E() { // 0x63
	reg.h = reg.e;
}

void CPU::LD_H_H() { // 0x64

}

void CPU::LD_H_L() { // 0x65
	reg.h = reg.l;
}

void CPU::LD_H_$HL() { // 0x66
	reg.h = (*memory)[reg.hl];
}

void CPU::LD_H_A() { // 0x67
}

void CPU::LD_L_B() { // 0x68
	reg.l = reg.b;
}

void CPU::LD_L_C() { // 0x69
	reg.l = reg.c;
}

void CPU::LD_L_D() { // 0x6a
	reg.l = reg.d;
}

void CPU::LD_L_E() { // 0x6b
	reg.l = reg.e;
}

void CPU::LD_L_H() { // 0x6c
	reg.l = reg.h;
}

void CPU::LD_L_L() { // 0x6d

}

void CPU::LD_L_$HL() { // 0x6e
	reg.l = (*memory)[reg.hl];
}

void CPU::LD_L_A() { // 0x6f
	reg.l = reg.a;
}

void CPU::LD_$HL_B() { // 0x70
	(*memory)[reg.hl] = reg.b;
}

void CPU::LD_$HL_C() { // 0x71
	(*memory)[reg.hl] = reg.c;

}

void CPU::LD_$HL_D() { // 0x72
	(*memory)[reg.hl] = reg.d;

}

void CPU::LD_$HL_E() { // 0x73
	(*memory)[reg.hl] = reg.e;

}

void CPU::LD_$HL_H() { // 0x74
	(*memory)[reg.hl] = reg.h;

}

void CPU::LD_$HL_L() { // 0x75
	(*memory)[reg.hl] = reg.b;

}

void CPU::HALT() { // 0x76

}

void CPU::LD_$HL_A() { // 0x77
	(*memory)[reg.hl] = reg.a;

}

void CPU::LD_A_B() { // 0x78
	reg.a = reg.b;

}

void CPU::LD_A_C() { // 0x79
	reg.a = reg.c;

}

void CPU::LD_A_D() { // 0x7a
	reg.a = reg.d;

}

void CPU::LD_A_E() { // 0x7b
	reg.a = reg.e;

}

void CPU::LD_A_H() { // 0x7c
	reg.a = reg.h;

}

void CPU::LD_A_L() { // 0x7d
	reg.a = reg.l;
}

void CPU::LD_A_$HL() { // 0x7e
	reg.a = reg.b;

}

void CPU::LD_A_A() { // 0x7f

}

void CPU::ADD_A_B() { // 0x80
	reg.a += reg.b;
}

void CPU::ADD_A_C() { // 0x81
	reg.a += reg.c;

}

void CPU::ADD_A_D() { // 0x82
	reg.a += reg.b;

}

void CPU::ADD_A_E() { // 0x83
	reg.a += reg.e;

}

void CPU::ADD_A_H() { // 0x84
	reg.a += reg.h;

}

void CPU::ADD_A_L() { // 0x85
	reg.a += reg.l;

}

void CPU::ADD_A_$HL() { // 0x86
	reg.a += (*memory)[reg.hl];
}

void CPU::ADD_A_A() { // 0x87

}

void CPU::ADC_A_B() { // 0x88

}

void CPU::ADC_A_C() { // 0x89

}

void CPU::ADC_A_D() { // 0x8a

}

void CPU::ADC_A_E() { // 0x8b

}

void CPU::ADC_A_H() { // 0x8c

}

void CPU::ADC_A_L() { // 0x8d

}

void CPU::ADC_A_$HL() { // 0x8e

}

void CPU::ADC_A_A() { // 0x8f

}

void CPU::SUB_B() { // 0x90
	reg.a -= reg.b;
}

void CPU::SUB_C() { // 0x91
	reg.a -= reg.c;
}

void CPU::SUB_D() { // 0x92
	reg.a += reg.d;

}

void CPU::SUB_E() { // 0x93
	reg.a -= reg.e;
}

void CPU::SUB_H() { // 0x94
	reg.a -= reg.h;

}

void CPU::SUB_L() { // 0x95
	reg.a -= reg.a;

}

void CPU::SUB_$HL() { // 0x96
	reg.a -= (*memory)[reg.hl];

}

void CPU::SUB_A() { // 0x97
	reg.a -= reg.a;

}

void CPU::SBC_A_B() { // 0x98

}

void CPU::SBC_A_C() { // 0x99

}

void CPU::SBC_A_D() { // 0x9a

}

void CPU::SBC_A_E() { // 0x9b

}

void CPU::SBC_A_H() { // 0x9c

}

void CPU::SBC_A_L() { // 0x9d

}

void CPU::SBC_A_$HL() { // 0x9e

}

void CPU::SBC_A_A() { // 0x9f

}

void CPU::AND_B() { // 0xa0
	reg.a &= reg.b;
}

void CPU::AND_C() { // 0xa1
	reg.a &= reg.c;

}

void CPU::AND_D() { // 0xa2
	reg.a &= reg.d;

}

void CPU::AND_E() { // 0xa3
	reg.a &= reg.e;

}

void CPU::AND_H() { // 0xa4
	reg.a &= reg.h;

}

void CPU::AND_L() { // 0xa5
	reg.a &= reg.l;

}

void CPU::AND_$HL() { // 0xa6
	reg.a &= (*memory)[reg.hl];

}

void CPU::AND_A() { // 0xa7
	reg.a &= (*memory)[reg.hl];

}

void CPU::XOR_B() { // 0xa8
	reg.a ^= (*memory)[reg.hl];

}

void CPU::XOR_C() { // 0xa9
	reg.a ^= reg.c;

}

void CPU::XOR_D() { // 0xaa
	reg.a ^= reg.d;

}

void CPU::XOR_E() { // 0xab
	reg.a ^= reg.e;

}

void CPU::XOR_H() { // 0xac
	reg.a ^= reg.h;

}

void CPU::XOR_L() { // 0xad
	reg.a ^= reg.l;

}

void CPU::XOR_$HL() { // 0xae
	reg.a ^= (*memory)[reg.hl];

}

void CPU::XOR_A() { // 0xaf
	reg.a ^= reg.a;

}

void CPU::OR_B() { // 0xb0
	reg.a |= reg.b;

}

void CPU::OR_C() { // 0xb1
	reg.a |= reg.c;

}

void CPU::OR_D() { // 0xb2
	reg.a |= reg.d;

}

void CPU::OR_E() { // 0xb3
	reg.a |= reg.e;

}

void CPU::OR_H() { // 0xb4
	reg.a |= reg.h;

}

void CPU::OR_L() { // 0xb5
	reg.a |= reg.l;

}

void CPU::OR_$HL() { // 0xb6
	reg.a |= (*memory)[reg.hl];
}

void CPU::OR_A() { // 0xb7
	reg.a |= reg.a;

}

void CPU::CP_B() { // 0xb8

}

void CPU::CP_C() { // 0xb9

}

void CPU::CP_D() { // 0xba

}

void CPU::CP_E() { // 0xbb

}

void CPU::CP_H() { // 0xbc

}

void CPU::CP_L() { // 0xbd

}

void CPU::CP_$HL() { // 0xbe

}

void CPU::CP_A() { // 0xbf

}

void CPU::RET_NZ() { // 0xc0

}

void CPU::POP_BC() { // 0xc1

}

void CPU::JP_NZ_a16() { // 0xc2

}

void CPU::JP_a16() { // 0xc3

}

void CPU::CALL_NZ_a16() { // 0xc4

}

void CPU::PUSH_BC() { // 0xc5

}

void CPU::ADD_A_d8() { // 0xc6

}

void CPU::RST_00H() { // 0xc7

}

void CPU::RET_Z() { // 0xc8

}

void CPU::RET() { // 0xc9
	reg.pc.l = (*memory)[reg.sp];
	reg.pc.h = (*memory)[reg.sp + 1];
	reg.sp += 2;
}

void CPU::JP_Z_a16() { // 0xca

}

void CPU::PREFIX_CB() { // 0xcb

}

void CPU::CALL_Z_a16() { // 0xcc

}

void CPU::CALL_a16() { // 0xcd

}

void CPU::ADC_A_d8() { // 0xce

}

void CPU::RST_08H() { // 0xcf

}

void CPU::RET_NC() { // 0xd0

}

void CPU::POP_DE() { // 0xd1

}

void CPU::JP_NC_a16() { // 0xd2

}

void CPU::CALL_NC_a16() { // 0xd4

}

void CPU::PUSH_DE() { // 0xd5

}

void CPU::SUB_d8() { // 0xd6

}

void CPU::RST_10H() { // 0xd7

}

void CPU::RET_C() { // 0xd8

}

void CPU::RETI() { // 0xd9

}

void CPU::JP_C_a16() { // 0xda

}

void CPU::CALL_C_a16() { // 0xdc

}

void CPU::SBC_A_d8() { // 0xde

}

void CPU::RST_18H() { // 0xdf

}

void CPU::LDH_$a8_A() { // 0xe0

}

void CPU::POP_HL() { // 0xe1

}

void CPU::LD_$C_A() { // 0xe2

}

void CPU::PUSH_HL() { // 0xe5

}

void CPU::AND_d8() { // 0xe6

}

void CPU::RST_20H() { // 0xe7

}

void CPU::ADD_SP_r8() { // 0xe8

}

void CPU::JP_$HL() { // 0xe9

}

void CPU::LD_$a16_A() { // 0xea

}

void CPU::XOR_d8() { // 0xee

}

void CPU::RST_28H() { // 0xef

}

void CPU::LDH_A_$a8() { // 0xf0

}

void CPU::POP_AF() { // 0xf1

}

void CPU::LD_A_$C() { // 0xf2

}

void CPU::DI() { // 0xf3

}

void CPU::PUSH_AF() { // 0xf5

}

void CPU::OR_d8() { // 0xf6

}

void CPU::RST_30H() { // 0xf7

}

void CPU::LD_HL_SP_r8() { // 0xf8

}

void CPU::LD_SP_HL() { // 0xf9

}

void CPU::LD_A_$a16() { // 0xfa

}

void CPU::EI() { // 0xfb

}

void CPU::CP_d8() { // 0xfe

}

void CPU::RST_38H() { // 0xff

}
