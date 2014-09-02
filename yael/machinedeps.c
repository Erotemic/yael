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


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>

#include <sys/time.h>


#include "machinedeps.h"


static int count_cpu_from_env() {
  int ncpu;
   
  if(!getenv("YAEL_COUNT_CPU")) return 0; 
  
  if(sscanf(getenv("YAEL_COUNT_CPU"), "%d", &ncpu) != 1 || ncpu <= 0) {
    fprintf(stderr, "could not parse YAEL_CPU_COUNT environment variable, using default\n"); 
    return 0; 
  } 
  return ncpu; 
}

#ifdef __linux__

#define __USE_GNU
#include <sched.h>

int count_cpu (void)
{
  int ncpu = count_cpu_from_env(); 
  if(ncpu) return ncpu; 

  cpu_set_t set;
  sched_getaffinity (0, sizeof (cpu_set_t), &set);
  int i, count = 0;
  for (i = 0; i < CPU_SETSIZE; i++)
    if (CPU_ISSET (i, &set))
      count++;
  return count;
}


#elif defined(__APPLE__)

#include <sys/types.h>
#include <sys/sysctl.h>


int count_cpu (void) {
  int ncpu = count_cpu_from_env(); 
  if(ncpu) return ncpu; 

  int count=-1;
  size_t count_size=sizeof(count);
  sysctlbyname("hw.ncpu",&count,&count_size,NULL,0);
  return count;
}

#else

int count_cpu() {
  return 1;
}


#endif

#ifndef __APPLE__

double log2(double x) {
  return log(x)/M_LN2;
}


#endif

#ifndef __linux__
void *memalign (size_t ignored, size_t nbytes)
{
  return malloc (nbytes);
}
#endif





double getmillisecs() 
{
  struct timeval tv;
  gettimeofday (&tv,NULL);
  return tv.tv_sec*1e3 +tv.tv_usec*1e-3;
}


/***********************************************************************
 *           Implementation of the threading part
 *
 * generic thread stuff */

#ifdef _OPENMP 

#include <omp.h>


#define GET_THREAD_NUM omp_get_thread_num()

#else


#define GET_THREAD_NUM 0

/* #pragma's will be ignored */

#endif


void compute_tasks (int n, int nt,
                    void (*task_fun) (void *arg, int tid, int i),
                    void *task_arg)
{
  int i;

#pragma omp parallel for schedule(dynamic) num_threads(nt)
  for(i = 0; i < n; i++) 
    (*task_fun)(task_arg, GET_THREAD_NUM, i);

}
