#!/bin/bash

source ${MODULESHOME}/init/bash
module reset
module load PrgEnv-cray cmake cray-python rocm craype-accel-amd-gfx90a

export CRAYPE_LINK_TYPE=dynamic
export MPICH_GPU_SUPPORT_ENABLED=1

GDAL_DIR=/ccs/proj/cli190/software/gdal/3.11.3
PROJ_DIR=/ccs/proj/cli190/software/PROJ/9.6.1
TIFF_DIR=/ccs/proj/cli190/software/libtiff/4.7.0

export CMAKE_PREFIX_PATH="$GDAL_DIR;$PROJ_DIR;$TIFF_DIR:$CMAKE_PREFIX_PATH"
export LD_LIBRARY_PATH="$GDAL_DIR/lib64:$PROJ_DIR/lib64:$TIFF_DIR/lib64:$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH="$GDAL_DIR/lib64/pkgconfig:$PROJ_DIR/lib64/pkgconfig:$TIFF_DIR/lib64/pkgconfig:$PKG_CONFIG_PATH"
export PATH="$GDAL_DIR/bin:$PROJ_DIR/bin:$TIFF_DIR/bin:$PATH"

export TRITON_BACKEND="HIP"
export TRITON_ARCH="AMD_GFX90A"
export TRITON_COMPILER=CC
export TRITON_COMPILER_FLAGS="-DTRITON_HIP_LAUNCHER -O1 "
export TRITON_LINKER_FLAGS=""
export TRITON_DEBUG=OFF
export TRITON_RUN_COMMAND="srun -n 8"

