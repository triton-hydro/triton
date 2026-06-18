// Extracted from src/kernels.h — Stage 3 header split.
// Contains wet_dry, wet_dry_qy_halo (Kokkos device kernels).

#ifndef TRITON_KERNELS_WETDRY_H
#define TRITON_KERNELS_WETDRY_H

#include "kernels.h"

namespace Kernels {

/** @brief It updates solid wall condition for all cells except q_y for halo cells.
*
*  @param size Array size
*  @param nrows Number of rows in that domain/subdomain
*  @param ncols Number of columns in that domain/subdomain
*  @param h_arr Water depth array
*  @param qx_arr Discharge in x direction array
*  @param qy_arr Discharge in y direction array
*  @param dem DEM array in that domain/subdomain (elevation)
*  @param max_h_arr Max water depth array
*  @param hextra Minimum depth (tolerance below water is at rest)
*  @param mpi_tasks Number of MPI tasks (domain decomposition)
*/
  template<std::floating_point T>
  void wet_dry(int size, int nrows, int ncols, T dt,
               T const * KOKKOS_RESTRICT h_arr    ,
               T       * KOKKOS_RESTRICT qx_arr   ,
               T       * KOKKOS_RESTRICT qy_arr   ,
               T const * KOKKOS_RESTRICT dem      ,
               T       * KOKKOS_RESTRICT max_h_arr,
               T hextra, int mpi_tasks)
  {
    triton::parallel_for( AUTO_LABEL() , size , KOKKOS_LAMBDA (int id) {

      int ix = (id / ncols);  //row id
      int iy = (id % ncols);  //col id
      int rx = (ix * ncols);  //row offset

      bool
      is_top = (ix <= GHOST_CELL_PADDING-1),
      is_btm = (ix >= nrows - GHOST_CELL_PADDING),
      is_lt = (iy <= GHOST_CELL_PADDING-1),
      is_rt = (iy >= ncols - GHOST_CELL_PADDING);


      if (is_top || is_lt || is_rt || is_btm) //exclude halo cells
      {
        return;
      }

      T hij = h_arr[id];
      T zij = dem[id];

      if (hij > hextra)
      {
        if (((hij + zij < dem[rx + iy + 1]) && (h_arr[rx + iy + 1] < EPS12)) || ((hij + zij < dem[rx + iy - 1]) && (h_arr[rx + iy - 1] < EPS12)))
        {
          qx_arr[id] = 0.0;
        }
        if(ix==GHOST_CELL_PADDING && mpi_tasks>1){ //first real row
            if ((hij + zij < dem[rx + ncols + iy]) && (h_arr[rx + ncols + iy] < EPS12))
            {
              qy_arr[id] = 0.0;
            }

        }else{
          if(ix==nrows - 2*GHOST_CELL_PADDING && mpi_tasks>1){ //last real row
            if ((hij + zij < dem[rx - ncols + iy]) && (h_arr[rx - ncols + iy] < EPS12))                       {
              qy_arr[id] = 0.0;
            }
          }else{
            if (((hij + zij < dem[rx - ncols + iy]) && (h_arr[rx - ncols + iy] < EPS12)) || ((hij + zij < dem[rx + ncols + iy]) && (h_arr[rx + ncols + iy] < EPS12)))
            {
              qy_arr[id] = 0.0;
            }

          }

        }
      }

      if(hij > max_h_arr[id]) max_h_arr[id] = hij;

    });
  }


/** @brief It updates q_y for halo cells.
*
*  @param size Array size
*  @param nrows Number of rows in that domain/subdomain
*  @param ncols Number of columns in that domain/subdomain
*  @param h_arr Water depth array
*  @param qy_arr Discharge in y direction array
*  @param dem DEM array in that domain/subdomain (elevation)
*  @param hextra Minimum depth (tolerance below water is at rest)
*/
  template<std::floating_point T>
  void wet_dry_qy_halo(int size, int nrows, int ncols,
                       T const * KOKKOS_RESTRICT h_arr ,
                       T       * KOKKOS_RESTRICT qy_arr,
                       T const * KOKKOS_RESTRICT dem   ,
                       T hextra)
  {
    triton::parallel_for( AUTO_LABEL() , size , KOKKOS_LAMBDA (int id) {
      int id1, id2;

      id1 =  id+(nrows - 2*GHOST_CELL_PADDING-1)*ncols;

      if(id < ncols*GHOST_CELL_PADDING)
      {
        id1=id;
      }

      id2=id1+ncols;

      T h1 = h_arr[id1];
      T h2 = h_arr[id2];
      T z1 = dem[id1];
      T z2 = dem[id2];

      if (h1 > hextra && h1+z1 < z2 && h2 < EPS12)
      {
        qy_arr[id1] = 0.0;
      }
      if (h2 > hextra && h2+z2 < z1 && h1 < EPS12)
      {
        qy_arr[id2] = 0.0;
      }

    });
  }

} // namespace Kernels

#endif // TRITON_KERNELS_WETDRY_H
