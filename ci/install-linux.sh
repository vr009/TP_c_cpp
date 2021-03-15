#GTEST_DIR=~/gtest
#GTEST_URL=https://github.com/google/googletest/archive/release-1.10.0.tar.gz
#GTEST_TAR=gtest-release-1.10.0.tar.gz
#
#wget $GTEST_URL
##mkdir -p $GTEST_DIR
#tar -xvf $GTEST_TAR $GTEST_DIR
##cd ${GTEST_DIR}
#cd googletest-release-1.10.0
#mkdir build
#cd build
#cmake ..
#make install
#export PATH=$GTEST_DIR/bin:$PATH
sudo apt-get update
sudo apt-get install valgrind
sudo apt-get install clang-format
sudo apt-get install lcov
sudo apt-get install libstdc++6
sudo apt-get install cmake
 # Google tests
sudo apt-get install libgtest-dev
cd /usr/src/gtest
sudo cmake .
sudo make
sudo cp *.a /usr/lib
cd -