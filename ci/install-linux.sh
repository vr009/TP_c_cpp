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