cd %CODE_DIR%\yael
:: Prebuilt SWIG
grabzippedurl.py http://downloads.sourceforge.net/project/swig/swigwin/swigwin-3.0.2/swigwin-3.0.2.zip
sh -c "./configure.sh --enable-numpy --enable-arpack --swig=swigwin-3.0.2/swig.exe"
sh -c "make"
 
ARPACKLDFLAGS=/usr/lib64/libarpack.so.2
 
http://icl.cs.utk.edu/lapack-for-windows/libraries/VisualStudio/3.5.0/Dynamic-MINGW/Win32/libblas.dll
http://icl.cs.utk.edu/lapack-for-windows/libraries/VisualStudio/3.5.0/Dynamic-MINGW/Win32/libblas.lib
http://icl.cs.utk.edu/lapack-for-windows/libraries/VisualStudio/3.5.0/Dynamic-MINGW/Win32/liblapack.dll 
http://icl.cs.utk.edu/lapack-for-windows/libraries/VisualStudio/3.5.0/Dynamic-MINGW/Win32/liblapack.lib


sed -i "s/lib64\/C:\/Program Files (x86)\/LAPACK\/lib/" makefile.inc 

code
svn co https://swig.svn.sourceforge.net/svnroot/swig/trunk swig
REM git clone https://github.com/swig/swig.git
cd %CODE_DIR%\swig
mingw-get install msys-perl
mingw-get install msys-gawk
mingw-get install mingw32-automake
REM mingw-get show | grep aclocal
REM mingw-get show | grep pcre
:: make sure fstab has mingw in it 
:: http://stackoverflow.com/questions/21162622/error-while-building-linphone-for-windows
REM cp C:\MinGW\msys\1.0\etc\fstab.sample C:\MinGW\msys\1.0\etc\fstab
wget ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.33.tar.gz
sh -c "./Tools/pcre-build.sh"
sh -c "./autogen.sh"
sh -c "./configure"
make -j9
make

cd %USERPROFILE%\code
mkdir arpack
cd arpack
grabzippedurl.py http://www.caam.rice.edu/software/ARPACK/SRC/arpack96.tar.gz
grabzippedurl.py http://www.caam.rice.edu/software/ARPACK/SRC/patch.tar.gz
cd ARPACK
cd %USERPROFILE%\code\arpack\ARPACK

sed -i "s/home = $(HOME)\/ARPACK/home = $(HOME)\/code\/arpack\/ARPACK/" ARmake.inc
sed -i "s/home/srcdir/" ARmake.inc
sed -i "s/f77/gfortran/" ARmake.inc
sed -i "s/^FFLAGS.*=.*/FFLAGS  = -g -O3 -fdefault-integer-8 -fPIC/" ARmake.inc
make lib

REM dllwrap --export-all-symbols BLAS/*.o LAPACK/*.o SRC/*.o UTIL/*.o -lg2c --output-def arpack_win32.def -o arpack_win32.dll
sh -c "dllwrap --export-all-symbols BLAS/*.o LAPACK/*.o SRC/*.o UTIL/*.o -lgfortran --output-def arpack_win32.def -o arpack_win32.dll"

sh -c "lib /machine:i386 /def:arpack_win32.def"


cd %USERPROFILE%\code
mkdir lapack
cd lapack
grabzippedurl.py http://netlib.org/lapack/lapack.tgz
cd lapack-3.5.0
mkdir build
cd build
cmake -G "MSYS Makefiles" -DBUILD_SHARED_LIBS=On ..
mingw32-make -j7 "MAKE=mingw32-make -j3" -f CMakeFiles\Makefile2 all
make install

cd %USERPROFILE%\code\lapack\lapack-3.5.0\build
