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

/*
To use Bits basically use the boolean operators |,& and ~. Examples of how to use bit multiplation is as shown.

if(Varible & (1 << Mask))
or
if(Varible & ENUM_MASK)

To set a bit you would do this.
Varible |= (1 << Mask);
or
Varible |= ENUM_MASK;

To clear all the bits you would do.
Varible = 0;

To clear just 1 bit out of any bit you would do.
Varible &= ~(1 << Mask);
or
Varible &= ~(ENUM_MASK);

To Toggle the bit on or off based on its last setting you would use.
Varible ^= (1 << Mask);
or
Varible ^= ENUM_MASK;
*/

typedef unsigned int lbool;
typedef unsigned short mbool;
typedef unsigned char sbool;
typedef enum boolean_flag_t boolean_flag_t;

enum boolean_flag_t
{
	FALSE = 0,
	TRUE = 1
};

/*
foo = 0;  clear all bits.
foo = ~0;  set all bits, as ~ is the NOT-operator.
(foo & mask)  selects the bits specified by mask, e.g. if mask is 00001110, it will select the second, third and fourth bit, since & is the AND-operator.
foo |= mask; sets the bits specified by mask, without affecting the other bits, since | is the OR-operator.
foo &= ~(mask);  clears the bits specified by mask, without affecting the other bits. NOT mask, means all the bits not specified by mask will be selected, and the result of that is being stored in foo.
foo ^= mask; toggles the bits specified by mask, as ^ is the XOR-operator.
*/

#endif