#ifndef __embedding_h
#define __embedding_h


/* Dimensionality after polynomial embedding of a d-dimensional vector */
#define EMB_POLY2_D(d)  ((d)*((d)+1)/2)
#define EMB_POLY3_D(d)  (((d)*(d)*(d)+3*(d)*(d)+2*(d))/6)


/* polynomial embedding of degree 2: cos(a,b) -> cos(a,b)^2 
   d is dimensionality of x. 
   y should be pre-allocated with dimensionality d*(d+1)/2     */
void emb_poly2 (const float * x, float * y, int d);

/* Same as emb_poly2a, but add to existing vector y */
void emb_poly2a (const float * x, float * y, int d);

/* polynomial embedding of degree 3: cos(a,b) -> cos(a,b)^3 */
void emb_poly3 (const float * x, float * y, int d);

/* Same as emb_poly3a,, but add instead of mapping */
void emb_poly3a (const float * x, float * y, int d);

/* Fourier modulation with F frequencies */
void ang_modulate (const float * an, const float * x, const float theta, float * y, int d, int F);


#endif