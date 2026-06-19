#!/bin/bash

module purge
module load PrgEnv-cray
module load cudatoolkit
module load craype-accel-nvidia80

# Supports multi-node simulations
module load craype-network-ucx
module load cray-mpich-ucx
module load ucx/1.20.0

export CC=cc
export CXX=CC
export FC=ftn

export MPICH_GPU_SUPPORT_ENABLED=1
export UCX_TLS=rc,sm,self,cuda,cuda_copy,cuda_ipc

GDAL_DIR=/autofs/nccs-svm1_proj/atm112/software/miller/gdal/gnu/13.3.0
GEOTIFF_DIR=/autofs/nccs-svm1_proj/atm112/software/miller/libgeotiff/gnu/13.3.0
PROJ_DIR=/autofs/nccs-svm1_proj/atm112/software/miller/PROJ/gnu/13.3.0
TIFF_DIR=/autofs/nccs-svm1_proj/atm112/software/miller/libtiff/gnu/13.3.0
SQLITE3_DIR=/autofs/nccs-svm1_proj/atm112/software/miller/sqlite3/gnu/13.3.0

export CMAKE_PREFIX_PATH="$GDAL_DIR;$GEOTIFF_DIR:$PROJ_DIR;$TIFF_DIR:$SQLITE3_DIR:$CMAKE_PREFIX_PATH"
export LD_LIBRARY_PATH="$GDAL_DIR/lib64:$GEOTIFF_DIR/lib:$PROJ_DIR/lib64:$TIFF_DIR/lib64:$SQLITE3_DIR/lib:$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH="$GDAL_DIR/lib64/pkgconfig:$GEOTIFF_DIR/lib/pkgconfig:$PROJ_DIR/lib64/pkgconfig:$TIFF_DIR/lib64/pkgconfig:$SQLITE3_DIR/lib/pkgconfig:$PKG_CONFIG_PATH"
export PATH="$GDAL_DIR/bin:$GEOTIFF_DIR/bin:$PROJ_DIR/bin:$TIFF_DIR/bin:$SQLITE3_DIR/bin:$PATH"

export TRITON_BACKEND="CUDA"
export TRITON_ARCH="AMPERE80"
export TRITON_COMPILER="CC"
export TRITON_COMPILER_FLAGS=
export TRITON_LINK_FLAGS=
export TRITON_DEBUG=OFF
export TRITON_RUN_COMMAND="srun -n 4 --gpus-per-task=1"

export CRAYPE_LINK_TYPE=dynamic
export CUDA_DIR=${CUDA_HOME}
export CRAY_CPU_TARGET=${CPU}
