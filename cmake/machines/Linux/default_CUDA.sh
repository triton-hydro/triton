#!/bin/bash

export TRITON_BACKEND="CUDA"
export TRITON_COMPILER=mpic++
export TRITON_RUN_COMMAND="mpirun -n 1"

