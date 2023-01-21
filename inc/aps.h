#ifndef APS_H
# define APS_H

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

extern int SMALL_SIZE;
extern int C;

// Merging functions
void serial_merge(int *a, long long int size, int *aux);
void serial_merge_generic(int *a, int *b, long long int a_start, long long int b_start,
                          int *aux, long long int aux_start, long long int size,
                          long long int a_size, long long int b_size,
                          long long int *a_inc, long long int *b_inc);
void parallel_mergepath(int *a, long long int size, int *aux, int threads);
void parallel_segmented_mergepath(int *a, long long int size, int *aux, int threads);

// Sorting functions
void serial_merge_sort(int *a, long long int size, int *aux);
void parallel_mergepath_sort(int *a, long long int size, int *aux, int threads);
void parallel_segmented_mergepath_sort(int *a, long long int size, int *aux, int threads);

#endif
