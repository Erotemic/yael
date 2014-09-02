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


/* Generic inverted file    */

#ifndef __ivfc_h
#define __ivfc_h

#include <stdio.h>
#include "hamming.h"

#define KEYNULL (2147483647)
#define IVFCHECKSUM (0xf0f0f0fa)

#define DEFAULT_SEG_SIZE 128

/* A structure representing the inverted file (loaded in memory)) */
typedef struct ivf_s {
  int checksum;
  int k;                 /* number of distinct key values */
  int n;                 /* number of vectors stored */
  int elem_size;         /* size of the additional info, per element */
  int *nbelems;          /* the number of elements stored for a given vw */
  int *seg_size;         /* the amount (number of elements) of data allocated for a given vw */
  int **ids;             /* the ids of the vectors (for each key, all the ids) */
  unsigned char ** adat; /* additional info stored with vectors */
} ivf_t;


/* matching elements (those returned) */
typedef struct ivfmatch_s {
  int qid;               /* query id */
  int bid;               /* base id */
  float score;           /* matching score */
} ivfmatch_t;


/*-------------------- Ivf interface --------------------*/

/* Check if the inverted file looks Ok */
int ivf_checksum (const ivf_t * ivf);

/* create a void inverted file in memory. 
   l: number of distinct inverted lists
   elemsize: size of the elementary element (0 for regular BOF)
   segsize: default if =0, otherwise specified number of elements
*/
ivf_t *ivf_new (int k, int elemsize, int segsize);


/* suppress the inverted file */
void ivf_delete (ivf_t * ivf);

/* Add n entries in the structure */
void ivf_addn (ivf_t * ivf, const int * id, const int * keys, 
	       const unsigned char * val, int n);

void ivf_add (ivf_t * ivf, int id, int key, const unsigned char * val);

/* returns the size of the list associated with a given key entry */
int ivf_get_nb_elems (const ivf_t * ivf, int key);

/* returns the set of vector identifiers associated with a given key value */
int * ivf_get_ids (const ivf_t * ivf, int key);

/* returns the set of values with a given key value */
unsigned char * ivf_get_vals (const ivf_t * ivf, int key);

/* returns the set of values with a given key value and given ids */
unsigned char * ivf_find_vals (const ivf_t * ivf, int * keys, int * ids, int n);

/* display the contents of an inverted file  */
void ivf_display (const ivf_t * ivf);

/* Count the total number of elements (descriptors) in the inverted file */
int ivf_count_nbelems (const ivf_t * ivf);

/* compute the imbalance factor */
double ivf_imbalance_factor (const ivf_t * ivf);

/* query the inverted file */
ivfmatch_t * ivf_hequery (const ivf_t * ivf, const int * qids, const int * keys, 
                          const unsigned char * adat, int nq,
                          int * buffer_size, int ht);

/* Alternate implementations, which weights the scores */
ivfmatch_t * ivf_hequeryw (const ivf_t * ivf, const int * qids, const int * keys,
                           const unsigned char * adat, int nq, int ht, size_t * nm, 
                           const float * score_map_, const float * list_w_);


/* Inverted file I/O */
int ivf_save (const char * fname, const ivf_t * ivf);

ivf_t * ivf_load (const char * fname);

ivfmatch_t * ivfmatch_new (int n);

/* Cross-match all elements based on binary signature 
 ivf_he_collect_crossmatches2 is similar to ivf_he_collect_crossmatches, 
 yet it collects reverse matches and self-matches (see crossmatch_he2 in hamming.h)
*/
hammatch_t ** ivf_he_collect_crossmatches (const ivf_t * ivf, int ht, size_t * nmatches);
hammatch_t ** ivf_he_collect_crossmatches2 (const ivf_t * ivf, int ht, size_t * nmatches);

void ivf_he_count_crossmatches (const ivf_t * ivf, int ht, size_t * nmatches);
void ivf_he_count_crossmatches2 (const ivf_t * ivf, int ht, size_t * nmatches);

/* Alternate implementation: slower when combined with 
   ivf_he_count_crossmatches, but uses half the memory.
   ivf_he_crossmatches_prealloc2 collects reverse matches (i.e., twice) and self-matches
*/
void ivf_he_crossmatches_prealloc (const ivf_t * ivf, int ht, 
                                   int * idx, uint16 * hams, 
                                   size_t * cumnmatches);
void ivf_he_crossmatches_prealloc2 (const ivf_t * ivf, int ht, 
                                    int * idx, uint16 * hams, 
                                    size_t * cumnmatches);


#endif
