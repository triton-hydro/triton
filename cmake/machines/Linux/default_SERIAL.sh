#!/bin/bash

export TRITON_BACKEND="SERIAL"
export TRITON_ARCH="NATIVE"
export TRITON_COMPILER=mpic++
export TRITON_RUN_COMMAND="mpirun -n 8"
