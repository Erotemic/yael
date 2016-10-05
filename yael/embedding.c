#include <math.h>
#include <stdio.h>

#define CST_SQRT2   1.414213562373095;   /* sqrt (2) */
#define CST_SQRT3   1.732050807568877;   /* sqrt (3) */
#define CST_SQRT6   2.449489742783178;   /* sqrt (6) */ 


/* polynomial embedding of degree 2: cos(a,b) -> cos(a,b)^2 */
void emb_poly2 (const float * x, float * y, int d)
{
  int i, j, pos = 0;
  
  for (pos = 0 ; pos < d ; pos++)
    y[pos] = x[pos] * x[pos];
  
  for (i = 0 ; i < d ; i++)
    for (j = i+1 ; j < d ; j++) 
      y[pos++] = x[i] * x[j] * CST_SQRT2;
    
}


/* Same as previous function, but add instead of mapping */
void emb_poly2a (const float * x, float * y, int d)
{
  int i, j, pos = 0;
  
  for (pos = 0 ; pos < d ; pos++)
    y[pos] += x[pos] * x[pos];
  
  for (i = 0 ; i < d ; i++)
    for (j = i+1 ; j < d ; j++) 
      y[pos++] += x[i] * x[j] * CST_SQRT2;
    
}



/* polynomial embedding of degree 3: cos(a,b) -> cos(a,b)^3 */
void emb_poly3 (const float * x, float * y, int d)
{
  int i, j, k, pos = 0;
  
  for (pos = 0 ; pos < d ; pos++)
    y[pos] = powf (x[pos], 3);
  
  for (i = 0 ; i < d ; i++)
    for (j = 0 ; j < d ; j++) {
      if (i == j)
        continue;
      y[pos++] = x[i] * x[i] * x[j] * CST_SQRT3;
    }
  
  for (i = 0 ; i < d ; i++) {
    const float xi = x[i];
    for (j = i+1 ; j < d ; j++) {
      const float xj = x[j];
      for (k = j+1 ; k < d ; k++)
        y[pos++] = xi * xj * x[k] * CST_SQRT6;
    }
  }
}


/* Same as previous function, but add instead of mapping */
void emb_poly3a (const float * x, float * y, int d)
{
  int i, j, k, pos = 0;
  
  for (pos = 0 ; pos < d ; pos++)
    y[pos] += powf (x[pos], 3);
  
  for (i = 0 ; i < d ; i++)
    for (j = 0 ; j < d ; j++) {
      if (i == j)
        continue;
      y[pos++] += x[i] * x[i] * x[j] * CST_SQRT3;
    }
  
  for (i = 0 ; i < d ; i++) {
    const float xi = x[i];
    for (j = i+1 ; j < d ; j++) {
      const float xj = x[j];
      for (k = j+1 ; k < d ; k++)
        y[pos++] += xi * xj * x[k] * CST_SQRT6;
    }
  }
}


/* Fourier modulation with F frequencies */
void ang_modulate (const float * an, const float * x, const float theta, float * y, int d, int F)
{
  int i, j, pos = 0;
  
  /* First fill constant part */
  float c = an[0];
  for (i = 0 ; i < d ; i++)
    y[pos++] += c * x[i];
  
  /* Now consider other frequencies */
  
  for (j = 1 ; j <= F ; j++) { 
    const float c1 = an[j] * cosf (j*theta);
    for (i = 0 ; i < d ; i++) {
      y[pos++] += c1 * x[i];
    }
  }
  for (j = 1 ; j <= F ; j++) { 
    const float c2 = an[j] * sinf (j*theta);
    for (i = 0 ; i < d ; i++) {
      y[pos++] += c2 * x[i];
    }
  }
}


#undef CST_SQRT2
#undef CST_SQRT3
#undef CST_SQRT6
        
