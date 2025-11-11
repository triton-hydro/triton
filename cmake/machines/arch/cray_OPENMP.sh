#!/bin/bash

module purge
module load PrgEnv-cray
module load cray-libsci

GDAL_DIR=/ccs/proj/nwp501/software/gdal/3.11.3
PROJ_DIR=/ccs/proj/nwp501/software/PROJ/9.6.1
TIFF_DIR=/ccs/proj/nwp501/software/libtiff/4.7.0
SQLITE3_DIR=/ccs/proj/nwp501/software/sqlite3/3.50.4

export CMAKE_PREFIX_PATH="$GDAL_DIR;$PROJ_DIR;$TIFF_DIR:$SQLITE3_DIR:$CMAKE_PREFIX_PATH"
export LD_LIBRARY_PATH="$GDAL_DIR/lib64:$PROJ_DIR/lib64:$TIFF_DIR/lib64:$SQLITE3_DIR/lib:$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH="$GDAL_DIR/lib64/pkgconfig:$PROJ_DIR/lib64/pkgconfig:$TIFF_DIR/lib64/pkgconfig:$SQLITE3_DIR/lib/pkgconfig:$PKG_CONFIG_PATH"
export PATH="$GDAL_DIR/bin:$PROJ_DIR/bin:$TIFF_DIR/bin:$SQLITE3_DIR/bin:$PATH"

export TRITON_BACKEND=OPENMP
export TRITON_COMPILER=CC
export TRITON_COMPILER_FLAGS="-fopenmp"
export TRITON_LINKER_FLAGS="-fopenmp"
export TRITON_DEBUG=OFF
export TRITON_RUN_COMMAND="srun -n 2"

export CRAYPE_LINK_TYPE=dynamic
export CRAY_CPU_TARGET=aarch64

export OMP_NUM_THREADS=2
export OMP_PROC_BIND=true
export OMP_WAIT_POLICY=PASSIVE
