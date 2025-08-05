/*
 * AT7456E_registers.h
 *
 *  Created on: Jun 3, 2025
 *      Author: sokolov
 */

#ifndef INC_AT7456E_REGISTERS_H_
#define INC_AT7456E_REGISTERS_H_

//-----------------------------------------------------------------------------
#define WRITE_ADDRESS(ADDRESS) (0x00 + ADDRESS)
#define READ_ADDRESS(ADDRESS)  (0x80 + ADDRESS)
//-----------------------REGISTERS---------------------------------------------
#define VM0   0x00
#define VM1   0x01
#define HOS   0x02
#define VOS   0x03
#define DMM   0x04
#define DMAH  0x05
#define DMAL  0x06
#define DMDI  0x07
#define CMM   0x08
#define CMAH  0x09
#define CMAL  0x0A
#define CMDI  0x0B
#define OSDM  0x0C
#define RB0   0x10
#define RB1   0x11
#define RB2   0x12
#define RB3   0x13
#define RB4   0x14
#define RB5   0x15
#define RB6   0x16
#define RB7   0x17
#define RB8   0x18
#define RB9   0x19
#define RB10  0x1A
#define RB11  0x1B
#define RB12  0x1C
#define RB13  0x1D
#define RB14  0x1E
#define RB15  0x1F
#define OSDBL  0x6C
//-----------------------REGISTERS FOR READ------------------------------------
#define STAT  0x20
#define DMDO  0x30
#define CMDO  0x40


#endif /* INC_AT7456E_REGISTERS_H_ */
