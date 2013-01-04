/*******************************************************************************
* Credits:  Anthony Ianozi
*			Andrew Wheeler/Genusis
*           Stephan
******************************************************************************/
#ifndef DE_BOOL_H
#define DE_BOOL_H

/*********************************************************************************************************
this is for True/False as well as bitsetting sigilar varible's.
If you need more than 32bits for a x86 OS or 64bits for a x64 OS then you should create an Bit set array .
If you do create a bitset array use only unsigned int don;t use anything else as that is the best/Fastest
for both systems.
*********************************************************************************************************/
//for people who are not use to bit methods.

//used for definitions or enum with (1<<?) already setup.
#define bitget(data, flag) (data & (flag))
#define bitset(data, flag) (data |= flag)
#define bitclear(data, flag) (data &= ~(flag))
#define bittoggle(data, flag) (data ^= flag)//on/off switch

//for use with numbers 0 - max
#define getbit(data, index) (data & (1 << index))
#define setbit(data, index) (data |= 1 << index)
#define clearbit(data, index) (data &= ~(1 << index))
#define togglebit(data, index) (data ^= 1 << index)//on/off switch

typedef unsigned int DE_Large_Boolean;
typedef unsigned short DE_Medium_Boolean;
typedef unsigned char DE_Small_Boolean;

#define sbool DE_Small_Boolean //used for 1- 8 booleans
#define mbool DE_Medium_Boolean //used for 9 - 16 booleans
#define lbool  DE_Large_Boolean //used for 17 - 32/64 booleans

#define FALSE 0
#define TRUE  1

/*
foo = 0;  clear all bits.
foo = ~0;  set all bits, as ~ is the NOT-operator.
(foo & mask)  selects the bits specified by mask, e.g. if mask is 00001110, it will select the second, third and fourth bit, since & is the AND-operator.
foo |= mask; sets the bits specified by mask, without affecting the other bits, since | is the OR-operator.
foo &= ~(mask);  clears the bits specified by mask, without affecting the other bits. NOT mask, means all the bits not specified by mask will be selected, and the result of that is being stored in foo.
foo ^= mask; toggles the bits specified by mask, as ^ is the XOR-operator.
*/

#endif