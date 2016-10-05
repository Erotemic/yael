#ifndef SPECTRAL_CLUSTERING_H_INCLUDED
#define SPECTRAL_CLUSTERING_H_INCLUDED

/*! @addtogroup clustering
 *  @{  */


/* perform a spectral clustering of the dataset v, 
   as proposed in [Ng Jordan Weiss 01]               */
double spectral_clustering (int d, int n, int k, double sigma, int niter,
			    const float * v, int nt, int seed, int nredo,
			    int * assign, int * nassign);


/*! @} */
#endif
