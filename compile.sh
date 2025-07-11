rm -rf ./build
mkdir -p ./build

cd ./build
cmake ..
make

./test_kv_store
