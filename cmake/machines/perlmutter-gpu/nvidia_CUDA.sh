#!/bin/bash

module load PrgEnv-nvidia
module load cudatoolkit

export TRITON_BACKEND="CUDA"
export TRITON_ARCH="AMPERE80"
export TRITON_COMPILER=CC
export TRITON_COMPILER_FLAGS="-DACTIVE_GPU=1"
export TRITON_LINKER_FLAGS=
export TRITON_DEBUG=OFF
export TRITON_RUN_COMMAND="srun -N 1 --gpus=4"

export CRAYPE_LINK_TYPE=dynamic
export CUDA_DIR=${CUDA_HOME}
export CRAY_CPU_TARGET=${CPU}

