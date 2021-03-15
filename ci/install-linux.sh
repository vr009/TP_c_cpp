GTEST_DIR=$(pwd)/gtest
GTEST_URL=https://github.com/google/googletest/archive/release-1.10.0.tar.gz
GTEST_TAR=gtest-release-1.10.0.tar.gz

wget --quit $GTEST_URL
mkdir -p $GTEST_DIR
tar -xzf $CMAKE_TAR -C $CMAKE_DIR
cd ${GTEST_DIR}
mkdir build
cd build
cmake ..
make install
export PATH=$GTEST_DIR/bin:$PATH