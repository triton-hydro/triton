// Extracted from src/kernels.h — Stage 3 header split.
// Contains halo_copy_to_gpu, halo_copy_from_gpu (Kokkos device kernels).

#ifndef TRITON_KERNELS_HALO_H
#define TRITON_KERNELS_HALO_H

#include "kernels.h"

namespace Kernels {

/** @brief It copies all halo cells flux value from halo flux bundle to main discharge array.
*
*  @param size Array size
*  @param nrows Number of rows in that domain/subdomain
*  @param ncols Number of columns in that domain/subdomain
*  @param h_arr Water depth array
*  @param qx_arr Discharge in x direction array
*  @param qy_arr Discharge in y direction array
*  @param halo Bundle array that contains only halo discharge cells
*/
  template<std::floating_point T>
  void halo_copy_to_gpu(int size, int nrows, int ncols,
                        T       * KOKKOS_RESTRICT h_arr ,
                        T       * KOKKOS_RESTRICT qx_arr,
                        T       * KOKKOS_RESTRICT qy_arr,
                        T const * KOKKOS_RESTRICT halo  )
  {
    triton::parallel_for( AUTO_LABEL() , size , KOKKOS_LAMBDA (int id) {

      int index =  id + 2*ncols*GHOST_CELL_PADDING;
      if(id < ncols*GHOST_CELL_PADDING)
      {
        index = id;
      }

      h_arr[id]  = halo[index + 0*GHOST_CELL_PADDING*ncols];
      qx_arr[id] = halo[index + 1*GHOST_CELL_PADDING*ncols];
      qy_arr[id] = halo[index + 2*GHOST_CELL_PADDING*ncols];

      int temp = id + (nrows - 2*GHOST_CELL_PADDING)*ncols;

      h_arr[temp]  = halo[index + 6*GHOST_CELL_PADDING*ncols];
      qx_arr[temp] = halo[index + 7*GHOST_CELL_PADDING*ncols];
      qy_arr[temp] = halo[index + 8*GHOST_CELL_PADDING*ncols];

    });
  }


/** @brief It copies all halo cells from main array to halo flux bundle.
*
*  @param size Array size
*  @param nrows Number of rows in that domain/subdomain
*  @param ncols Number of columns in that domain/subdomain
*  @param h_arr Water depth array
*  @param qx_arr Discharge in x direction array
*  @param qy_arr Discharge in y direction array
*  @param halo Bundle array that contains only halo discharge cells
*/
  template<std::floating_point T>
  void halo_copy_from_gpu(int size, int nrows, int ncols,
                          T const * KOKKOS_RESTRICT h_arr ,
                          T const * KOKKOS_RESTRICT qx_arr,
                          T const * KOKKOS_RESTRICT qy_arr,
                          T       * KOKKOS_RESTRICT halo  )
  {
    triton::parallel_for( AUTO_LABEL() , size , KOKKOS_LAMBDA (int id) {

      int index = id + 2*ncols*GHOST_CELL_PADDING;
      if(id < ncols*GHOST_CELL_PADDING)
      {
        index = id;
      }

      halo[index + 0*GHOST_CELL_PADDING*ncols] = h_arr[id];
      halo[index + 1*GHOST_CELL_PADDING*ncols] = qx_arr[id];
      halo[index + 2*GHOST_CELL_PADDING*ncols] = qy_arr[id];

      int temp = id + (nrows - 2*GHOST_CELL_PADDING)*ncols;

      halo[index + 6*GHOST_CELL_PADDING*ncols] = h_arr[temp];
      halo[index + 7*GHOST_CELL_PADDING*ncols] = qx_arr[temp];
      halo[index + 8*GHOST_CELL_PADDING*ncols] = qy_arr[temp];
    });
  }

} // namespace Kernels

#endif // TRITON_KERNELS_HALO_H
