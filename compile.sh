rebuild "$@"

if [[ "$rebuild" == "true" ]]; then
  rm -rf ./build
fi
mkdir -p ./build

cd ./build
cmake ..
make

ctest -V --rerun-failed --output-on-failure
