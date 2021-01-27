
#include "math.h"

unsigned long sqrt2(unsigned long N)		//Fastest square root function I could get
{
	unsigned long x,j;
	x = 0;
	for(j = 0x8000; j!=0; j>>=1)				//Start with 0x800 instead of 0x8000 since we know the result will never be
	{										//greater than 4095, this makes the function faster.
		x = x + j;
		if( x*x > N) x = x - j;
	}
	return(x);
}
