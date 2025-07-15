rm -rf ./build
mkdir -p ./build

cd ./build
cmake ..
make

ctest --output-on-failure -V
