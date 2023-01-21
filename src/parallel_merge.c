#include "aps.h"

/*
** Name    : Guilherme Akira ALVES DOS SANTOS
** Class   : Advanced Parallel Systems
** Project : Merge Path - A Visually Intuitive Approach to Parallel Merging
** Link    : https://arxiv.org/pdf/1406.2628.pdf
*/

static void diagonal_intersection(int *a, int *b,
                                  long long int a_size, long long int b_size,
                                  int threads,
                                  long long int *a_start, long long int *b_start,
                                  long long int thread_id, long long int diag);


// Parallel merge sort (merge path)
void parallel_mergepath(int *a, long long int size, int *aux, int threads)
{
  long long int i;
  int p;

  i = 0;
  p = threads;
#pragma omp parallel for num_threads(threads) schedule(static, 1)
  for (i = 0; i < threads; ++i)
  {
    long long int length, a_start, b_start, a_inc, b_inc, diag, aux_start, thread_id;

    thread_id = i;
    length = (thread_id == threads - 1 ? size - ((p - 1) * (size / p)) : size / p);
    diag = thread_id * (size / p);
    diagonal_intersection(a, a + size / 2, size / 2, size - size / 2, threads,
                         &a_start, &b_start, thread_id, diag);
    aux_start = thread_id * (size / p);
    serial_merge_generic(a, a + size / 2, a_start, b_start, aux, aux_start,
                         length, size / 2, size - size / 2, &a_inc, &b_inc);
  }

#pragma omp parallel for num_threads(threads) schedule(dynamic, size / threads)
  for (long long int j = 0; j < size; ++j) {
    a[j] = aux[j];
  }
}


// Parallel merge sort (merge path with segmented style)
void parallel_segmented_mergepath(int *a, long long int size, int *aux, int threads)
{
  int max_iter, i, p;
  long long int l, start_point, a_offset, b_offset, a_inc, b_inc;
  
  l = C / 2;
  max_iter = 2;
  max_iter = (max_iter == 0 ? 1 : max_iter);
  start_point = 0;
  a_offset = 0;
  b_offset = size / 2;
  i = 0;
  p = threads;
  for (int k = 0; k < max_iter; ++k) {
    if (k == max_iter - 1) {
      l = size - k * l;
    }
#pragma omp parallel for num_threads(threads) schedule(static, 1)
    for (i = 0; i < threads; ++i) {
      long long int length, a_start, b_start, diag, aux_start;
      
      length = (i == threads - 1 ? (l - ((l / p) * (p - 1))) : l / p);
      diag = i * (l / p);
      diagonal_intersection(
          a + a_offset, a + b_offset,
          ((l > size / 2 - a_offset) ? (size / 2 - a_offset) : l),
          ((l > (size - b_offset)) ? ((size - b_offset)) : l), threads, &a_start,
          &b_start, i, diag);
      aux_start = start_point + (i) * (l / p);
      serial_merge_generic(a + a_offset, a + b_offset, a_start, b_start, aux,
                           aux_start, length, size / 2 - a_offset,
                           size - b_offset, (i == threads - 1) ? &a_inc : NULL,
                           (i == threads - 1) ? &b_inc : NULL);
    }
    a_offset += a_inc;
    b_offset += b_inc;
    start_point += l;
  }

#pragma omp parallel for num_threads(threads) schedule(dynamic, size / threads)
  for (long long int j = 0; j < size; ++j) {
    a[j] = aux[j];
  }
}



// Intermediate function (see the paper for more information about this part)
static void diagonal_intersection(int *a, int *b,
                                  long long int a_size, long long int b_size,
                                  int threads,
                                  long long int *a_start, long long int *b_start,
                                  long long int thread_id, long long int diag)
{
  (void)threads;
  if (!thread_id)
  {
    *a_start = 0;
    *b_start = 0;
    return;
  }
  int overhalf_a, overhalf_b;
  long long int overhalf_offset_a, overhalf_offset_b;
  long long int left, mid, right, a_i, b_i;

  overhalf_a = diag > a_size ? 1 : 0;
  overhalf_b = diag > b_size ? 1 : 0;
  overhalf_offset_a = overhalf_b == 0 ? 0 : diag - b_size;
  overhalf_offset_b = overhalf_a == 0 ? 0 : diag - a_size;
  if (overhalf_a && overhalf_b) {
    diag = b_size + a_size - diag;
  } else if (overhalf_a && !overhalf_b) {
    diag = a_size;
  } else if (overhalf_b && !overhalf_a) {
    diag = b_size;
  }
  left = 0;
  right = diag;
  a_i = -1;
  b_i = -1;
  while (left <= right) {
    mid = (left + right) / 2;
    a_i = diag - mid + overhalf_offset_a;
    b_i = mid + overhalf_offset_b;
    *a_start = a_i;
    *b_start = b_i;
    if ((b_i >= b_size && a[a_i] > b[b_i - 1]) ||
        (a_i >= a_size && a[a_i - 1] < b[b_i]))
      break;
    if (a[a_i] >= b[b_i - 1] && a[a_i - 1] < b[b_i])
      break;
    else if (a_i == a_size || a[a_i] >= b[b_i - 1])
      left = mid + 1;
    else
      right = mid - 1;
  }
  return;
}
