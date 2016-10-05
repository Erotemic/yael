Python interface
================

High-level interface
--------------------

The high-level interface of yael is ``ynumpy``. It gives access to the
the functions of Yael that are not implemented in Numpy or are
implemented inefficiently. The functions take a few mandatory
arguments, optional ones can be used to change the default behaviour. 

All matrix arguments must be in C-order ``float32`` arrays, because
numpy's support for Fortran-style indexing (used in the Matlab and C
versions) is close to unusable. Therefore, in the C API
documentation, all references to "columns" should become "lines".

The available functions are:

* ``knn(queries, base, nnn = 1, distance_type = 2, nt = 1)`` Computes
  the exact `nnn` nearest-neighbors of each line of the ``queries`` in
  each of the lines in the ``base`` matrix. Supported distance types
  are 2 (Euclidean), 1 (L1), 3 (symmetric Chi^2), 5 (histogram
  intersection). The outputs are a n-by-nnn table of int32 indices and
  a squared distance table of the same size. The option ``nt`` gives
  the number of computing threads to use.

* ``kmeans(v, k, distance_type = 2, nt = 1, niter = 30, seed = 0, redo = 1, verbose = True, output = 'centroids')``
  computes a k-means on the set of vectors ``v`` with ``k`` centroids. Returns 
  the kmeans centroids as a n-by-k matrix. When the ouput is set to ``full`` it additionally 
  returns ``(centroids, qerr, dis, assign, nassign)``:
  the quantization error, the vector-centroid distance matrix, the assignment table and the 
  number of assignments per centroid.

* ``cross_distances(a, b, distance_type = 12)`` computes the pairwise
  distances between the sets of vectors ``a`` and ``b``, expressed as
  matrices. The ``distance_type`` argument has the same meaning as for ``knn``.

* ``partial_pca(mat, nev = 6, nt = 1)`` computes a partial PCA matrix
  for vectors in matrix ``mat``. It returns ``(avg, singvals,
  pcamat)``: the average vector, the singular values and the PCA
  matrix, with ``nev`` lines.

* ``gmm_learn(v, k, nt = 1, niter = 30, seed = 0, redo = 1)`` computes
  a Gaussian Mixture Model with diagonal covariance matrix, with ``k``
  centroids for the vector in ``v``. It returns a triplet ``(w, mu,
  sigma)`` containing the weight of each Gaussian, the centroids and
  the diagonal covariance matrix.

Yael should be compiled with ``--enable-numpy``. Assuming that the
``PYTHONPATH`` environment variable is set to Yael's installation
root, the recommended way of using this interface is with::

  from yael import ynumpy 
  ynumpy.knn(...)

Low-level interface
-------------------

In the low-level interface, all of the C Yael functions are accessible. 

Assuming that the ``PYTHONPATH`` environment variable is set to Yael's
installation root, importing the Yael interface and creating a new
vector is done as::

  from yael import yael
  a = yael.fvec_new_0(5)


In order to shorten the call, one could also import the function 
in the current namespace, as::

  from yael.yael import *
  a = fvec_new_0(5)

However, we do not advise to do so, in order to avoid function name
conflicts when using other python libraries jointly with Yael.

Guidelines for the wrapping process
```````````````````````````````````

* for most of the objects, memory is **not** managed by Python. They
  must be free'd explicitly. The main exception is for vectors, which
  can be explicitly acquired by Python so that they are
  garbage-collected like a Python object

* arrays for simple types are called ``ivec``, ``fvec``, etc. Usage:

  * ``a = ivec(4)`` constructs an array of 4 ints, accessible in Python 
    with ``a[2]``, as one would expect. There is no bound checking:
    the Python object does not know about the size of the array (like
    with C pointers).

  * ``a.cast()`` returns an ``int*`` usable as a C function argument
    (most of the time, the cast is automatic, and ``a`` can be used
    when a function expects an ``int *``).

  * if a C function returns an ``int*``, ``b = ivec.frompointer(x)``
    makes the Python ``a[i]`` valid to access C's ``x[i]``.

  * ``b.plus(2)`` returns ``x + 2`` (pointer arithmetic).

  * ``b = ivec.acquirepointer(x)`` will, in addition, call ``free(x)``
    when the Python object ``b`` is deleted. This function therefore
    ensures that ``x`` will be cleaned up by the Python garbage collector. 
    Often, when a C function returns a newly allocated pointer ``x``,
    it is advisable to immediately do ``x=ivec.acquirepointer(x)``.

  * ``a.clear(3)`` clears out the 3 first elements of the vector.

  * if ``c`` is another ``int*``, ``a.copyfrom(c, 1, 2)`` will copy 2
    elements from ``c`` to ``a`` at offset 1 (ie. ``a[1] = c[0]`` and
    ``a[2] = c[1]``).

  * ``a.tostring(3)`` returns a Python string with the 3 first
    elements of ``a`` as raw binary data. They can be used eg. in the
    ``array`` module.

  * similarly, ``a.fromstring(s)`` fills the first elements of ``a``
    with raw values read from the string.

* all wrapped functions release Python's Global Interpreter Lock (to
  allow multithreaded execution), so Python API functions should not 
  be called in C code.

* output arguments in the C code (their names end in ``_out``) are
  combined with the function results tuples.

Low-level to Numpy interface
````````````````````````````

If Yael is configured with ``--enable-numpy``, arrays can be exchanged
with Numpy arrays. This is done through a series of functions with
self-explanatory names::

  fvec_to_numpy 
  ivec_to_numpy 
  numpy_to_fvec 
  numpy_to_ivec 

Arrays corresponding to Yael's ``fvec`` are of Numpy's
``dtype='float32'``. Moving from yael to numpy produces line vectors,
that can be reshaped to matrices if needed.

These functions copy their arguments. To share the same data buffer
between Yael and Numpy, suffix the function with ``_ref``.

See the ``test_numpy.py`` program for an example usage. 


ctypes interface
````````````````

Arrays can also be exchanged with ctypes. This is done by converting
pointers to integers. See ``test_ctypes.py`` for an example.




