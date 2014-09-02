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


/* This code was written by Herve Jegou. Contact: herve.jegou@inria.fr  */
/* Last change: June 1st, 2010                                          */
/* This software is governed by the CeCILL license under French law and */
/* abiding by the rules of distribution of free software.               */
/* See http://www.cecill.info/licences.en.html                          */

#include <assert.h>
#include <string.h>
#include "mex.h"
#include "../yael/hamming.h"
#include "../yael/machinedeps.h"

#ifdef _OPENMP
#include <omp.h>
#endif


void usage (const char * msg) 
{
  char msgtot[1024];
  const char * msgusg = 
    "There are two modes, depending on whether a threshold is given or not\n\n"
    "H = yael_hamming (X, Y);\n\n"
    "       X and Y are set of compact bit vectors (uint8), 1 per column\n"
    "       H is the set of all Hamming distances, in uint16 format\n\n"
    "[ids, hdis] = yael_hamming (X, Y, thres);\n"
    "       ids: matching elements, thres: hamming threshold\n";
  
  sprintf (msgtot, "%s\n\n%s\n", msg, msgusg);
  mexErrMsgTxt (msgtot);
}


void mexFunction (int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray*prhs[])

{
  int mode_thres = 0;
  if (nrhs != 2 && nrhs != 3) 
    mexErrMsgTxt ("This function requires either 2 input arguments.");
  
  if (nrhs == 3) mode_thres = 1; 
  
  int d = mxGetM (prhs[0]);   /* d is the number of codes, i.e., 8 times the number of bits */
  int na = mxGetN (prhs[0]);
  int nb = mxGetN (prhs[1]);

  if (mxGetM (prhs[1]) != d) 
      usage ("Dimension of binary vectors are not consistent");

  if (mxGetClassID(prhs[0]) != mxUINT8_CLASS)
    	usage ("first argument should be uint 8 type"); 

  if (mxGetClassID(prhs[1]) != mxUINT8_CLASS)
      usage ("second argument should be uint8 type"); 

  uint8 * a = (uint8*) mxGetPr (prhs[0]);
  uint8 * b = (uint8*) mxGetPr (prhs[1]);


  /* Just compute all Hamming distances */
  if (mode_thres == 0) {
    if (nlhs > 1)
      usage ("This syntax expects only exactly one output argument");

    /* ouptut: distances */
    plhs[0] = mxCreateNumericMatrix (na, nb, mxUINT16_CLASS, mxREAL);
    uint16 *dis = (uint16*) mxGetPr (plhs[0]);

    compute_hamming (dis, a, b, na, nb, d);
  }
  
  /* Return only the Hamming distances below a given threshold */
  else {
    if (nlhs != 2)
      usage ("This syntax expects only exactly two output arguments");
    int ht = (int) mxGetScalar (prhs[2]);
    size_t totmatches;
    int * keys;
    uint16 *dis;
    size_t i;
    
/* #ifndef _OPENMP */
    match_hamming_count (a, b, na, nb, ht, d, &totmatches);
    
    plhs[0] = mxCreateNumericMatrix (2, totmatches, mxINT32_CLASS, mxREAL);
    plhs[1] = mxCreateNumericMatrix (1, totmatches, mxUINT16_CLASS, mxREAL);
    
    keys = (int *) mxGetPr(plhs[0]);
    dis = (uint16*) mxGetPr (plhs[1]);
    
    size_t ret = match_hamming_thres_prealloc (a, b, na, nb, ht, d, keys, dis);
 
    assert (ret == totmatches);

    /* Fix Matlab identifiers */
    for (i = 0 ; i < 2 * totmatches ; i++)
      keys[i] = keys[i] + 1;
  }
            
}

