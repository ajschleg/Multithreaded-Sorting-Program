// Compile:  "gcc -pthread -o part2 pthread_sort-part2.c"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 8
#define N 1024

// arrays shared among threads
int unsorted[N];
int sorted[N];

// THOUGHT: Make another type for sort_mate which doesn't need tid?
typedef struct {
  int m;  
  int n;
  int start_index;
} parameters;

void *node(void *param);
void *leaf(void *param);

void seedArray(int *arr, int size, int min, int max);
void bubbleSort(int *arr, int size);
void mergeSortedLists(int *arr1, int *arr2, int *mergedArr, int size);

// TODO: Pass in N and M values in argv (also check to make sure pwr of 2 and N/M valid)
int main(int argc, char *argv[]) {

  clock_t start;
  float execution_time;

  pthread_t main_thread_id;
  pthread_attr_t thread_attr;  // thread attributes

  parameters *main_thread_params = (parameters *) malloc(sizeof(parameters));
  main_thread_params->m = M;
  main_thread_params->n = N;
  main_thread_params->start_index = 0;

  // printf("Size of parameters ptr:  %lu\n", sizeof(main_thread_params));
  // printf("Size of parameters:  %lu\n", sizeof(*main_thread_params));

  seedArray(unsorted, N, 0, 50);


  start = clock();

  // get default attributes
  pthread_attr_init(&thread_attr);

  // create the main thread
  pthread_create(&main_thread_id, &thread_attr, node, main_thread_params);

  // wait for the thread to exit
  pthread_join(main_thread_id, NULL);

  execution_time = ((float)(clock() - start))/CLOCKS_PER_SEC;

  printf("Number of threads: %d\nLength of list: %d\n",M,N);
  printf("Execution Time (secs): %f\n", execution_time);


  free(main_thread_params);

  return 0;
}



// Merger Thread
void *node(void *param) {
  //printf("Node Thread entered\n");

  pthread_attr_t thread_attr;  // thread attributes
  pthread_attr_init(&thread_attr);  // using default attr for all

  int i = 0;

  parameters *sp = param;
  int m = sp->m;
  int n = sp->n;
  int start = sp->start_index;

  /* STEPS:
      1. Create threads until m==1
      2. Sort remaining length to which we're assigned
      3. Wait for threads to complete (in reverse order of creation)
      4. Merge each thread as it is ready
  */

  // Step 1. Will need to create new params to pass in (multiple structs so need params* array)

  // Trying static allocation to confirm this thing even works
  parameters *child_params[M-1];
  // and space for tids
  pthread_t child_thread_id[M-1];

  while(m > 1) {
    //printf("Making child params from: m = %d, n = %d, start_index = %d\n", m, n, start);
    child_params[i] = (parameters *) malloc(sizeof(parameters));
    child_params[i]->m = m/2;
    child_params[i]->n = n/2;
    child_params[i]->start_index = n/2 + start;  // (n/m)*(m/2) = n/2
    // create the thread
    if (m > 2) {
      //printf("Creating Node Thread: m = %d, n = %d, st_ind = %d\n", child_params[i]->m, child_params[i]->n, child_params[i]->start_index);
      pthread_create(&child_thread_id[i], &thread_attr, node, child_params[i]);
    }
    else {  // last thread created should be leaf
      //printf("Creating Leaf Thread: m = %d, n = %d, st_ind = %d\n", child_params[i]->m, child_params[i]->n, child_params[i]->start_index);
      pthread_create(&child_thread_id[i], &thread_attr, leaf, child_params[i]);
    }

    m/=2;
    n/=2;
    i++;
  }
  // int i will be equal to # of child threads created
  
  //printf("Node thread done spawning threads, continuing to sort: i = %d, m = %d, n = %d \n", i, m, n);
  // Step 2. Do our sorting
  bubbleSort(unsorted + start, n);

  // Forgot to change up arrays while merging
  i--;  // or i = 0;
  // Step 3 (and 4). Wait for threads to complete, merging as they are (complete)
  while (i >= 0) {
    pthread_join(child_thread_id[i], NULL);
    //printf("Node thread: a child finished, merging...\n");
    mergeSortedLists(unsorted + start, unsorted + start + n, sorted, n);
    i--;
  //  m*=2; // m not used anymore so doesn't really matter
    n*=2;
  }

  pthread_exit(0);
}

// Sorter Thread
void *leaf(void *param) {
  //printf("Leaf Thread entered\n");

  parameters *sp = param;

  bubbleSort(unsorted + sp->start_index, sp->n);

  //printf("Leaf Thread done sorting\n");

  pthread_exit(NULL);
}







void seedArray(int *arr, int size, int min, int max) {
  srand((unsigned)time(NULL));

  for (int i = 0; i < size; i++) {
    arr[i] = rand()% (max - min + 1) + min;
  }
}

void bubbleSort(int *arr, int size) {
  int i;
  int j;
  int swap;

  for (i = 0; i < size - 1; i++) {
    for (j = 0; j < size - i - 1; j++) {
      if (arr[j] > arr[j+1]) {
        swap = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = swap;
      }
    }
  }

} 

// size here is size of each array
void mergeSortedLists(int *arr1, int *arr2, int *mergedArr, int size) {

  int i = 0;
  int j = 0;
  int k = 0;

  // walk each array, copying smaller of two elements
  while (j < size && k < size) {
    if (arr1[j] <= arr2[k]) {
      mergedArr[i++] = arr1[j++];
    }
    else {
      mergedArr[i++] = arr2[k++];
    }
  }

  // if there are remaining elements in one array (not sure which), copy them over
  while(j < size) {
    mergedArr[i++] = arr1[j++];
  }
  while(k < size) {
    mergedArr[i++] = arr2[k++];
  }

  // copy back to source array
  for (i = 0; i < size*2; i++) {
    arr1[i] = mergedArr[i];
  }
}