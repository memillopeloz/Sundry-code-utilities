#include "time.h"

ULONG64 GetProcessorSpeed()
{
	time_t basetime;

	basetime = time(NULL);    /* time returns seconds */

	while (time(NULL) == basetime);
	ULONG64 stclk = __rdtsc();    /* rdtsc is an assembly instruction */
	ULONG64 endclk;

	basetime = time(NULL);
	while (time(NULL) == basetime);
	endclk = __rdtsc();

	
	ULONG64 nclks = endclk - stclk;
	ULONG64 mult_factor1 = 182065;
	ULONG64 add_factor2 = 90000;
	ULONG64 div_factor3 = 180000;

	ULONG64 HZ = (((nclks*mult_factor1) + add_factor2) / div_factor3);
	ULONG64 MHZ = ((HZ + (ULONG64)5000000) / (ULONG64)1000000);

	return MHZ;
}
