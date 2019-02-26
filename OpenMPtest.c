#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	/* Whenever OpenMP encounters the directive #pragma omp parallel it creates 
	as many threads are there are processing cores in the system. */
	/* sequential code */
	#pragma omp parallel
	{
		printf("I am a parallel region.");
	}
	/* sequential code */
	return 0;
}