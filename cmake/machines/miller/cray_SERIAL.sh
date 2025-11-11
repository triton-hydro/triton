#!/bin/bash

module purge
module load PrgEnv-cray

GDAL_DIR=/autofs/nccs-svm1_proj/atm112/software/miller/gdal/gnu/13.3.0
GEOTIFF_DIR=/autofs/nccs-svm1_proj/atm112/software/miller/libgeotiff/gnu/13.3.0
PROJ_DIR=/autofs/nccs-svm1_proj/atm112/software/miller/PROJ/gnu/13.3.0
TIFF_DIR=/autofs/nccs-svm1_proj/atm112/software/miller/libtiff/gnu/13.3.0
SQLITE3_DIR=/autofs/nccs-svm1_proj/atm112/software/miller/sqlite3/gnu/13.3.0

export CMAKE_PREFIX_PATH="$GDAL_DIR;$GEOTIFF_DIR:$PROJ_DIR;$TIFF_DIR:$SQLITE3_DIR:$CMAKE_PREFIX_PATH"
export LD_LIBRARY_PATH="$GDAL_DIR/lib64:$GEOTIFF_DIR/lib:$PROJ_DIR/lib64:$TIFF_DIR/lib64:$SQLITE3_DIR/lib:$LD_LIBRARY_PATH"
export PKG_CONFIG_PATH="$GDAL_DIR/lib64/pkgconfig:$GEOTIFF_DIR/lib/pkgconfig:$PROJ_DIR/lib64/pkgconfig:$TIFF_DIR/lib64/pkgconfig:$SQLITE3_DIR/lib/pkgconfig:$PKG_CONFIG_PATH"
export PATH="$GDAL_DIR/bin:$GEOTIFF_DIR/bin:$PROJ_DIR/bin:$TIFF_DIR/bin:$SQLITE3_DIR/bin:$PATH"

export TRITON_BACKEND=SERIAL
export TRITON_COMPILER=CC
export TRITON_COMPILER_FLAGS=
export TRITON_LINKER_FLAGS=
export TRITON_DEBUG=OFF
export TRITON_RUN_COMMAND="srun -n 2"

export CRAYPE_LINK_TYPE=dynamic
export CRAY_CPU_TARGET=x86-64
