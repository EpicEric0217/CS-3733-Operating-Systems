/*parallel_sort.c: compiled with gcc parallel_sort.c -pthread. ran with ./a.out*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void* sortsublists(void *args);
void* mergesublists(void *args);

struct mergeparam_s { //this struct is used to store struct variables for the third thread two merge the two sublists into the single sorted list.
	double *arrin; /*this represents the input  array*/
	double *arrout; /*this represents the output array*/
	int n;
	int midl;
};

struct sortparam_s { //this struct is used to store struct variables for the threads to sort the sublists
	double *array;
	int beggng;
	int procend;
};

int main(int argc, char *argv[]) {
	pthread_t thr1; //the first p thread
	
	pthread_t thr2; //the second pthread
	
	pthread_t thr3; // the third p thread
	
	struct sortparam_s p1, p2;
	
	struct mergeparam_s p3;
	
	int N, mid, i;
	
	double * array; /* array of type double */
	
	double * array2; /* Used for copy of 'array'*/
	
	double * mergedarr; /* For storing merged array */
	
	clock_t start, end;
	
	if (argc != 2) {
		printf("Usage:%s<N>\n", argv[0]);
			
		return -1;
	}
	
	array = (double*)malloc(N * sizeof(double));
	
	array2 = (double*)malloc(N * sizeof(double));
	
	mergedarr = (double*)malloc(N * sizeof(double));
	
	N = atoi(argv[1]);
	
	srand(time(NULL)); //seed the random number generator used by rand() function.
	
	/*printf("Array: ");*/
	for(i = 0; i < N; i++) {/* generate the random numbers from the rand() function. */
	array[i] = array2[i] = (double)rand() / RAND_MAX * 99.0 + 1.0; /* range 1.0 - 100.0 */
	/* printf("%f ", arr[i]); */
	}
	/* printf("\n"); */
	
	p1.beggng = 0;
	
	p1.procend = N / 2;
	
	p1.array = array;
	
	p2.beggng = N /2;
	
	p2.procend = N;
	
	p2.array = array;
	
	p3.midl = N / 2; 
	
	p3.n = N;
	
	p3.arrin = array;
	
	p3.arrout = mergedarr;
	
	start = clock(); /*initialize the start time; set to the number of clock ticks*/
	
	//Now I am going to start the threads to begin working on the sub arrays
	pthread_create(&thr1, NULL, sortsublists, &p1); //create the first thread for sorting the first sublist, p1.
	
	pthread_create(&thr2, NULL, sortsublists, &p2); //create the second thread for sorting the first sublist, p1.
	
	//Wait for the sorting to complete...
	pthread_join(thr1, NULL);
	
	pthread_join(thr2, NULL);
	
	//Now I start the thread used for merging the two sublists
	pthread_create(&thr3, NULL, mergesublists, &p3);
	
	//Now I wait for the merging of the threads to complete
	pthread_join(thr3, NULL);
	
	end = clock() - start; //Set the end time; equal to number of current clock ticks minus the number of previous ones
	
	printf("Sorting is done in %fms when two threads are utilized\n", end*1000.0f / CLOCKS_PER_SEC);
	
	p1.procend = N; //This is for running on the entire array.
	
	p1.array = array2; //The first array, 'array', is already partially sorted. Now we pass the original copy of the array.
	
	start = clock(); //reinitialize the start time, equal to # of clock ticks
	
	sortsublists(&p1);
	
	end = clock() - start; //end time = # of current clock ticks minus the starting time
	
	printf("Sorting is done in %fms when one thread is utilized\n", end * 1000.0f / CLOCKS_PER_SEC);
	
	/*Now I am going to free the memory of the sorted sublists*/
	free(array);
	
	free(array2);
	
	free(mergedarr);
	
	return 0;
}

void* mergesublists(void *args) {
	int i, j, k;
	
	double *arrin, *arrout;
	
	struct mergeparam_s* proc = (struct mergeparam_s*) args;
	
	/* printf("Merge %d : ", proc->n); */
	
	i = k = 0; //initializing k and i to zero.
	
	j = proc->midl;
	
	arrin = proc -> arrin;
	
	arrout = proc -> arrout;
	
	while(i < proc ->midl && j < proc -> n){
		if(arrin[i] < arrin[j]) 
			arrout[k++] = arrin[i++]; 
		else
			arrout[k++] = arrin[j++];
	}
	
	while(i < proc -> midl) 
		arrout[k++] = arrin[i++];
	while(j < proc ->n)
		arrout[k++] = arrin[j++];
	/* for(i = 0; i < proc->n; i++)
     printf("%f ", arrout[i]);
	printf("\n"); */
	return NULL;
}

void* sortsublists(void *args) {
	int i, j, minim;
	
	double temp;
	
	double *array;
	
	struct sortparam_s* proc = (struct sortparam_s*) args;
	
	array = proc -> array;
	
	// printf("sort %d to %d: ", proc->beggng, proc->procend); 
	
	for(i = proc -> beggng; i < proc -> procend; i++) {
		minim = i;
		
		for(j = i + 1; j < proc -> procend; j++) {
			if(array[j] < array[minim])
				minim = j;
		}
		temp = array[i]; //this array hold the value of array for ith iteration from beggining of process until end of process.
		
		array[i] = array[minim];
		
		array [minim] = temp;
	}
	/*for(i = proc->beggng; i < proc->procend; i++)
    printf("%f ", array[i]);
	printf("\nEnd of the sort\n"); */	
	
	return NULL;
}
