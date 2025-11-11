#!/bin/bash

export TRITON_BACKEND="OPENMP"
export TRITON_ARCH="NATIVE"
export TRITON_COMPILER=mpic++
export TRITON_COMPILER_FLAGS="-fopenmp"
export TRITON_LINKER_FLAGS="-fopenmp"
export TRITON_RUN_COMMAND="mpirun -n 8"
