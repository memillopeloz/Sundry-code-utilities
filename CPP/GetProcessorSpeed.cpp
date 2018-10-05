#define ULONG64 unsigned long long

ULONG64 TimerStart;
ULONG64 TimerStop;

void StartTimer(void)
{
	TimerStart = __rdtsc();
}

void StopTimer(void)
{
	TimerStop = __rdtsc();
}

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

void main()
{
	ULONG64 timeConversion = GetProcessorSpeed();
	cout << "CPU speed (MHZ): " << timeConversion << "\n\n";
	StartTimer();
	exampleFunctionToProfile();
	StopTimer();

	double diff = TimerStop - TimerStart;
	
	cout << "exampleFunctionToProfile() took " << ((diff / (double)timeConversion)) * 1000 << " nanoSeconds\n\n";
}
