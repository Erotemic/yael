export FC="gfortran"
export FFLAGS="-g -O3 -fdefault-integer-8 -fPIC" 

#sudo apt-get install libarpack2-dev
#sudo apt-get install libsuperlu3-dev 
#sudo apt-get install gfortran 
 
./configure.sh --enable-numpy --enable-arpack
sed -i "s/lib64\/libarpack.so.2/lib\/libarpack.so.2/" makefile.inc 

make
#-msse4
