#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 2

int sum; /* this data is shared by the thread(s) */
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

	printf("sum = %d\n", sum);
}

/* The thread will begin control in this function */
void *runner(void *param)
{
	sum++;

	pthread_exit(0);
}