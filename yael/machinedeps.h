#ifndef MACHINEDEPS_H_INCLUDED
#define MACHINEDEPS_H_INCLUDED

#ifdef __APPLE__
#define HAVE_QSORT_R
#endif

#ifdef __linux__
#define HAVE_TLS
#else
#define __thread 
#endif

/*---------------------------------------------------------------------------*/
/*! @addtogroup machinedep
 *  @{
 */


/*! Return the number of cores. */
int count_cpu(void);

#ifndef __APPLE__

double log2(double x);

#endif

#ifdef __linux__
#include <malloc.h>
#else
#include <stdlib.h>

/*! allocate memory such that the pointer is aligned*/
void *memalign (size_t ignored, size_t nbytes);

#endif






/*! return a timestamp, which is useful to measure elapsed time */
double getmillisecs();

/*! exectutes a set of tasks in parallel using a thread pool 
 *
 * @param n            number of tasks to execute
 * @param nthread      number of threads that will run the tasks
 * @param task_fun     this callback will be called with 
 *              -  arg = task_arg
 *              -  tid = identifier of the thread in 0..nthread-1
 *              -  i = call number in 0..n-1
*/
void compute_tasks (int n, int nthread,
                    void (*task_fun) (void *arg, int tid, int i),
                    void *task_arg);




/*! @} */
#endif
