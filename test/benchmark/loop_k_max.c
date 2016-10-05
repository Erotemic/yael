#include <assert.h>
#include <stdio.h>
#include <yael/machinedeps.h>
#include <yael/vector.h>

/* 

gcc -fPIC -Wall -g -O3  -msse4 -c sorting.c -o sorting.o -Dstatic=

gcc -o loop_k_max -I ../../ -O3 loop_k_max.c -L../../yael -lyael
*/

void fvec_k_max_hoare (const float *val, int n, int *idx, int k);

void fvec_k_max_maxheap (const float *val, int n,
				int *idx, int k);




int main(int argc, char** argv) {
  assert(argc == 3); 
  int n = atoi(argv[1]); 
  int nrepeat = atoi(argv[2]);
  
  float * v = fvec_new_rand(n); 
  int * idx = ivec_new(n); 
  int * idx2 = ivec_new(n); 
  int k0, ki; 
  int m[3] = {1, 2, 5};
  for(k0 = 1; k0 < n; k0 *= 10) {
    for(ki = 0; ki < 3; ki++) {
      int k = k0 * m[ki]; 
      printf("k = %d ", k); 
      double st0 = 0, st1 = 0; 
      int r;
      for(r = 0; r < nrepeat; r++) {
      
	double t0 = getmillisecs(); 
	fvec_k_max_hoare(v, n, idx, k); 
	double t1 = getmillisecs(); 
	fvec_k_max_maxheap(v, n, idx2, k); 
	double t2 = getmillisecs(); 
	st0 += t1 - t0; 
	st1 += t2 - t1;
      }

      printf("qselect: %.4f ms, maxheap: %.4f ms\n", 
	     st0 / nrepeat, st1 / nrepeat);
      
    }



  }

 

  

  return 0; 

}
