/** @file kokkos_utils.h
 *  @brief Header containing map of kokkos calls/constants/types
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


#pragma once

namespace triton {
  #if defined(TRITON_HIP_LAUNCHER) || defined(TRITON_CUDA_LAUNCHER)
    #if defined(TRITON_HIP_LAUNCHER)
      int constexpr VecLen = 256;
    #else
      int constexpr VecLen = 128;
    #endif
    template <class F> __global__ __launch_bounds__(VecLen)
    void Kernel( int size , F f ) {
      int i = blockIdx.x*blockDim.x + threadIdx.x;
      if (i < size) f(i);
    }
    template<class F>
    inline void parallel_for( std::string lab ,  int size , F const &f ) {
      Kernel <<< (unsigned int) (size-1)/VecLen+1 , VecLen , 0 , 0 >>> ( size , f );
    }
  #else
    template<class F>
    inline void parallel_for( std::string lab ,  int size , F const &f ) {
      Kokkos::parallel_for(lab,size,f);
    }
  #endif
}

typedef int gpuStream_t;

inline void gpuMalloc(void **ptr , size_t size) {  *ptr = Kokkos::kokkos_malloc(size); }

inline void gpuFree(void *ptr) { Kokkos::kokkos_free(ptr); }

int constexpr gpuMemcpyHostToDevice   = 0;
int constexpr gpuMemcpyDeviceToHost   = 1;
int constexpr gpuMemcpyDeviceToDevice = 2;
int constexpr gpuMemcpyHostToHost     = 3;
inline void gpuMemcpyAsync( void *dest_p , void *src_p , size_t bytes , int dir , gpuStream_t str) {
  typedef typename Kokkos::DefaultHostExecutionSpace::memory_space H_SPACE;
  typedef typename Kokkos::DefaultExecutionSpace::memory_space     D_SPACE;
  typedef typename Kokkos::MemoryTraits<Kokkos::Unmanaged>         UMG;
  char *dest_p_char = static_cast<char *>(dest_p);
  char *src_p_char  = static_cast<char *>(src_p );
  if        (dir == gpuMemcpyHostToDevice) {
    Kokkos::View<char *,Kokkos::LayoutRight,D_SPACE,UMG> dest(dest_p_char,bytes);
    Kokkos::View<char *,Kokkos::LayoutRight,H_SPACE,UMG> src (src_p_char ,bytes);
    Kokkos::deep_copy( dest , src );
  } else if (dir == gpuMemcpyDeviceToHost) {
    Kokkos::View<char *,Kokkos::LayoutRight,H_SPACE,UMG> dest(dest_p_char,bytes);
    Kokkos::View<char *,Kokkos::LayoutRight,D_SPACE,UMG> src (src_p_char ,bytes);
    Kokkos::deep_copy( dest , src );
  } else if (dir == gpuMemcpyDeviceToDevice) {
    Kokkos::View<char *,Kokkos::LayoutRight,D_SPACE,UMG> dest(dest_p_char,bytes);
    Kokkos::View<char *,Kokkos::LayoutRight,D_SPACE,UMG> src (src_p_char ,bytes);
    Kokkos::deep_copy( dest , src );
  } else if (dir == gpuMemcpyHostToHost) {
    Kokkos::View<char *,Kokkos::LayoutRight,H_SPACE,UMG> dest(dest_p_char,bytes);
    Kokkos::View<char *,Kokkos::LayoutRight,H_SPACE,UMG> src (src_p_char ,bytes);
    Kokkos::deep_copy( dest , src );
  } else {
    throw std::runtime_error("ERROR: gpuMemcpyAsync called with invalid direction parameter");
  }
}

inline void gpuStreamSynchronize(gpuStream_t str) { Kokkos::fence(); }

inline void gpuStreamDestroy(gpuStream_t str) { Kokkos::fence(); }

inline void gpuStreamCreate(gpuStream_t *str) { }

