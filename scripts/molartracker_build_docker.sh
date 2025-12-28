if [[ -n $@ ]]
then
    docker exec qtdev rm -rf /workspace/build/*
    docker exec qtdev cmake -S /workspace -B /workspace/build -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
fi

docker exec qtdev cmake --build /workspace/build -j
