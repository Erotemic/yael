#ifndef __hkm_h
#define __hkm_h


/*--------------------------------------------------------------
 * hierarchical clustering
 --------------------------------------------------------------*/

/*! the structure used for the quantization */
typedef struct hkm_s {
  int nlevel;            /* number of levels */
  int bf;                /* the branching factor */
  int k;                 /* the number of leaves (bf^nlevel) */
  int d;                 /* dimension of the input vectors */
  float ** centroids;    /* centroids for all levels */
} hkm_t;


/*! create/delete a hierarchical quantizer structure.
   nlevel is the number of levels in the tree and bf the branching factor */
hkm_t * hkm_learn (int n, int d, int nlevel, int bf, 
		   const float * v, int nb_iter_max, int nt, int verbose, 
		   int ** clust_assign_out);
		 
void hkm_delete (hkm_t * hkm);

/*! Quantization usign the hierarchical clustering */
void hkm_quantize (const hkm_t * hkm, int n, const float * v, int * idx);

/*! I/O function for hkm */
void hkm_write (const char * filename, const hkm_t * hkm);
hkm_t * hkm_read (const char * filename);

/*! retrieve the centroids from a particular level */
float * hkm_get_centroids (const hkm_t * hkm, int l, int no);

#endif
