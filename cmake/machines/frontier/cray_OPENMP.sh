#!/bin/bash

source ${MODULESHOME}/init/bash
module reset
module load PrgEnv-cray cmake cray-python

export CRAYPE_LINK_TYPE=dynamic

GDAL_DIR=/ccs/proj/cli190/software/gdal/3.11.3
PROJ_DIR=/ccs/proj/cli190/software/PROJ/9.6.1
TIFF_DIR=/ccs/proj/cli190/software/libtiff/4.7.0

export CMAKE_PREFIX_PATH="$GDAL_DIR;$PROJ_DIR;$TIFF_DIR:$CMAKE_PREFIX_PATH"
export LD_LIBRARY_PATH="$GDAL_DIR/lib64:$PROJ_DIR/lib64:$TIFF_DIR/lib64:$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH="$GDAL_DIR/lib64/pkgconfig:$PROJ_DIR/lib64/pkgconfig:$TIFF_DIR/lib64/pkgconfig:$PKG_CONFIG_PATH"
export PATH="$GDAL_DIR/bin:$PROJ_DIR/bin:$TIFF_DIR/bin:$PATH"

export TRITON_BACKEND="OPENMP"
export TRITON_ARCH="NATIVE"
export TRITON_COMPILER=CC
export TRITON_COMPILER_FLAGS="-O2 -fopenmp"
export TRITON_LINKER_FLAGS="-fopenmp"
export TRITON_DEBUG=OFF
export TRITON_RUN_COMMAND="srun -n 8"


