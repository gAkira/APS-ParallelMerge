#include "aps.h"

/*
** Name    : Guilherme Akira ALVES DOS SANTOS
** Class   : Advanced Parallel Systems
** Project : Merge Path - A Visually Intuitive Approach to Parallel Merging
** Link    : https://arxiv.org/pdf/1406.2628.pdf
*/

// Compare function to be used by the quick sort (small arrays)
static int compare(const void *a, const void *b)
{
  return (*(int*)a - *(int*)b);
}


// Functions encapsulating the merge algorithm:

// Traditional merge sort
void serial_merge_sort(int *a, long long int size, int *aux)
{
  if (size <= SMALL_SIZE)
  {
    qsort(a, size, sizeof(int), compare);
    return ;
  }
  serial_merge_sort(a, size / 2, aux);
  serial_merge_sort(a + size / 2, size - size / 2, aux);
  
  serial_merge(a, size, aux);
}


// Parallel merge sort (merge path)
void parallel_mergepath_sort(int *a, long long int size, int *aux, int threads)
{
  if (threads == 1 || size <= SMALL_SIZE)
  {
    serial_merge_sort(a, size, aux);
    return ;
  }

#pragma omp parallel sections num_threads(threads)
  {
#pragma omp section
    { parallel_mergepath_sort(a, size / 2, aux, threads / 2); }
#pragma omp section
    {
      parallel_mergepath_sort(a + size / 2, size - size / 2, aux + size / 2,
                           threads - threads / 2);
    }
  }

  parallel_mergepath(a, size, aux, threads);
}


// Parallel merge sort (merge path with semengeted style)
void parallel_segmented_mergepath_sort(int *a, long long int size, int *aux, int threads)
{
  if (threads == 1 || size <= SMALL_SIZE)
  {
    serial_merge_sort(a, size, aux);
    return ;
  }

#pragma omp parallel sections num_threads(threads)
  {
#pragma omp section
    { parallel_mergepath_sort(a, size / 2, aux, threads / 2); }
#pragma omp section
    {
      parallel_mergepath_sort(a + size / 2, size - size / 2, aux + size / 2,
                           threads - threads / 2);
    }
  }

  parallel_segmented_mergepath(a, size, aux, threads);
}
