#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include <yael/binheap.h>


int main (int argc, char ** argv)
{
  int n = 10000, k = 10;
  int i;
  float * v = malloc (n * sizeof (v));
  int * bestidx = malloc (k * sizeof (*bestidx));
  float * bestval = malloc (k * sizeof (*bestval));

  for (i = 0 ; i < n ; i++) {
    v[i] = rand() / (double) INT_MAX;
  }

  fbinheap_t * bh = fbinheap_new (k);
    
  for (i = 0 ; i < n ; i++) 
    fbinheap_add (bh, i, v[i]);

  fbinheap_sort (bh, bestidx, bestval);
  for (i = 0 ; i < k ; i++)
    printf ("%d %.5g / ", bestidx[i], bestval[i]);
  printf ("\n");

  return 0;
}
