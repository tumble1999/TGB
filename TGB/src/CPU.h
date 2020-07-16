#ifndef _CPU_H_
#define _CPU_H_

#include <cstdint>
#include "AddressBus.h"


enum class Flag : unsigned int {
	FLAG_C = 4u,	//Carry (cy)[NC] 
	FLAG_H,//HAlf Carry (h) (BCD)
	FLAG_N,	//Subtract (n) (BCD)
	FLAG_Z		//Zero (zf) [NZ]
};


struct Instruction
{
	const char* opcode = "NOP";
	const char* operands[2]{};
	int paramCount = 0;
	char flags[4]{ -1 };
	void (CPU::* execute)(void) = nullptr;

	void call(CPU* gb) {
		(gb->*execute)();
	}
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

	GBuint16 sp = 0xFFFE;
	GBuint16 pc = 0;
};


class CPU {
private:
	AddressBus* memory;
	Registers reg{};
	/*
	Flags
	-1 = -
	0 = 0
	1 = 1
	2 = [FLAG_Z,FLAG_N,FLAG_H,FLAG_C]
	*/
	Instruction instructions[0x100]{
{"NOP",{},0,{-1,-1,-1,-1},&CPU::NOP},
{"LD",{"BC","d16"},2,{-1,-1,-1,-1},&CPU::LD_BC_d16},
{"LD",{"$BC","A"},0,{-1,-1,-1,-1},&CPU::LD_$BC_A},
{"INC",{"BC"},0,{-1,-1,-1,-1},&CPU::INC_BC},
{"INC",{"B"},0,{2,0,2,-1},&CPU::INC_B},
{"DEC",{"B"},0,{2,1,2,-1},&CPU::DEC_B},
{"LD",{"B","d8"},1,{-1,-1,-1,-1},&CPU::LD_B_d8},
{"RLCA",{},0,{0,0,0,2},&CPU::RLCA},
{"LD",{"$a16","SP"},2,{-1,-1,-1,-1},&CPU::LD_$a16_SP},
{"ADD",{"HL","BC"},0,{-1,0,2,2},&CPU::ADD_HL_BC},
{"LD",{"A","$BC"},0,{-1,-1,-1,-1},&CPU::LD_A_$BC},
{"DEC",{"BC"},0,{-1,-1,-1,-1},&CPU::DEC_BC},
{"INC",{"C"},0,{2,0,2,-1},&CPU::INC_C},
{"DEC",{"C"},0,{2,1,2,-1},&CPU::DEC_C},
{"LD",{"C","d8"},1,{-1,-1,-1,-1},&CPU::LD_C_d8},
{"RRCA",{},0,{0,0,0,2},&CPU::RRCA},
{"STOP",{"0"},0,{-1,-1,-1,-1},&CPU::STOP_0},
{"LD",{"DE","d16"},2,{-1,-1,-1,-1},&CPU::LD_DE_d16},
{"LD",{"$DE","A"},0,{-1,-1,-1,-1},&CPU::LD_$DE_A},
{"INC",{"DE"},0,{-1,-1,-1,-1},&CPU::INC_DE},
{"INC",{"D"},0,{2,0,2,-1},&CPU::INC_D},
{"DEC",{"D"},0,{2,1,2,-1},&CPU::DEC_D},
{"LD",{"D","d8"},1,{-1,-1,-1,-1},&CPU::LD_D_d8},
{"RLA",{},0,{0,0,0,2},&CPU::RLA},
{"JR",{"r8"},1,{-1,-1,-1,-1},&CPU::JR_r8},
{"ADD",{"HL","DE"},0,{-1,0,2,2},&CPU::ADD_HL_DE},
{"LD",{"A","$DE"},0,{-1,-1,-1,-1},&CPU::LD_A_$DE},
{"DEC",{"DE"},0,{-1,-1,-1,-1},&CPU::DEC_DE},
{"INC",{"E"},0,{2,0,2,-1},&CPU::INC_E},
{"DEC",{"E"},0,{2,1,2,-1},&CPU::DEC_E},
{"LD",{"E","d8"},1,{-1,-1,-1,-1},&CPU::LD_E_d8},
{"RRA",{},0,{0,0,0,2},&CPU::RRA},
{"JR",{"NZ","r8"},1,{-1,-1,-1,-1},&CPU::JR_NZ_r8},
{"LD",{"HL","d16"},2,{-1,-1,-1,-1},&CPU::LD_HL_d16},
{"LD",{"$HLI","A"},0,{-1,-1,-1,-1},&CPU::LD_$HLI_A},
{"INC",{"HL"},0,{-1,-1,-1,-1},&CPU::INC_HL},
{"INC",{"H"},0,{2,0,2,-1},&CPU::INC_H},
{"DEC",{"H"},0,{2,1,2,-1},&CPU::DEC_H},
{"LD",{"H","d8"},1,{-1,-1,-1,-1},&CPU::LD_H_d8},
{"DAA",{},0,{2,-1,0,2},&CPU::DAA},
{"JR",{"Z","r8"},1,{-1,-1,-1,-1},&CPU::JR_Z_r8},
{"ADD",{"HL","HL"},0,{-1,0,2,2},&CPU::ADD_HL_HL},
{"LD",{"A","$HLI"},0,{-1,-1,-1,-1},&CPU::LD_A_$HLI},
{"DEC",{"HL"},0,{-1,-1,-1,-1},&CPU::DEC_HL},
{"INC",{"L"},0,{2,0,2,-1},&CPU::INC_L},
{"DEC",{"L"},0,{2,1,2,-1},&CPU::DEC_L},
{"LD",{"L","d8"},1,{-1,-1,-1,-1},&CPU::LD_L_d8},
{"CPL",{},0,{-1,1,1,-1},&CPU::CPL},
{"JR",{"NC","r8"},1,{-1,-1,-1,-1},&CPU::JR_NC_r8},
{"LD",{"SP","d16"},2,{-1,-1,-1,-1},&CPU::LD_SP_d16},
{"LD",{"$HLD","A"},0,{-1,-1,-1,-1},&CPU::LD_$HLD_A},
{"INC",{"SP"},0,{-1,-1,-1,-1},&CPU::INC_SP},
{"INC",{"$HL"},0,{2,0,2,-1},&CPU::INC_$HL},
{"DEC",{"$HL"},0,{2,1,2,-1},&CPU::DEC_$HL},
{"LD",{"$HL","d8"},1,{-1,-1,-1,-1},&CPU::LD_$HL_d8},
{"SCF",{},0,{-1,0,0,1},&CPU::SCF},
{"JR",{"C","r8"},1,{-1,-1,-1,-1},&CPU::JR_C_r8},
{"ADD",{"HL","SP"},0,{-1,0,2,2},&CPU::ADD_HL_SP},
{"LD",{"A","$HLD"},0,{-1,-1,-1,-1},&CPU::LD_A_$HLD},
{"DEC",{"SP"},0,{-1,-1,-1,-1},&CPU::DEC_SP},
{"INC",{"A"},0,{2,0,2,-1},&CPU::INC_A},
{"DEC",{"A"},0,{2,1,2,-1},&CPU::DEC_A},
{"LD",{"A","d8"},1,{-1,-1,-1,-1},&CPU::LD_A_d8},
{"CCF",{},0,{-1,0,0,2},&CPU::CCF},
{"LD",{"B","B"},0,{-1,-1,-1,-1},&CPU::LD_B_B},
{"LD",{"B","C"},0,{-1,-1,-1,-1},&CPU::LD_B_C},
{"LD",{"B","D"},0,{-1,-1,-1,-1},&CPU::LD_B_D},
{"LD",{"B","E"},0,{-1,-1,-1,-1},&CPU::LD_B_E},
{"LD",{"B","H"},0,{-1,-1,-1,-1},&CPU::LD_B_H},
{"LD",{"B","L"},0,{-1,-1,-1,-1},&CPU::LD_B_L},
{"LD",{"B","$HL"},0,{-1,-1,-1,-1},&CPU::LD_B_$HL},
{"LD",{"B","A"},0,{-1,-1,-1,-1},&CPU::LD_B_A},
{"LD",{"C","B"},0,{-1,-1,-1,-1},&CPU::LD_C_B},
{"LD",{"C","C"},0,{-1,-1,-1,-1},&CPU::LD_C_C},
{"LD",{"C","D"},0,{-1,-1,-1,-1},&CPU::LD_C_D},
{"LD",{"C","E"},0,{-1,-1,-1,-1},&CPU::LD_C_E},
{"LD",{"C","H"},0,{-1,-1,-1,-1},&CPU::LD_C_H},
{"LD",{"C","L"},0,{-1,-1,-1,-1},&CPU::LD_C_L},
{"LD",{"C","$HL"},0,{-1,-1,-1,-1},&CPU::LD_C_$HL},
{"LD",{"C","A"},0,{-1,-1,-1,-1},&CPU::LD_C_A},
{"LD",{"D","B"},0,{-1,-1,-1,-1},&CPU::LD_D_B},
{"LD",{"D","C"},0,{-1,-1,-1,-1},&CPU::LD_D_C},
{"LD",{"D","D"},0,{-1,-1,-1,-1},&CPU::LD_D_D},
{"LD",{"D","E"},0,{-1,-1,-1,-1},&CPU::LD_D_E},
{"LD",{"D","H"},0,{-1,-1,-1,-1},&CPU::LD_D_H},
{"LD",{"D","L"},0,{-1,-1,-1,-1},&CPU::LD_D_L},
{"LD",{"D","$HL"},0,{-1,-1,-1,-1},&CPU::LD_D_$HL},
{"LD",{"D","A"},0,{-1,-1,-1,-1},&CPU::LD_D_A},
{"LD",{"E","B"},0,{-1,-1,-1,-1},&CPU::LD_E_B},
{"LD",{"E","C"},0,{-1,-1,-1,-1},&CPU::LD_E_C},
{"LD",{"E","D"},0,{-1,-1,-1,-1},&CPU::LD_E_D},
{"LD",{"E","E"},0,{-1,-1,-1,-1},&CPU::LD_E_E},
{"LD",{"E","H"},0,{-1,-1,-1,-1},&CPU::LD_E_H},
{"LD",{"E","L"},0,{-1,-1,-1,-1},&CPU::LD_E_L},
{"LD",{"E","$HL"},0,{-1,-1,-1,-1},&CPU::LD_E_$HL},
{"LD",{"E","A"},0,{-1,-1,-1,-1},&CPU::LD_E_A},
{"LD",{"H","B"},0,{-1,-1,-1,-1},&CPU::LD_H_B},
{"LD",{"H","C"},0,{-1,-1,-1,-1},&CPU::LD_H_C},
{"LD",{"H","D"},0,{-1,-1,-1,-1},&CPU::LD_H_D},
{"LD",{"H","E"},0,{-1,-1,-1,-1},&CPU::LD_H_E},
{"LD",{"H","H"},0,{-1,-1,-1,-1},&CPU::LD_H_H},
{"LD",{"H","L"},0,{-1,-1,-1,-1},&CPU::LD_H_L},
{"LD",{"H","$HL"},0,{-1,-1,-1,-1},&CPU::LD_H_$HL},
{"LD",{"H","A"},0,{-1,-1,-1,-1},&CPU::LD_H_A},
{"LD",{"L","B"},0,{-1,-1,-1,-1},&CPU::LD_L_B},
{"LD",{"L","C"},0,{-1,-1,-1,-1},&CPU::LD_L_C},
{"LD",{"L","D"},0,{-1,-1,-1,-1},&CPU::LD_L_D},
{"LD",{"L","E"},0,{-1,-1,-1,-1},&CPU::LD_L_E},
{"LD",{"L","H"},0,{-1,-1,-1,-1},&CPU::LD_L_H},
{"LD",{"L","L"},0,{-1,-1,-1,-1},&CPU::LD_L_L},
{"LD",{"L","$HL"},0,{-1,-1,-1,-1},&CPU::LD_L_$HL},
{"LD",{"L","A"},0,{-1,-1,-1,-1},&CPU::LD_L_A},
{"LD",{"$HL","B"},0,{-1,-1,-1,-1},&CPU::LD_$HL_B},
{"LD",{"$HL","C"},0,{-1,-1,-1,-1},&CPU::LD_$HL_C},
{"LD",{"$HL","D"},0,{-1,-1,-1,-1},&CPU::LD_$HL_D},
{"LD",{"$HL","E"},0,{-1,-1,-1,-1},&CPU::LD_$HL_E},
{"LD",{"$HL","H"},0,{-1,-1,-1,-1},&CPU::LD_$HL_H},
{"LD",{"$HL","L"},0,{-1,-1,-1,-1},&CPU::LD_$HL_L},
{"HALT",{},0,{-1,-1,-1,-1},&CPU::HALT},
{"LD",{"$HL","A"},0,{-1,-1,-1,-1},&CPU::LD_$HL_A},
{"LD",{"A","B"},0,{-1,-1,-1,-1},&CPU::LD_A_B},
{"LD",{"A","C"},0,{-1,-1,-1,-1},&CPU::LD_A_C},
{"LD",{"A","D"},0,{-1,-1,-1,-1},&CPU::LD_A_D},
{"LD",{"A","E"},0,{-1,-1,-1,-1},&CPU::LD_A_E},
{"LD",{"A","H"},0,{-1,-1,-1,-1},&CPU::LD_A_H},
{"LD",{"A","L"},0,{-1,-1,-1,-1},&CPU::LD_A_L},
{"LD",{"A","$HL"},0,{-1,-1,-1,-1},&CPU::LD_A_$HL},
{"LD",{"A","A"},0,{-1,-1,-1,-1},&CPU::LD_A_A},
{"ADD",{"A","B"},0,{2,0,2,2},&CPU::ADD_A_B},
{"ADD",{"A","C"},0,{2,0,2,2},&CPU::ADD_A_C},
{"ADD",{"A","D"},0,{2,0,2,2},&CPU::ADD_A_D},
{"ADD",{"A","E"},0,{2,0,2,2},&CPU::ADD_A_E},
{"ADD",{"A","H"},0,{2,0,2,2},&CPU::ADD_A_H},
{"ADD",{"A","L"},0,{2,0,2,2},&CPU::ADD_A_L},
{"ADD",{"A","$HL"},0,{2,0,2,2},&CPU::ADD_A_$HL},
{"ADD",{"A","A"},0,{2,0,2,2},&CPU::ADD_A_A},
{"ADC",{"A","B"},0,{2,0,2,2},&CPU::ADC_A_B},
{"ADC",{"A","C"},0,{2,0,2,2},&CPU::ADC_A_C},
{"ADC",{"A","D"},0,{2,0,2,2},&CPU::ADC_A_D},
{"ADC",{"A","E"},0,{2,0,2,2},&CPU::ADC_A_E},
{"ADC",{"A","H"},0,{2,0,2,2},&CPU::ADC_A_H},
{"ADC",{"A","L"},0,{2,0,2,2},&CPU::ADC_A_L},
{"ADC",{"A","$HL"},0,{2,0,2,2},&CPU::ADC_A_$HL},
{"ADC",{"A","A"},0,{2,0,2,2},&CPU::ADC_A_A},
{"SUB",{"B"},0,{2,1,2,2},&CPU::SUB_B},
{"SUB",{"C"},0,{2,1,2,2},&CPU::SUB_C},
{"SUB",{"D"},0,{2,1,2,2},&CPU::SUB_D},
{"SUB",{"E"},0,{2,1,2,2},&CPU::SUB_E},
{"SUB",{"H"},0,{2,1,2,2},&CPU::SUB_H},
{"SUB",{"L"},0,{2,1,2,2},&CPU::SUB_L},
{"SUB",{"$HL"},0,{2,1,2,2},&CPU::SUB_$HL},
{"SUB",{"A"},0,{2,1,2,2},&CPU::SUB_A},
{"SBC",{"A","B"},0,{2,1,2,2},&CPU::SBC_A_B},
{"SBC",{"A","C"},0,{2,1,2,2},&CPU::SBC_A_C},
{"SBC",{"A","D"},0,{2,1,2,2},&CPU::SBC_A_D},
{"SBC",{"A","E"},0,{2,1,2,2},&CPU::SBC_A_E},
{"SBC",{"A","H"},0,{2,1,2,2},&CPU::SBC_A_H},
{"SBC",{"A","L"},0,{2,1,2,2},&CPU::SBC_A_L},
{"SBC",{"A","$HL"},0,{2,1,2,2},&CPU::SBC_A_$HL},
{"SBC",{"A","A"},0,{2,1,2,2},&CPU::SBC_A_A},
{"AND",{"B"},0,{2,0,1,0},&CPU::AND_B},
{"AND",{"C"},0,{2,0,1,0},&CPU::AND_C},
{"AND",{"D"},0,{2,0,1,0},&CPU::AND_D},
{"AND",{"E"},0,{2,0,1,0},&CPU::AND_E},
{"AND",{"H"},0,{2,0,1,0},&CPU::AND_H},
{"AND",{"L"},0,{2,0,1,0},&CPU::AND_L},
{"AND",{"$HL"},0,{2,0,1,0},&CPU::AND_$HL},
{"AND",{"A"},0,{2,0,1,0},&CPU::AND_A},
{"XOR",{"B"},0,{2,0,0,0},&CPU::XOR_B},
{"XOR",{"C"},0,{2,0,0,0},&CPU::XOR_C},
{"XOR",{"D"},0,{2,0,0,0},&CPU::XOR_D},
{"XOR",{"E"},0,{2,0,0,0},&CPU::XOR_E},
{"XOR",{"H"},0,{2,0,0,0},&CPU::XOR_H},
{"XOR",{"L"},0,{2,0,0,0},&CPU::XOR_L},
{"XOR",{"$HL"},0,{2,0,0,0},&CPU::XOR_$HL},
{"XOR",{"A"},0,{2,0,0,0},&CPU::XOR_A},
{"OR",{"B"},0,{2,0,0,0},&CPU::OR_B},
{"OR",{"C"},0,{2,0,0,0},&CPU::OR_C},
{"OR",{"D"},0,{2,0,0,0},&CPU::OR_D},
{"OR",{"E"},0,{2,0,0,0},&CPU::OR_E},
{"OR",{"H"},0,{2,0,0,0},&CPU::OR_H},
{"OR",{"L"},0,{2,0,0,0},&CPU::OR_L},
{"OR",{"$HL"},0,{2,0,0,0},&CPU::OR_$HL},
{"OR",{"A"},0,{2,0,0,0},&CPU::OR_A},
{"CP",{"B"},0,{2,1,2,2},&CPU::CP_B},
{"CP",{"C"},0,{2,1,2,2},&CPU::CP_C},
{"CP",{"D"},0,{2,1,2,2},&CPU::CP_D},
{"CP",{"E"},0,{2,1,2,2},&CPU::CP_E},
{"CP",{"H"},0,{2,1,2,2},&CPU::CP_H},
{"CP",{"L"},0,{2,1,2,2},&CPU::CP_L},
{"CP",{"$HL"},0,{2,1,2,2},&CPU::CP_$HL},
{"CP",{"A"},0,{2,1,2,2},&CPU::CP_A},
{"RET",{"NZ"},0,{-1,-1,-1,-1},&CPU::RET_NZ},
{"POP",{"BC"},0,{-1,-1,-1,-1},&CPU::POP_BC},
{"JP",{"NZ","a16"},2,{-1,-1,-1,-1},&CPU::JP_NZ_a16},
{"JP",{"a16"},2,{-1,-1,-1,-1},&CPU::JP_a16},
{"CALL",{"NZ","a16"},2,{-1,-1,-1,-1},&CPU::CALL_NZ_a16},
{"PUSH",{"BC"},0,{-1,-1,-1,-1},&CPU::PUSH_BC},
{"ADD",{"A","d8"},1,{2,0,2,2},&CPU::ADD_A_d8},
{"RST",{"00H"},0,{-1,-1,-1,-1},&CPU::RST_00H},
{"RET",{"Z"},0,{-1,-1,-1,-1},&CPU::RET_Z},
{"RET",{},0,{-1,-1,-1,-1},&CPU::RET},
{"JP",{"Z","a16"},2,{-1,-1,-1,-1},&CPU::JP_Z_a16},
{"PREFIX",{"CB"},0,{-1,-1,-1,-1},&CPU::PREFIX_CB},
{"CALL",{"Z","a16"},2,{-1,-1,-1,-1},&CPU::CALL_Z_a16},
{"CALL",{"a16"},2,{-1,-1,-1,-1},&CPU::CALL_a16},
{"ADC",{"A","d8"},1,{2,0,2,2},&CPU::ADC_A_d8},
{"RST",{"08H"},0,{-1,-1,-1,-1},&CPU::RST_08H},
{"RET",{"NC"},0,{-1,-1,-1,-1},&CPU::RET_NC},
{"POP",{"DE"},0,{-1,-1,-1,-1},&CPU::POP_DE},
{"JP",{"NC","a16"},2,{-1,-1,-1,-1},&CPU::JP_NC_a16},
{},
{"CALL",{"NC","a16"},2,{-1,-1,-1,-1},&CPU::CALL_NC_a16},
{"PUSH",{"DE"},0,{-1,-1,-1,-1},&CPU::PUSH_DE},
{"SUB",{"d8"},1,{2,1,2,2},&CPU::SUB_d8},
{"RST",{"10H"},0,{-1,-1,-1,-1},&CPU::RST_10H},
{"RET",{"C"},0,{-1,-1,-1,-1},&CPU::RET_C},
{"RETI",{},0,{-1,-1,-1,-1},&CPU::RETI},
{"JP",{"C","a16"},2,{-1,-1,-1,-1},&CPU::JP_C_a16},
{},
{"CALL",{"C","a16"},2,{-1,-1,-1,-1},&CPU::CALL_C_a16},
{},
{"SBC",{"A","d8"},1,{2,1,2,2},&CPU::SBC_A_d8},
{"RST",{"18H"},0,{-1,-1,-1,-1},&CPU::RST_18H},
{"LDH",{"$a8","A"},1,{-1,-1,-1,-1},&CPU::LDH_$a8_A},
{"POP",{"HL"},0,{-1,-1,-1,-1},&CPU::POP_HL},
{"LD",{"$C","A"},0,{-1,-1,-1,-1},&CPU::LD_$C_A},
{},
{},
{"PUSH",{"HL"},0,{-1,-1,-1,-1},&CPU::PUSH_HL},
{"AND",{"d8"},1,{2,0,1,0},&CPU::AND_d8},
{"RST",{"20H"},0,{-1,-1,-1,-1},&CPU::RST_20H},
{"ADD",{"SP","r8"},1,{0,0,2,2},&CPU::ADD_SP_r8},
{"JP",{"$HL"},0,{-1,-1,-1,-1},&CPU::JP_$HL},
{"LD",{"$a16","A"},2,{-1,-1,-1,-1},&CPU::LD_$a16_A},
{},
{},
{},
{"XOR",{"d8"},1,{2,0,0,0},&CPU::XOR_d8},
{"RST",{"28H"},0,{-1,-1,-1,-1},&CPU::RST_28H},
{"LDH",{"A","$a8"},1,{-1,-1,-1,-1},&CPU::LDH_A_$a8},
{"POP",{"AF"},0,{2,2,2,2},&CPU::POP_AF},
{"LD",{"A","$C"},0,{-1,-1,-1,-1},&CPU::LD_A_$C},
{"DI",{},0,{-1,-1,-1,-1},&CPU::DI},
{},
{"PUSH",{"AF"},0,{-1,-1,-1,-1},&CPU::PUSH_AF},
{"OR",{"d8"},1,{2,0,0,0},&CPU::OR_d8},
{"RST",{"30H"},0,{-1,-1,-1,-1},&CPU::RST_30H},
{"LD",{"HL","SP_r8"},1,{0,0,2,2},&CPU::LD_HL_SP_r8},
{"LD",{"SP","HL"},0,{-1,-1,-1,-1},&CPU::LD_SP_HL},
{"LD",{"A","$a16"},2,{-1,-1,-1,-1},&CPU::LD_A_$a16},
{"EI",{},0,{-1,-1,-1,-1},&CPU::EI},
{},
{},
{"CP",{"d8"},1,{2,1,2,2},&CPU::CP_d8},
{"RST",{"38H"},0,{-1,-1,-1,-1},&CPU::RST_38H}
	};

	bool CheckFlag(Flag flag) {
		uint8_t binCheck = pow(2u, (uint8_t)flag);
		return (reg.f & binCheck) >> (int)flag;
	}

	void LD(uint8_t& dest, uint8_t& src) { dest = src; };
	void LD(GBuint16& dest, GBuint16& src) { dest = src; };
	void PUSH(GBuint16& src);
	void POP(GBuint16& src);


	//NOP No Instruction
	void STOP();//halt until interrupt occurs (low power)

	void JP(uint16_t addr);
	void JR(uint16_t addr);
	void CALL(GBuint16 addr);
	void RST(uint8_t addr);

	void NOP(); // 0x0
	void LD_BC_d16(); // 0x1
	void LD_$BC_A(); // 0x2
	void INC_BC(); // 0x3
	void INC_B(); // 0x4
	void DEC_B(); // 0x5
	void LD_B_d8(); // 0x6
	void RLCA(); // 0x7
	void LD_$a16_SP(); // 0x8
	void ADD_HL_BC(); // 0x9
	void LD_A_$BC(); // 0xa
	void DEC_BC(); // 0xb
	void INC_C(); // 0xc
	void DEC_C(); // 0xd
	void LD_C_d8(); // 0xe
	void RRCA(); // 0xf
	void STOP_0(); // 0x10
	void LD_DE_d16(); // 0x11
	void LD_$DE_A(); // 0x12
	void INC_DE(); // 0x13
	void INC_D(); // 0x14
	void DEC_D(); // 0x15
	void LD_D_d8(); // 0x16
	void RLA(); // 0x17
	void JR_r8(); // 0x18
	void ADD_HL_DE(); // 0x19
	void LD_A_$DE(); // 0x1a
	void DEC_DE(); // 0x1b
	void INC_E(); // 0x1c
	void DEC_E(); // 0x1d
	void LD_E_d8(); // 0x1e
	void RRA(); // 0x1f
	void JR_NZ_r8(); // 0x20
	void LD_HL_d16(); // 0x21
	void LD_$HLI_A(); // 0x22
	void INC_HL(); // 0x23
	void INC_H(); // 0x24
	void DEC_H(); // 0x25
	void LD_H_d8(); // 0x26
	void DAA(); // 0x27
	void JR_Z_r8(); // 0x28
	void ADD_HL_HL(); // 0x29
	void LD_A_$HLI(); // 0x2a
	void DEC_HL(); // 0x2b
	void INC_L(); // 0x2c
	void DEC_L(); // 0x2d
	void LD_L_d8(); // 0x2e
	void CPL(); // 0x2f
	void JR_NC_r8(); // 0x30
	void LD_SP_d16(); // 0x31
	void LD_$HLD_A(); // 0x32
	void INC_SP(); // 0x33
	void INC_$HL(); // 0x34
	void DEC_$HL(); // 0x35
	void LD_$HL_d8(); // 0x36
	void SCF(); // 0x37
	void JR_C_r8(); // 0x38
	void ADD_HL_SP(); // 0x39
	void LD_A_$HLD(); // 0x3a
	void DEC_SP(); // 0x3b
	void INC_A(); // 0x3c
	void DEC_A(); // 0x3d
	void LD_A_d8(); // 0x3e
	void CCF(); // 0x3f
	void LD_B_B(); // 0x40
	void LD_B_C(); // 0x41
	void LD_B_D(); // 0x42
	void LD_B_E(); // 0x43
	void LD_B_H(); // 0x44
	void LD_B_L(); // 0x45
	void LD_B_$HL(); // 0x46
	void LD_B_A(); // 0x47
	void LD_C_B(); // 0x48
	void LD_C_C(); // 0x49
	void LD_C_D(); // 0x4a
	void LD_C_E(); // 0x4b
	void LD_C_H(); // 0x4c
	void LD_C_L(); // 0x4d
	void LD_C_$HL(); // 0x4e
	void LD_C_A(); // 0x4f
	void LD_D_B(); // 0x50
	void LD_D_C(); // 0x51
	void LD_D_D(); // 0x52
	void LD_D_E(); // 0x53
	void LD_D_H(); // 0x54
	void LD_D_L(); // 0x55
	void LD_D_$HL(); // 0x56
	void LD_D_A(); // 0x57
	void LD_E_B(); // 0x58
	void LD_E_C(); // 0x59
	void LD_E_D(); // 0x5a
	void LD_E_E(); // 0x5b
	void LD_E_H(); // 0x5c
	void LD_E_L(); // 0x5d
	void LD_E_$HL(); // 0x5e
	void LD_E_A(); // 0x5f
	void LD_H_B(); // 0x60
	void LD_H_C(); // 0x61
	void LD_H_D(); // 0x62
	void LD_H_E(); // 0x63
	void LD_H_H(); // 0x64
	void LD_H_L(); // 0x65
	void LD_H_$HL(); // 0x66
	void LD_H_A(); // 0x67
	void LD_L_B(); // 0x68
	void LD_L_C(); // 0x69
	void LD_L_D(); // 0x6a
	void LD_L_E(); // 0x6b
	void LD_L_H(); // 0x6c
	void LD_L_L(); // 0x6d
	void LD_L_$HL(); // 0x6e
	void LD_L_A(); // 0x6f
	void LD_$HL_B(); // 0x70
	void LD_$HL_C(); // 0x71
	void LD_$HL_D(); // 0x72
	void LD_$HL_E(); // 0x73
	void LD_$HL_H(); // 0x74
	void LD_$HL_L(); // 0x75
	void HALT(); // 0x76
	void LD_$HL_A(); // 0x77
	void LD_A_B(); // 0x78
	void LD_A_C(); // 0x79
	void LD_A_D(); // 0x7a
	void LD_A_E(); // 0x7b
	void LD_A_H(); // 0x7c
	void LD_A_L(); // 0x7d
	void LD_A_$HL(); // 0x7e
	void LD_A_A(); // 0x7f
	void ADD_A_B(); // 0x80
	void ADD_A_C(); // 0x81
	void ADD_A_D(); // 0x82
	void ADD_A_E(); // 0x83
	void ADD_A_H(); // 0x84
	void ADD_A_L(); // 0x85
	void ADD_A_$HL(); // 0x86
	void ADD_A_A(); // 0x87
	void ADC_A_B(); // 0x88
	void ADC_A_C(); // 0x89
	void ADC_A_D(); // 0x8a
	void ADC_A_E(); // 0x8b
	void ADC_A_H(); // 0x8c
	void ADC_A_L(); // 0x8d
	void ADC_A_$HL(); // 0x8e
	void ADC_A_A(); // 0x8f
	void SUB_B(); // 0x90
	void SUB_C(); // 0x91
	void SUB_D(); // 0x92
	void SUB_E(); // 0x93
	void SUB_H(); // 0x94
	void SUB_L(); // 0x95
	void SUB_$HL(); // 0x96
	void SUB_A(); // 0x97
	void SBC_A_B(); // 0x98
	void SBC_A_C(); // 0x99
	void SBC_A_D(); // 0x9a
	void SBC_A_E(); // 0x9b
	void SBC_A_H(); // 0x9c
	void SBC_A_L(); // 0x9d
	void SBC_A_$HL(); // 0x9e
	void SBC_A_A(); // 0x9f
	void AND_B(); // 0xa0
	void AND_C(); // 0xa1
	void AND_D(); // 0xa2
	void AND_E(); // 0xa3
	void AND_H(); // 0xa4
	void AND_L(); // 0xa5
	void AND_$HL(); // 0xa6
	void AND_A(); // 0xa7
	void XOR_B(); // 0xa8
	void XOR_C(); // 0xa9
	void XOR_D(); // 0xaa
	void XOR_E(); // 0xab
	void XOR_H(); // 0xac
	void XOR_L(); // 0xad
	void XOR_$HL(); // 0xae
	void XOR_A(); // 0xaf
	void OR_B(); // 0xb0
	void OR_C(); // 0xb1
	void OR_D(); // 0xb2
	void OR_E(); // 0xb3
	void OR_H(); // 0xb4
	void OR_L(); // 0xb5
	void OR_$HL(); // 0xb6
	void OR_A(); // 0xb7
	void CP_B(); // 0xb8
	void CP_C(); // 0xb9
	void CP_D(); // 0xba
	void CP_E(); // 0xbb
	void CP_H(); // 0xbc
	void CP_L(); // 0xbd
	void CP_$HL(); // 0xbe
	void CP_A(); // 0xbf
	void RET_NZ(); // 0xc0
	void POP_BC(); // 0xc1
	void JP_NZ_a16(); // 0xc2
	void JP_a16(); // 0xc3
	void CALL_NZ_a16(); // 0xc4
	void PUSH_BC(); // 0xc5
	void ADD_A_d8(); // 0xc6
	void RST_00H(); // 0xc7
	void RET_Z(); // 0xc8
	void RET(); // 0xc9
	void JP_Z_a16(); // 0xca
	void PREFIX_CB(); // 0xcb
	void CALL_Z_a16(); // 0xcc
	void CALL_a16(); // 0xcd
	void ADC_A_d8(); // 0xce
	void RST_08H(); // 0xcf
	void RET_NC(); // 0xd0
	void POP_DE(); // 0xd1
	void JP_NC_a16(); // 0xd2
	void CALL_NC_a16(); // 0xd4
	void PUSH_DE(); // 0xd5
	void SUB_d8(); // 0xd6
	void RST_10H(); // 0xd7
	void RET_C(); // 0xd8
	void RETI(); // 0xd9
	void JP_C_a16(); // 0xda
	void CALL_C_a16(); // 0xdc
	void SBC_A_d8(); // 0xde
	void RST_18H(); // 0xdf
	void LDH_$a8_A(); // 0xe0
	void POP_HL(); // 0xe1
	void LD_$C_A(); // 0xe2
	void PUSH_HL(); // 0xe5
	void AND_d8(); // 0xe6
	void RST_20H(); // 0xe7
	void ADD_SP_r8(); // 0xe8
	void JP_$HL(); // 0xe9
	void LD_$a16_A(); // 0xea
	void XOR_d8(); // 0xee
	void RST_28H(); // 0xef
	void LDH_A_$a8(); // 0xf0
	void POP_AF(); // 0xf1
	void LD_A_$C(); // 0xf2
	void DI(); // 0xf3
	void PUSH_AF(); // 0xf5
	void OR_d8(); // 0xf6
	void RST_30H(); // 0xf7
	void LD_HL_SP_r8(); // 0xf8
	void LD_SP_HL(); // 0xf9
	void LD_A_$a16(); // 0xfa
	void EI(); // 0xfb
	void CP_d8(); // 0xfe
	void RST_38H(); // 0xff
public:
	CPU();
	~CPU();

	void update();
	Registers getRegisters() { return reg; };
};

#endif // !_CPU_H_
