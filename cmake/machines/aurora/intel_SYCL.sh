#!/bin/bash

source /usr/share/lmod/lmod/init/bash
module reset
module load oneapi/release/2025.0.5 cmake cray-pals

export MPIR_CVAR_ENABLE_GPU=1
export MPICH_GPU_SUPPORT_ENABLED=1

export TRITON_COMPILER=mpic++
export TRITON_BACKEND="SYCL"
export TRITON_ARCH="INTEL_PVC"
export TRITON_COMPILER_FLAGS="-fp-model=precise -O3 -qopenmp -Wno-unused-result"
export TRITON_LINKER_FLAGS="-qopenmp"
export TRITON_RUN_COMMAND="mpiexec -n 1 --ppn 4 --depth=8 --cpu-bind depth --env OMP_NUM_THREADS=1 -env OMP_PLACES=threads"
export TRITON_DEBUG=OFF
