#include "aps.h"

/*
** Name    : Guilherme Akira ALVES DOS SANTOS
** Class   : Advanced Parallel Systems
** Project : Merge Path - A Visually Intuitive Approach to Parallel Merging
** Link    : https://arxiv.org/pdf/1406.2628.pdf
*/

// TO EDIT:
// Array small enough to be sorted by some other algorithm (like quick sort)
int SMALL_SIZE = 12;
// Cache size (getconf -a | grep CACHE)
int C = 32768;
// Number of threads
int threads = 4;
// List of array sizes to test the different algorithms
int test_sizes[] = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};


// If you don't know EXACTLY what you're doing, do NOT edit the code below  :)

// Prototype of some auxiliary functions
void print_header(void);
void print_time(int size, struct timeval *s, struct timeval *p, struct timeval *seg);
void print_end(void);
void init_array(int **random, int **a, int **aux, int size);

int main(void)
{
  // Length of the global array of sizes, arrays to be created and timestamps
  int test_sizes_len = sizeof(test_sizes) / sizeof(test_sizes[0]);
  int *random_array, *a, *aux;
  struct timeval serial[2], parallel[2], segmented[2];

  // Seed the RNG, enable nested parallelization and print the header of output
  srand(time(NULL));
  //omp_set_nested(1);
  print_header();

  // For each size value, create a new random set of array, run the algorithms
  // and print the results
  for (int i = 0; i < test_sizes_len; i++)
  {
    init_array(&random_array, &a, &aux, test_sizes[i]);
    
    // Run serial merge sort
    memcpy(a, random_array, test_sizes[i] * sizeof(int));
    gettimeofday(&(serial[0]), NULL);
    serial_merge_sort(a, test_sizes[i], aux);
    gettimeofday(&(serial[1]), NULL);
    
    // Run parallel merge sort (merge path)
    memcpy(a, random_array, test_sizes[i] * sizeof(int));
    gettimeofday(&(parallel[0]), NULL);
    parallel_mergepath_sort(a, test_sizes[i], aux, threads);
    gettimeofday(&(parallel[1]), NULL);
    
    // Run parallel merge sort (merge path with segmented style)
    memcpy(a, random_array, test_sizes[i] * sizeof(int));
    gettimeofday(&(segmented[0]), NULL);
    parallel_segmented_mergepath_sort(a, test_sizes[i], aux, threads);
    gettimeofday(&(segmented[1]), NULL);

    // Print results
    print_time(test_sizes[i], serial, parallel, segmented);

    // Free memory
    free(random_array);
    free(a);
    free(aux);
  }

  // Print last line of output
  print_end();
}

void print_header(void)
{
  printf("+------------------------------------------------------------------------------------+\n");
  printf("|                                  MERGE ALGORITHMS                                  |\n");
  printf("+------------------------------------------------------------------------------------+\n");
  printf("|               Threads = %-4d             |      Cache size (bytes) = %-10d    |\n", threads, C);
  printf("+------------------------------------------------------------------------------------+\n");
  printf("|   %10s   |   %12s   |   %16s   |   %19s   |\n", "Array size", "Serial", "Parallel: simple", "Parallel: segmented");
  printf("+------------------------------------------------------------------------------------+\n");
}

void print_time(int size, struct timeval *s, struct timeval *p, struct timeval *seg)
{
  printf("|   %10d   |   %9ld us   |   %13ld us   |   %16ld us   |\n", size,
      (long)((s[1].tv_sec - s[0].tv_sec) * 1000000 + (s[1].tv_usec - s[0].tv_usec)),
      (long)((p[1].tv_sec - p[0].tv_sec) * 1000000 + (p[1].tv_usec - p[0].tv_usec)),
      (long)((seg[1].tv_sec - seg[0].tv_sec) * 1000000 + (seg[1].tv_usec - seg[0].tv_usec)));
}

void print_end(void)
{
  printf("+------------------------------------------------------------------------------------+\n");
}

void init_array(int **random, int **a, int **aux, int size)
{
  *random = malloc(size * sizeof(int));
  *a = malloc(size * sizeof(int));
  *aux = malloc(size * sizeof(int));

  for (int i = 0; i < size; i++)
  {
    (*random)[i] = rand() % size;
  }
}
