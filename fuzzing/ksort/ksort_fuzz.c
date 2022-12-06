#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "ksort.h"

#define MAX_LEN 1024
void check_result(int *array, int N, char *sort_type);
KSORT_INIT_GENERIC(int)

void check_result(int *array, int N, char *sort_type){

	for (int i = 0; i < N-1; ++i) {
		if (array[i] > array[i+1]) {
			fprintf(stderr, "Bug in %s!\n", sort_type);
			exit(0);
		}

	}
}

int main(int argc, char *argv[])
{
	FILE *fd ;
	char temp[MAX_LEN], element[MAX_LEN];
	int i, N;
	int *array, *ksmall, *introsort, *combsort, *mergesort, *heapsort;
	
	/***** Generate Data *****/
	scanf("%s", temp);
	N = atoi(temp);
		
	if( N < 0 || N > MAX_LEN ) exit(1); //exit(0)
	
	ksmall = (int*)malloc(sizeof(int) * N);
	introsort = (int*)malloc(sizeof(int) * N);
	mergesort = (int*)malloc(sizeof(int) * N);
	combsort = (int*)malloc(sizeof(int) * N);
	heapsort = (int*)malloc(sizeof(int) * N);
	array = (int*)malloc(sizeof(int) * N);
	
	for (i = 0; i < N; ++i) {
			if(feof(stdin)) break;
			scanf("%s", element);
			int data = atoi(element);
			
			array[i] = data;
			ksmall[i] = data;
			introsort[i] = data;
			combsort[i] = data;
			mergesort[i] = data;
			heapsort[i] = data;
	}
	
	/***** sort *****/
	ks_ksmall(int, N, ksmall, sizeof(int) * N);
	ks_introsort(int, N, introsort);
	check_result(introsort, N, "introsort");

#ifndef _ALIGNED_ONLY
	{ // test unaligned ksmall
		unsigned char *a;
		int *b;
		a = malloc(N * sizeof(int) + 1);
		b = (int*)(a + 1);
		b = array;
		ks_introsort(int, N, b);
		free(a);
	}
#endif
	
	ks_combsort(int, N, combsort);
	check_result(combsort, N, "combsort");
	
	ks_mergesort(int, N, mergesort, 0);
	check_result(mergesort, N, "mergesort");

	ks_heapmake(int, N, heapsort);
	ks_heapsort(int, N, heapsort);
	check_result(heapsort, N, "heapsort");

	free(ksmall);
	free(introsort);
	free(combsort);
	free(mergesort);
	free(heapsort);
	free(array);
	return 0;
}
