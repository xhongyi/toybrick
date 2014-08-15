%module fast_brute
%include "cdata.i"

%{
  /* Put headers and other declarations here */

  extern int* validLocs;

  extern char* seeds2string (int**, int);
  extern void copy_seeds_list(int**, int**, int);

  extern int num_loc_generated(int**, int, int**, int);
  extern int is_valid(int, int**, int);
  extern char* find_optimal_seeds(PyObject*, int, int, int, int, int);
  %}

%include "fast_brute.c"
