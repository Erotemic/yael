#export FC="gfortran"
#export FFLAGS="-g -O3 -fdefault-integer-8 -fPIC" 

#sudo apt-get install libarpack2-dev
#sudo apt-get install libsuperlu3-dev 
#sudo apt-get install gfortran 
 
./configure.sh --enable-numpy --enable-arpack
sed -i "s/lib64\/libarpack.so.2/lib\/libarpack.so.2/" makefile.inc 
#./configure.sh --enable-numpy

make
 
#sudo pip uninstall yael
sudo python setup.py develop
#-msse4
   

uninstall_yael(){
    cd $CODE_DIR/yael
    export PYSITE=$(python -c "import site; print(site.getsitepackages()[0])")
    sudo rm -rf yael.egg-info
    sudo rm $PYSITE/yael.egg-link
    sudo sed -i '/yael/d' $PYSITE/easy-install.pth
    

    make clean
    git clean -f

}

 
# Pip whines without a remote
sh -c 'cat > .git/config << EOF
[core]
    repositoryformatversion = 0
    filemode = true
    bare = false
    logallrefupdates = true
[remote "origin"]
    fetch = +refs/heads/*:refs/remotes/origin/*
    url = git@github.com:Erotemic/yael.git
[branch "master"]
    remote = origin
    merge = refs/heads/master
EOF'
