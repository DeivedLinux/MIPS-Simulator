/*
 * Instrucoes.h
 *
 *  Created on: 10/05/2017
 *      Author: Deived William Jal Windows
 */

#ifndef INSTRUCOES_H_
#define INSTRUCOES_H_

#define add(_rd,_rs, _rt)     _rd.word = _rs.word + _rt.word
#define addu(_rd,_rs,_rt)     _rd.wordu = (unsigned)(_rs.wordu + _rt.wordu)

#define sub(_rd,_rs,_rt)      _rd.word = _rs.word - _rt.word
#define subu(_rd,_rs,_rt)     _rd.wordu = (unsigned)_rs.wordu - _rt.wordu

#define mult(_rs,_rt)         _rs.word*_rt.word
#define multu(_rs,_rt)        _rs.wordu*_rt.wordu

#define mul(acc,_rd,_rs,_rt)     acc = _rd.word = _rs.word*_rt.word
#define madd(acc,_rs,_rt)        acc +=_rs.word*_rt.word
#define maddu(acc,_rs,_rt)       acc +=_rs.wordu*_rt.wordu

#define msub(acc,_rs,_rt)        acc -=_rs.word*_rt.word
#define msubu(acc,_rs,_rt)       acc -=_rs.wordu*_rt.wordu

#define _div(_rs, _rt)        _rs.wordu/_rt.wordu
#define divu(_rs,_rt)         _rs.wordu/_rt.wordu

#define slt(_rd,_rs,_rt)      if(_rs.word < _rt.word) _rd.word = 1; else _rd.word = 0;
#define sltu(_rd,_rs,_rt)     if(_rs.wordu < _rt.wordu) _rd.wordu = 1; else _rd.wordu = 0;

#define and(_rd,_rs,_rt)      _rd.word = _rs.word&_rt.word
#define or(_rd,_rs,_rt)       _rd.word = _rs.word|_rt.word
#define nor(_rd,_rs,_rt)      _rd.word = !(_rs.word|_rt.word)
#define xor(_rd,_rs,_rt)      _rd.word = _rs.word^_rt.word

#define addi(_rt,_rs,_const)  _rt.word = _rs.word + _const
#define addiu(_rt,_rs,_const) _rt.wordu = (unsigned)(_rs.wordu + _const)

#define slti(_rs,_rt,_const)       { if(_rs.word < _const) _rt.word = 1; else _rt.word = 0; }
#define sltiu(_rs,_rt,_const)      { if(_rs.wordu < _const) _rt.wordu = 1; else _rt.wordu = 0; }

#define andi(_rs,_rt,_const)  _rt.word = _rs.word & _const
#define ori(_rs,_rt,_const)   _rt.word = _rs.word | _const
#define xori(_rs,_rt,_const)  _rt.word = _rs.word ^ _const

#define nop()                 asm("nop")

#define PSEUDOINSTRUCTION 0 

typedef enum
{
   ADD = 0b100000,
   ADDU,
   SUB,
   SUBU,
   DIV = 0x1A,
   DIVU,
   MULT = 0x18,
   MULTU,
   SLT = 0b101010,
   SLTU,
   AND = 0b100100,
   OR,
   NOR = 0b100111,
   XOR,
   JR = 0b001000,
   JALR,
   NOP = 0b000000,
   MFHI = 0b010000,
   MFLO = 0b010010,
   SLL = 0b000000,
   SRL = 0b000010,
   SLLV = 0b000100,
   SRA = 0b000011,
   SRAV = 0b000111,
   SRLV = 0b000110,
   TEQ = 0x34,
   TGE = 0x30,
   TGEU,
   TLT,
   TLTU,
   MTHI = 0x11,
   MTLO = 0x13,
   MOVZ = 0xA,
   MOVN = 0xB,
   TRUNC = 0xD,
   SYSCALL = 0xC,
   BREAK = 0xD,

}RTypeFunctProcessor;

typedef enum
{
  MADD = 0,
  MADDU,
  MUL = 0x02,
  MSUB = 0x04,
  MSUBU,
  CLZ = 0x20,
  CLO
}RTypeFunctProcessor_Mode2;

typedef enum
{
  ADDI = 0b001000,
  ADDIU,
  SLTI,
  SLTIU,
  ANDI,
  ORI,
  XORI,
  LW = 0b100011,
  SW = 0b101011,
  LBU = 0b100100,
  LB = 0b100000,
  SB = 0b101000,
  SH,
  LUI = 0b001111,
  LH = 0x21,
  LWL,
  LHU = 0x25,
  LWR,
  BEQ = 0b000100,
  BNE,
  BLEZ,
  BGTZ,
  BLTZ = 0b000001,
  BGEZ = 0b000001,
  BLTZAL = 0b000001,
  BGEZAL = 0b000001,
  LWC1 = 0b110001,
  SWC1 = 0b111001,
  BCLF = 0b010001,
  LL = 0x30,
  SWL = 0x2A,
  SWR = 0x2E,
  SDC1 = 0x3D,
  SC = 0x38,
 

}ITypeOpcode;

typedef enum 
{
  TEQI = 0xC,
  TGEI = 0x08,
  TGEIU,
  TLTI,
  TLTIU
}ITypeTrapException;

typedef enum
{
	J = 0b000010,
	JAL
}__attribute__((packed))JTypeOpcode;


typedef enum
{
  ADDf = 0,
  SUBf,
  MULf,
  DIVf,
  SQRT,
  ABS,
  MOV,          /*MOVE float ou double*/
  NEGf,
  ROUND = 0xC,
  CEIL_W = 0xE,
  FLOOR_W,        
  CVT_S = 0x20, /*Converte inteiro para float ou double para float*/
  CVT_D,        /*Converte inteiro para double ou float para double*/
  CVT_W = 0x24 /*Converte float para inteiro ou double para inteiro*/ 
}__attribute__((packed))RtypeFunctCoprocessor1;

typedef enum
{
   MFC1 = 0b000000,
   MTC1 = 0b000100

}__attribute__((packed))RtypeFmtCoprocessor1;

typedef enum
{
   MFC0 = 0b000000,
   MTC0 = 0b000100

}__attribute__((packed))RtypeFmtCoprocessor0;

typedef enum
{
	SinglePrecision = 0x10,
	DoublePrecision
}__attribute__((packed))Coprocessor1Fmt;



typedef enum
{
	R_OpProcessor = 0b000000,
  R_OpProcessor_mode2 = 0x1C,
	R_OpCoprocessor1 = 0b010001,
	R_OpCoprocessor0 = 0x10,
  TrapException = 0x01
}__attribute__((packed))OpcodeType;

typedef union
{
	unsigned int wordu;
	int word;
	struct
	{
		unsigned int funct:6;
		unsigned int shamt:5;
		unsigned int rd:5;
		unsigned int rt:5;
		unsigned int rs:5;
		unsigned int op:6;
	}acess;
}__attribute__((aligned(4)))RTypeInstruction;


typedef union
{
	unsigned int wordu;
	unsigned int word;
	struct
	{
	  unsigned int addressOrConstant:16;
		unsigned rt:5;
		unsigned rs:5;
		unsigned op:6;
	}acess;
}__attribute__((aligned(4)))ITypeInstruction;



typedef union
{
	unsigned int word;
	struct
	{
		unsigned address:26;
		unsigned op:6;
	}acess;
}__attribute__((aligned(4)))JTypeInstruction;

typedef union
{
	unsigned int wordu;
  int word;
	struct
	{
      int reserved:26;
	    unsigned op:6;
	}acess;
}__attribute__((aligned(4))) GenericInstruction;

typedef union
{
	unsigned word;
	struct
	{
		unsigned funct:6;
		unsigned fd:5;
		unsigned fs:5;
		unsigned ft:5;
		unsigned fmt:5;
		unsigned op:6;
	}acess;
}__attribute__((aligned(4)))FRTypeInstruction;

typedef union
{
  unsigned word;
  struct
  {
    unsigned funct:6;
    unsigned zero:5;
    unsigned rd:5;
    unsigned cond:2;
    unsigned cc:3;
    unsigned rs:5;
    unsigned op:6;
  }acess;
}__attribute__((aligned(4)))MoveFormatCondicionalInstruction;


typedef union
{
	unsigned wordu;
	int word;

	struct
	{
		unsigned imm:16;
		unsigned ft:5;
		unsigned rs:5;
		unsigned op:6;
	}acess;
}__attribute__((aligned(4)))FITypeInstruction;


#endif /* INSTRUCOES_H_ */
