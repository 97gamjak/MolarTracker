docker run -d \
  --name qtdev \
  -v "$PWD":/workspace \
  -v "$PWD/build-docker":/workspace/build \
  -v qtdev-ccache:/ccache \
  qtdev

docker exec qtdev cmake -S /workspace -B /workspace/build -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
