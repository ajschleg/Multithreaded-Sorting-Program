#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	/* Whenever OpenMP encounters the directive #pragma omp parallel it creates 
	as many threads are there are processing cores in the system. */
	/* sequential code */
	int i= 0;
	int num = atoi(argv[1]);


	#pragma omp parallel num_threads(num)
	{
		int id = omp_get_thread_num();
		printf("I am a parallel region. id = %d\n", id);
		if(id == 2)
		{
			printf("I am 2\n");
		}
	}
	/* sequential code */
	return 0;
}