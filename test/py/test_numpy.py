#!/usr/bin/env python
import sys
#from yael.yael import *  # NOQA
from yael import yael
import numpy as np

print('Testing Yael <-> numpy interface')

if 'fvec_to_numpy' not in dir(yael):
    #globals():
    print('Numpy interface not compiled in')
    sys.exit(1)

print('float array')
numpy_a = np.array(list(range(5)), dtype='float32')
print(numpy_a)

print('-> Yael A')
yael_a = yael.FloatArray.acquirepointer(yael.numpy_to_fvec(numpy_a))
n = numpy_a.size
yael.fvec_print(yael_a, n)

print('-> Numpy A')
print(yael.fvec_to_numpy(yael_a, n))
print('int array')
numpy_a = np.array(list(range(5)), dtype='int32')
print(numpy_a)

print('-> Yael A2')
yael_a = yael.IntArray.acquirepointer(yael.numpy_to_ivec(numpy_a))
n = numpy_a.size
yael.ivec_print(yael_a, n)

print('-> Numpy A2')
print(yael.ivec_to_numpy(yael_a, n))
print('float array, pass by reference')
numpy_a = np.array(list(range(5)), dtype='float32')
print(numpy_a)


if '--force-crash' in sys.argv:
    yael_a = yael.FloatArray.acquirepointer(yael.numpy_to_fvec_ref(numpy_a))
    n = numpy_a.size
    yael.fvec_print(yael_a, n)
    del numpy_a
    print('Forced Crash Example!')
    yael.fvec_print(yael_a, n)
