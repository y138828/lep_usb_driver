#!/bin/bash

BUILD_TYPE=release
BUILD_DIR=build
PLATFORM=x64
DEVMODE=1
UNAME=$(uname)
KERNEL_VERSION=$(uname -r)
KERNEL_MACHINE=$(uname -m)
KERNEL_NAME=$(uname -s)
if [ x${KERNEL_MACHINE} == xmips64 ] ; then
    KERNEL_DIR="/usr/src/kernels/${KERNEL_VERSION}"
else
    KERNEL_DIR="/usr/src/linux-headers-${KERNEL_VERSION}"
fi


function die()
{
    echo "Error: $*" >&2
    exit 1
}

function get_cpu_count()
{
    if [ ${UNAME} = "Darwin" ] ; then
        CPU_COUNT=$(sysctl -n hw.ncpu)
    else
        CPU_COUNT=$(grep processor /proc/cpuinfo | wc -l)
    fi
}

function usage()
{
    echo "Usage: $0 [ debug | release ] [dev]" >&2
    exit 1
}

echo "BUILD_TYPE: $BUILD_TYPE"
get_cpu_count
if [ x$CPU_COUNT = x ] ; then
   CPU_COUNT=4
fi
 
echo "CPU count: ${CPU_COUNT}." 

echo "remove build directory"
rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}



pushd build
    ret=$(which clang)
    if [ 0 -eq $? ] ; then
        cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DDEVMODE=${DEVMODE} -DKERNEL_NAME=${KERNEL_NAME} -DKERNEL_VERSION=${KERNEL_VERSION} -DKERNEL_MACHINE=${KERNEL_MACHINE} -DKERNEL_DIR=${KERNEL_DIR} -DPLATFORM_TYPE=$PLATFORM -DCMAKE_TOOLCHAIN_FILE=../linux.toolchain.cmake -DCPU_COUNT=$CPU_COUNT ..
    else
        cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DDEVMODE=${DEVMODE} -DKERNEL_NAME=${KERNEL_NAME} -DKERNEL_VERSION=${KERNEL_VERSION} -DKERNEL_MACHINE=${KERNEL_MACHINE} -DKERNEL_DIR=${KERNEL_DIR} -DPLATFORM_TYPE=$PLATFORM -DCPU_COUNT=$CPU_COUNT ..
    fi
    make VERBOSE=1 -j ${CPU_COUNT}
    make install
    # make package
popd
