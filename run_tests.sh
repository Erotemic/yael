python -c "import yael; print(dir(yael))"
python -c "from yael import yael; print(dir(yael)); print(yael.__package__)"
python test/test_ctypes.py 
python test/test_kmeans_alt_dist.py

python test/py/test_kmeans.py
python test/py/test_mmul.py
python test/py/test_nn.py 
python test/py/test_partial_svd.py
python test/py/test_ynumpy.py

python test/py/test_numpy.py  
