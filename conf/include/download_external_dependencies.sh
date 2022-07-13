#!/bin/bash
# Downloads external HailoRT dependecies for offline compilation
set -e

EXTERNAL_DIR="hailort/external/"
EXTERNAL_CMAKE_FILE="$(realpath hailort/pre_build/external/CMakeLists.txt)"

mkdir -p $EXTERNAL_DIR
cd $EXTERNAL_DIR

clone() {
    name=$1
    git_url=$2
    rev=$3

    if [[ ! -d $name ]]
    then
        git clone $git_url $name --recurse-submodules
    fi

    pushd $name
    git checkout $rev
    popd
}

grep -oP 'git_clone\(\K[^)]+' $EXTERNAL_CMAKE_FILE | while read line
do
    clone $line
done