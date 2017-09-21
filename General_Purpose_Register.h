/*
 * General_Purpose_Register.h
 *
 *  Created on: 27 de jun de 2017
 *      Author: Deived William Jal Windows
 */

#ifndef GENERAL_PURPOSE_REGISTER_H_
#define GENERAL_PURPOSE_REGISTER_H_



typedef enum
{
  Zero = 0,
  At,
  V0,
  V1,
  A0,
  A1,
  A2,
  A3,
  T0,
  T1,
  T2,
  T3,
  T4,
  T5,
  T6,
  T7,
  S0,
  S1,
  S2,
  S3,
  S4,
  S5,
  S6,
  S7,
  T8,
  T9,
  K0,
  K1,
  Gp,
  Sp,
  Fp,
  Ra,
}Register;


typedef union
{
	int word;
	int*ptr;
	unsigned wordu;
	short v[2];
	struct
	{
		char LB;
		char HB;
		char UB;
		char MB;
	}acess;
}__attribute__((aligned(4)))GPRegisters;

typedef enum
{
	Low_Order = 0,
	High_Order
}__attribute__((packed))Word_Order;



#endif /* GENERAL_PURPOSE_REGISTER_H_ */
