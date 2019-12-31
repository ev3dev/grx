#!/bin/sh

set -e

project="grx"

script_dir=$(dirname $(readlink -f "${0}"))

case ${1} in
    armel|armhf)
        arch=${1}
        ;;
    *)
        echo "Error: Must specify 'armel' or 'armhf'"
        exit 1
        ;;
esac

if ! which docker >/dev/null; then
    echo "Error: Docker is not installed"
    exit 1
fi

build_dir=build-${arch}
image_name="${project}-${arch}"
container_name="${project}_${arch}"

mkdir -p ${build_dir}

docker build \
    --tag ${image_name} \
    --no-cache \
    --file "${script_dir}/${arch}.dockerfile" \
    "${script_dir}/"

docker rm --force ${container_name} >/dev/null 2>&1 || true
docker run \
    --volume "$(readlink -f ${build_dir}):/build" \
    --volume "$(pwd):/src" \
    --workdir /build \
    --name ${container_name} \
    --env "TERM=${TERM}" \
    --env "DESTDIR=/build/dist" \
    --tty \
    --detach \
    ${image_name} tail

docker exec --tty ${container_name} cmake /src \
    -DCMAKE_BUILD_TYPE=Debug \
    -DGRX_PLUGIN_GTK3=No \
    -DGRX_ENABLE_DOC=No

echo "Done. You can now compile by running 'docker exec --tty ${container_name} make'"
