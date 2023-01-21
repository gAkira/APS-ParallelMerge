#include "aps.h"

/*
** Name    : Guilherme Akira ALVES DOS SANTOS
** Class   : Advanced Parallel Systems
** Project : Merge Path - A Visually Intuitive Approach to Parallel Merging
** Link    : https://arxiv.org/pdf/1406.2628.pdf
*/

// Serial merge (normal one)
void serial_merge(int *a, long long int size, int *aux)
{
  long long int i = 0;
  long long int k = size / 2;
  long long int m = 0;

  while (i < size / 2 && k < size)
  {
    if (a[i] < a[k])
    {
      aux[m] = a[i];
      i++;
    } else {
      aux[m] = a[k];
      k++;
    }
    m++;
  }
  while (i < size / 2) {
    aux[m] = a[i];
    i++;
    m++;
  }
  while (k < size) {
    aux[m] = a[k];
    k++;
    m++;
  }
  memcpy(a, aux, size * sizeof(int));
}


// Serial merge (generic, so it can be used in other functions such the parallel ones)
void serial_merge_generic(int *a, int *b, long long int a_start, long long int b_start,
                          int *aux, long long int aux_start, long long int size,
                          long long int a_size, long long int b_size,
                          long long int *a_inc, long long int *b_inc)
{
  long long int i = a_start;
  long long int k = b_start;
  long long int m = 0;

  while (m < size && i < a_size && k < b_size) {
    if (a[i] < b[k]) {
      aux[aux_start + m] = a[i];
      i++;
    } else {
      aux[aux_start + m] = b[k];
      k++;
    }
    m++;
  }
  while (m < size && i < a_size) {
    aux[aux_start + m] = a[i];
    i++;
    m++;
  }
  while (m < size && k < b_size) {
    aux[aux_start + m] = b[k];
    k++;
    m++;
  }
  if (a_inc)
    *a_inc = i;
  if (b_inc)
    *b_inc = k;
}
