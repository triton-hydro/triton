#!/bin/bash

module purge
module load craype-arm-grace
module load libfabric/2.3.1
module load craype-network-ofi
module load Core/25.04
module load DefApps
module load PrgEnv-gnu
module load cudatoolkit/25.9_13.0
module load craype-accel-nvidia90
module load cray-libsci/26.03.0

GDAL_DIR=/ccs/proj/nwp501/software/gdal/3.11.3
PROJ_DIR=/ccs/proj/nwp501/software/PROJ/9.6.1
TIFF_DIR=/ccs/proj/nwp501/software/libtiff/4.7.0
SQLITE3_DIR=/ccs/proj/nwp501/software/sqlite3/3.50.4

export CMAKE_PREFIX_PATH="$GDAL_DIR;$PROJ_DIR;$TIFF_DIR:$SQLITE3_DIR:$CMAKE_PREFIX_PATH"
export LD_LIBRARY_PATH="$GDAL_DIR/lib64:$PROJ_DIR/lib64:$TIFF_DIR/lib64:$SQLITE3_DIR/lib:$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH="$GDAL_DIR/lib64/pkgconfig:$PROJ_DIR/lib64/pkgconfig:$TIFF_DIR/lib64/pkgconfig:$SQLITE3_DIR/lib/pkgconfig:$PKG_CONFIG_PATH"
export PATH="$GDAL_DIR/bin:$PROJ_DIR/bin:$TIFF_DIR/bin:$SQLITE3_DIR/bin:$PATH"

export TRITON_BACKEND=CUDA
export TRITON_ARCH=HOPPER90
export TRITON_COMPILER=CC
export TRITON_COMPILER_FLAGS=
export TRITON_LINKER_FLAGS=
export TRITON_DEBUG=OFF
export TRITON_RUN_COMMAND="srun -n 4"

export CRAYPE_LINK_TYPE=dynamic
export CRAY_CPU_TARGET=aarch64

