#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2
#define ARR_LEN 10

//int count = 10; /* this data is shared by the thread(s) */
int num_arr[ARR_LEN];
int sorted_arr[ARR_LEN];

void *runner(void *param); /* threads call this function */


int main(int argc, char *argv[])
{
	int j;

	/* an array of threads to be joined upon */
	pthread_t workers[NUM_THREADS]; /* the thread identifier */
	pthread_attr_t attr; /* set of thread attributes */

	if (argc != 2) 
	{
		fprintf(stderr,"usage: a.out <integer value>\n");
		return -1;
	}

	if (atoi(argv[1]) < 0) 
	{
		fprintf(stderr,"%d must be >= 0\n",atoi(argv[1]));
		return -1;
	}

	/*set array values to random numbers then print*/
	for (int i = 0; i < ARR_LEN; ++i)
	{
		num_arr[i] = (rand() % 100);
	}
	printf("Unsorted array: ");
	for (int i = 0; i < ARR_LEN; ++i)
	{
		printf("%d ",num_arr[i] );
	}
	printf("\n");

	/* get the default attributes */
	pthread_attr_init(&attr);

	for(j = 0; j < NUM_THREADS; j++)
	{
		/* create the threads */
		pthread_create(&workers[j], &attr, runner, argv[1]);
	}

	for(j = 0; j < NUM_THREADS; j++)
	{
		/* wait for the thread to exit */
		pthread_join(workers[j], NULL);
	}

	for (int i = 0; i < ARR_LEN; ++i)
	{
		printf("%d\n", num_arr[i]);
	}
}

/* The thread will begin control in this function.
	Thread sorts list using bubble sort. */
void *runner(void *param)
{
	for (int i = 0; i < ARR_LEN - 1; ++i)
	{
		for (int j = 0; j < ARR_LEN - i - 1; ++j)
		{
			if(num_arr[j] > num_arr[j+1])
			{
				 /*swap elements*/
				int temp = num_arr[j];
				num_arr[j] = num_arr[j+1];
				num_arr[j+1] = temp;
			}
		}
	}

	printf("Sorted array: ");
	for (int i = 0; i < ARR_LEN; ++i)
	{
		printf("%d ", num_arr[i]);
	}
	printf("\n");

	pthread_exit(0);
}