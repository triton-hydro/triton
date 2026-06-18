// Extracted from src/kernels.h — Stage 3 header split.
// Contains update_cells (Kokkos device kernel).

#ifndef TRITON_KERNELS_STATE_H
#define TRITON_KERNELS_STATE_H

#include "kernels.h"

namespace Kernels {

/** @brief It calculates each cells new depth, fluxes and updates them.
*
*  @param size Array size
*  @param nrows Number of rows in that domain/subdomain
*  @param ncols Number of columns in that domain/subdomain
*  @param dt Time step size
*  @param h_arr Water depth array
*  @param qx_arr Discharge in x direction array
*  @param qy_arr Discharge in y direction array
*  @param dem DEM array in that domain/subdomain (elevation)
*  @param n_arr Manning array in that domain/subdomain
*  @param rhsh0 RHS array to store partial water depth contributions (east-north)
*  @param rhsh1 RHS array to store partial water depth contributions (west-south)
*  @param rhsqx0 RHS array to store partial discharge in x direction contributions (east-north)
*  @param rhsqx1 RHS array to store partial discharge in x direction contributions (west-south)
*  @param rhsqy0 RHS array to store partial discharge in y direction contributions (east-north)
*  @param rhsqy1 RHS array to store partial discharge in y direction contributions (west-south)
*  @param hextra Minimum depth (tolerance below water is at rest)
*/
  template<std::floating_point T>
  void update_cells(int size, int nrows, int ncols, T dt,
                    T       * KOKKOS_RESTRICT h_arr ,
                    T       * KOKKOS_RESTRICT qx_arr,
                    T       * KOKKOS_RESTRICT qy_arr,
                    T const * KOKKOS_RESTRICT dem   ,
                    T const * KOKKOS_RESTRICT n_arr ,
                    T       * KOKKOS_RESTRICT rhsh0 ,
                    T       * KOKKOS_RESTRICT rhsh1 ,
                    T       * KOKKOS_RESTRICT rhsqx0,
                    T       * KOKKOS_RESTRICT rhsqx1,
                    T       * KOKKOS_RESTRICT rhsqy0,
                    T       * KOKKOS_RESTRICT rhsqy1,
                    T hextra)
  {
    triton::parallel_for( AUTO_LABEL() , size , KOKKOS_LAMBDA (int id) {

      int ix = (id / ncols);  //row id
      int iy = (id % ncols);  //col id

      bool
      is_top = (ix <= GHOST_CELL_PADDING-1),
      is_btm = (ix >= nrows - GHOST_CELL_PADDING),
      is_lt = (iy <= GHOST_CELL_PADDING-1),
      is_rt = (iy >= ncols - GHOST_CELL_PADDING);

      if (is_top || is_lt || is_rt || is_btm) //exclude halo cells
      {
        return;
      }
      T hij, qxij, qyij;

      T hn = h_arr[id];

      //update depth
      hij = hn - rhsh0[id] - rhsh1[id];

      //if there are negative depths, there are removed. Should be residual values (close to machine accuracy but negative)
      if (hij < 0.0)
      {
        hij = 0.0;
      }

      //if water is below hextra, velocities are removed
      if (hij < hextra)
      {
        qxij = 0.0;
        qyij = 0.0;
      }
      else
      {
        //update friction and momentum
        T mx = qx_arr[id] - rhsqx0[id] - rhsqx1[id];
        T my = qy_arr[id] - rhsqy0[id] - rhsqy1[id];

        T modM = sqrt(mx*mx + my * my);
        if (n_arr[id] > EPS12 && hn >= hextra && modM > EPS12)
        {
          T tt = dt * _G_*n_arr[id] * modM / (hn*hn*cbrt(hn));
          qxij = -0.5*(mx - mx * sqrt(1.0 + 4.0*tt)) / tt;
          qyij = -0.5*(my - my * sqrt(1.0 + 4.0*tt)) / tt;
        }
        else
        {
          qxij = mx;
          qyij = my;
        }
      }

      h_arr[id]=hij;
      qx_arr[id]=qxij;
      qy_arr[id]=qyij;

    });
  }

} // namespace Kernels

#endif // TRITON_KERNELS_STATE_H
