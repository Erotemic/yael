/* Display a set of vector stored in a vector float format */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>


void display_help (const char * progname)
{
  fprintf (stderr, "%s reads a fvecfile on standard input\n", progname);
  exit (0);
}


int main (int argc, char **argv) 
{
  int i, j, d, ret;
  int n = INT_MAX;          /* maximum number of vectors to be read */
  int maxd = 500000000;

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
    else {
      fprintf (stderr, "could not parse argument %s\n", a);
      display_help (argv[0]);
    }
  }

  /* Read the values while there are some */
  int * v = malloc (sizeof (*v) * maxd);
  i = 0;
  while (!feof (fi) && i < n) {
    ret = fread (&d, sizeof (d), 1, fi);

    if (ret == 0)
      break;

    assert (d < maxd); 
    ret = fread (v, sizeof (*v), d, fi);
    assert (ret == d);
    
    fprintf (fo, "[");
    for (j = 0 ; j < d ; j++)
      fprintf (fo, "%d ", v[j]);
    fprintf (fo, "]\n");
    i++;
  }

  free (v);
  fprintf (stderr, "found %d vectors\n", i);
  return 0;
}


