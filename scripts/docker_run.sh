docker run -it --rm \
    --name qtdev-run \
    --hostname "$(hostname)" \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v "$PWD":/workspace \
    -v "$PWD/build-docker":/workspace/build \
    qtdev \
    /workspace/build/bin/MolarTracker
