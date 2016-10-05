#include <stdio.h>

#include <yael/vector.h>
#include <yael/kmeans.h>
#include <yael/machinedeps.h>

int main (int argc, char ** argv)
{
  int k = 50;                           /* number of cluster to create */
  int d = 20;                           /* dimensionality of the vectors */
  int n = 1000;                         /* number of vectors */
  int nt = 2;                           /* number of threads to use */
  int niter = 0;                        /* number of iterations (0 for convergence)*/
  int redo = 1;                         /* number of redo */

  float * v = fvec_new_rand (d * n);    /* random set of vectors */

  /* variables are allocated externaly */
  float * centroids = fvec_new (d * k); /* output: centroids */
  float * dis = fvec_new (n);           /* point-to-cluster distance */
  int * assign = ivec_new (n);          /* quantization index of each point */
  int * nassign = ivec_new (k);         /* output: number of vectors assigned to each centroid */

  double t1 = getmillisecs();
  kmeans (d, n, k, niter, v, 1, 1, redo, centroids, dis, assign, nassign);
  double t2 = getmillisecs();

  printf ("kmeans performed in %.3fs\n", (t2 - t1)  / 1000);
  
  ivec_print (nassign, k);              /* print the assignment to clusters */

  return 0;
}
