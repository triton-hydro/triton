/** @file gputils.h
 *  @brief Header containing map of generic "gpu" to hip or cuda calls/constants/types
 *
 *  @author Mario Morales Hernandez
 *  @author Sudershan Gangrade
 *  @author Shih-Chieh Kao
 *  @author Michael Kelleher
 *  @author Matthew R. Norman
 *  @author Youngsung Kim
 *  @author Juan Manuel Perez Garcia de Carellan
 *  @author Ganesh Ghimire
 *  @bug No known bugs.
 */ 


#ifdef AMDGPU
#include "hip/hip_runtime.h"
#define gpuStream_t hipStream_t
#define gpuError_t hipError_t
#define gpuGetDevice hipGetDevice
#define gpuGetDeviceCount hipGetDeviceCount
#define gpuGetErrorString hipGetErrorString
#define gpuSuccess hipSuccess
#define gpuSetDevice hipSetDevice
#define gpuStreamCreate hipStreamCreate
#define gpuMalloc hipMalloc
#define gpuMemcpyAsync hipMemcpyAsync
#define gpuMemcpyHostToDevice hipMemcpyHostToDevice
#define gpuStreamSynchronize hipStreamSynchronize
#define gpuMemcpyDeviceToHost hipMemcpyDeviceToHost
#define gpuFree hipFree
#define gpuStreamDestroy hipStreamDestroy
#else
#define gpuStream_t cudaStream_t
#define gpuError_t cudaError_t
#define gpuGetDevice cudaGetDevice
#define gpuGetDeviceCount cudaGetDeviceCount
#define gpuGetErrorString cudaGetErrorString
#define gpuSuccess cudaSuccess
#define gpuSetDevice cudaSetDevice
#define gpuStreamCreate cudaStreamCreate
#define gpuMalloc cudaMalloc
#define gpuMemcpyAsync cudaMemcpyAsync
#define gpuMemcpyHostToDevice cudaMemcpyHostToDevice
#define gpuStreamSynchronize cudaStreamSynchronize
#define gpuMemcpyDeviceToHost cudaMemcpyDeviceToHost
#define gpuFree cudaFree
#define gpuStreamDestroy cudaStreamDestroy
#endif
