#!/bin/sh
set -eu

DOCKER_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PROJECT_DIR=$(dirname "$DOCKER_DIR")
OUTPUT_DIR=$(pwd)
IMAGE=samp-gcc34:latest

if ! docker info >/dev/null 2>&1; then
    echo "Docker is not available." >&2
    exit 1
fi

if ! docker image inspect "$IMAGE" >/dev/null 2>&1; then
    docker build --platform linux/386 -t "$IMAGE" -f "$DOCKER_DIR/Dockerfile" "$DOCKER_DIR"
fi

CONTAINER=$(docker run -d --platform linux/386 -w /work "$IMAGE" /bin/sh -c 'while :; do sleep 3600; done')
cleanup() {
    docker rm -f "$CONTAINER" >/dev/null 2>&1 || true
}
trap cleanup EXIT

docker exec "$CONTAINER" mkdir -p /work/tools /work/source /work/third-party

docker cp "$PROJECT_DIR/tools/announce" "$CONTAINER:/work/tools/"
docker cp "$PROJECT_DIR/server" "$CONTAINER:/work/"

docker cp "$PROJECT_DIR/source/httpclient.cpp" "$CONTAINER:/work/source/"
docker cp "$PROJECT_DIR/source/httpclient.h" "$CONTAINER:/work/source/"
docker cp "$PROJECT_DIR/source/static_assert.h" "$CONTAINER:/work/source/"
docker cp "$PROJECT_DIR/source/runutil.cpp" "$CONTAINER:/work/source/"
docker cp "$PROJECT_DIR/source/runutil.h" "$CONTAINER:/work/source/"

docker cp "$PROJECT_DIR/third-party/amx" "$CONTAINER:/work/third-party/"
docker cp "$PROJECT_DIR/third-party/lzma" "$CONTAINER:/work/third-party/"
docker cp "$PROJECT_DIR/third-party/RakNet" "$CONTAINER:/work/third-party/"
docker cp "$PROJECT_DIR/third-party/sqlite" "$CONTAINER:/work/third-party/"

docker exec -w /work/tools/announce "$CONTAINER" make -B CC=gcc-3.4 CXX=g++-3.4
docker exec -w /work/server "$CONTAINER" make -B CC=gcc-3.4 CXX=g++-3.4

docker cp "$CONTAINER:/work/tools/announce/announce" "$OUTPUT_DIR/announce"
docker cp "$CONTAINER:/work/server/samp03svr" "$OUTPUT_DIR/samp03svr"
printf 'Built: %s/announce and %s/samp03svr\n' "$OUTPUT_DIR" "$OUTPUT_DIR"
