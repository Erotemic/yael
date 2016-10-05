#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "vlad.h"
#include "nn.h"
#include "vector.h"
#include "sorting.h"

void vlad_compute(int k, int d, const float *centroids, 
                  int n, const float *v, float *desc) 
{
  
  int i,j;
  int *assign = ivec_new (n);
 
  nn (n, k, d, centroids, v, assign);

  fvec_0 (desc, k * d);
      
  for (i = 0 ; i < n ; i++) {
    for (j = 0 ; j < d ; j++) 
      desc[assign[i]*d+j] += v[i*d+j] - centroids[assign[i]*d+j];
  }      

  free(assign);
}


void vlad_compute_weighted(int k, int d, const float *centroids, 
                           int n, const float *v, const float *weights, 
                           float *desc) 
{
  
  int i,j;
  int *assign = ivec_new (n);
 
  nn (n, k, d, centroids, v, assign);

  fvec_0(desc,k*d);
      
  for(i = 0 ; i < n ; i++) {
    float w = weights[i];
    for (j = 0 ; j < d ; j++) 
      desc[assign[i]*d+j] += (v[i*d+j] - centroids[assign[i]*d+j]) * w;
  }      

  free(assign);
}


void vlad_compute_subsets(int k, int d, const float *centroids, 
                          int n, const float *v,
                          int n_subset,
                          const int *subset_indexes, 
                          const int *subset_ends,
                          float *desc) 
{
  int j;
  int *assign = ivec_new(n);
 
  nn (n, k, d, centroids, v, assign);

  fvec_0 (desc, k * d * n_subset);
      
  int ss, ss_begin = 0;
  for (ss = 0 ; ss < n_subset ; ss++) {
    float *descss = desc + ss * k * d;
    int ss_end = subset_ends[ss], ii;
    for (ii = ss_begin ; ii < ss_end ; ii++) {
      int i = subset_indexes[ii];
      for (j = 0 ; j < d ; j++) 
        descss[assign[i]*d+j] += v[i*d+j] - centroids[assign[i]*d+j];
    }
    ss_begin = ss_end;
  }

  free(assign);
}


void bof_compute_subsets(int k, int d, const float *centroids, 
                         int n, const float *v,
                         int n_subset,
                         const int *subset_indexes, 
                         const int *subset_ends,
                         float *desc) 
{
  int *assign=ivec_new(n);
 
  nn (n, k, d, centroids, v, assign);

  fvec_0 (desc, k * n_subset);
      
  int ss, ss_begin = 0;
  for (ss = 0 ; ss < n_subset ; ss++) {
    float *descss = desc + ss * k;
    int ss_end = subset_ends[ss], ii;
    for (ii = ss_begin ; ii < ss_end ; ii++) {
      int i = subset_indexes[ii];
      descss[assign[i]] ++;
    }
    ss_begin = ss_end;
  }

  free (assign);
}


void bof_compute (int k, int d, const float *centroids, 
		  int n, const float *v, int *desc)
{
  int i;
  int *assign = ivec_new(n);
  nn (n, k, d, centroids, v, assign);
  ivec_0(desc,k);

  for(i=0;i<n;i++)
    desc[assign[i]]++;

  free(assign);
}


void bof_compute_ma (int k, int d, const float *centroids, 
		     int n, const float *v, int *desc, 
		     int ma, float alpha, int nt)
{
  int i;
  int *assign = ivec_new(n*ma);
  knn_thread (n, k, d, ma, centroids, v, assign, nt);
  ivec_0(desc,k);

  for(i=0;i<n*ma;i++)
    desc[assign[i]]++;

  free(assign);
}

