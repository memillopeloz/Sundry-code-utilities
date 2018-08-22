#include <iostream>
#include <cstdio>
#include <algorithm>
#define getcx getchar_unlocked
#define putcx putchar_unlocked
int inline fastInput() {
	register int N = 0, C;
	while ((C = getcx()) < '0');
	do {
		N = (N<<3) + (N<<1) + C - '0';
	}while ((C = getcx()) >= '0');
	return N;
}
void inline fastOutput(int* res) {
	register int N = * res , rev, count = 0;
	rev = N;
	if (N == 0) {
		putcx('0');
		putcx('\n');
		return ;
	}
	while ((rev % 10) == 0) {
		count++;
		rev /= 10;
	}
	rev = 0;
	while (N != 0) {
		rev = (rev<<3) + (rev<<1) + N % 10;
		N /= 10;
	}
	while (rev != 0) {
		putcx(rev%10 + '0');
		rev /= 10;
	}
	while (count--)
	putcx('0');
	putcx('\n');
	return ;
}
