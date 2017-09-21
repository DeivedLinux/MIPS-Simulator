/*
 * Coprocessador1.h
 *
 *  Created on: 10/05/2017
 *      Author: Deived  William Jal Windows
 */

#ifndef COPROCESSADOR1_H_
#define COPROCESSADOR1_H_


#define add_s(_ft,_fs,_fd) _fd = _fs + _ft
#define sub_s(_ft,_fs,_fd) _fd = _fs - _ft
#define mul_s(_ft,_fs,_fd) _fd = _fs * _ft
#define div_s(_ft,_fs,_fd) _fd = _fs / _ft

#define add_d(_ft,offset1,_fs,offset2,_fd,offset3) (*(((double*)(_fd + offset1)))) = (*((double*)(_fs + offset2)))+ (*((double*)(_fd + offset3)))
#define sub_d(_ft,offset1,_fs,offset2,_fd,offset3) (*(((double*)(_fd + offset1)))) = (*((double*)(_fs + offset2)))- (*((double*)(_fd + offset3)))
#define mul_d(_ft,offset1,_fs,offset2,_fd,offset3) (*(((double*)(_fd + offset1)))) = (*((double*)(_fs + offset2)))* (*((double*)(_fd + offset3)))
#define div_d(_ft,offset1,_fs,offset2,_fd,offset3) (*(((double*)(_fd + offset1)))) = (*((double*)(_fs + offset2))) / (*((double*)(_fd + offset3)))



typedef enum
{
	f0,
	f1,
	f2,
	f3,
	f4,
	f5,
	f6,
	f7,
	f8,
	f9,
	f10,
	f11,
	f12,
	f13,
	f14,
	f15,
	f16,
	f17,
	f18,
	f19,
	f20,
	f21,
	f22,
	f23,
	f24,
	f25,
	f26,
	f27,
	f28,
	f29,
	f30,
	f31
}__attribute__((packed))FPURegisters;

typedef struct
{
	float *_register;
	union
	{
		unsigned char conditionalFlags;
		struct
		{
			unsigned char flag0:1;
			unsigned char flag1:1;
			unsigned char flag2:1;
			unsigned char flag3:1;
			unsigned char flag4:1;
			unsigned char flag5:1;
			unsigned char flag6:1;
			unsigned char flag7:1;
		}acess;
	};
}__attribute__((packed))Coprocessor1;

typedef struct
{
	unsigned int vaddr;
	unsigned int status;
	unsigned int cause;
	unsigned int epc;
}__attribute__((aligned))Coprocessor0;




#endif /* COPROCESSADOR1_H_ */
