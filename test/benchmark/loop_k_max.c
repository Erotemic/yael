/*
Copyright © INRIA 2010-2011. 
Authors: Matthijs Douze & Herve Jegou 
Contact: matthijs.douze@inria.fr  herve.jegou@inria.fr

This software is a computer program whose purpose is to provide 
efficient tools for basic yet computationally demanding tasks, 
such as find k-nearest neighbors using exhaustive search 
and kmeans clustering. 

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/


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
