/* Display a set of vector stored in a vector float format */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>


#define FMT_TXT     0
#define FMT_FVECS   1

void display_help (const char * progname)
{
  fprintf (stderr, "%s reads a fvecfile on standard input\n", progname);
  exit (0);
}


int main (int argc, char **argv) 
{
  int i, j, d, ret;
  int n = INT_MAX;         /* maximum number of vectors to be read */ 
  int maxd = 1000000;      /* maximum number of dimension */	  
  int out_fmt = FMT_TXT;   /* output to text (default) */         

  FILE * fi = stdin;
  FILE * fo = stdout;

  /* read the arguments */
  for (i = 1 ; i < argc ; i++) {
    char *a = argv[i];

    if (!strcmp (a, "-n") && i + 1 < argc) {
      ret = sscanf (argv[++i], "%d", &n);
      assert (ret == 1);
    } 
    else if (!strcmp (a, "-maxd") && i + 1 < argc) {
      ret = sscanf (argv[++i], "%d", &maxd);
      assert (ret == 1);
    } 
    else if (!strcmp (a, "-fvecs") && i + 1 < argc) {
      fo = fopen (argv[++i], "w");
      out_fmt = FMT_FVECS;
      assert (fo);
    }
    else {
      fprintf (stderr, "could not parse argument %s\n", a);
      display_help (argv[0]);
    }
  }

  /* Read the values while there are some */
  unsigned char * v = malloc (sizeof (*v) * maxd);
  float * vf = malloc (sizeof (*vf) * maxd);

  i = 0;
  while (!feof (fi) && i < n) {
    ret = fread (&d, sizeof (d), 1, fi);

    if (ret == 0)
      break;

    assert (d < maxd); 
    ret = fread (v, sizeof (*v), d, fi);
    assert (ret == d);

    if (out_fmt == FMT_TXT) {
      fprintf (fo, "[");
      for (j = 0 ; j < d ; j++)
	fprintf (fo, "%u ", v[j]);
      fprintf (fo, "]\n");
    }
    else if (out_fmt == FMT_FVECS) {
      for (j = 0 ; j < d ; j++)
	vf[j] = (unsigned char) v[j];
      ret = fwrite (&d, sizeof (d), 1, fo);
      assert (ret == 1);
      ret = fwrite (vf, sizeof (*vf), d, fo);
      assert (ret == d);
    }
    i++;
  }

  free (v);
  fprintf (stderr, "found %d vectors\n", i);
  fclose (fo);
  return 0;
}


