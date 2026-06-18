// Extracted from src/kernels.h — Stage 3 header split.
// Contains flux_x, flux_y (Kokkos device kernels).

#ifndef TRITON_KERNELS_FLUX_H
#define TRITON_KERNELS_FLUX_H

#include "kernels.h"

namespace Kernels {

/** @brief It calculates flux in x direction.
*
*  @param size Array size
*  @param nrows Number of rows in that domain/subdomain
*  @param ncols Number of columns in that domain/subdomain
*  @param dx Cell size
*  @param dt Time step size
*  @param h_arr Water depth array
*  @param qx_arr Discharge in x direction array
*  @param qy_arr Discharge in y direction array
*  @param dem DEM array in that domain/subdomain (elevation)
*  @param sqrth Square root of water depth array
*  @param rhsh0 RHS array to store partial water depth contributions (east-north)
*  @param rhsh1 RHS array to store partial water depth contributions (west-south)
*  @param rhsqx0 RHS array to store partial discharge in x direction contributions (east-north)
*  @param rhsqx1 RHS array to store partial discharge in x direction contributions (west-south)
*  @param rhsqy0 RHS array to store partial discharge in y direction contributions (east-north)
*  @param rhsqy1 RHS array to store partial discharge in y direction contributions (west-south)
*  @param hextra Minimum depth (tolerance below water is at rest)
*/
  template<std::floating_point T>
  void flux_x(int size, int nrows, int ncols, T dx, T dt,
              T const * KOKKOS_RESTRICT h_arr ,
              T const * KOKKOS_RESTRICT qx_arr,
              T const * KOKKOS_RESTRICT qy_arr,
              T const * KOKKOS_RESTRICT dem   ,
              T const * KOKKOS_RESTRICT sqrth ,
              T       * KOKKOS_RESTRICT rhsh0 ,
              T       * KOKKOS_RESTRICT rhsh1 ,
              T       * KOKKOS_RESTRICT rhsqx0,
              T       * KOKKOS_RESTRICT rhsqx1,
              T       * KOKKOS_RESTRICT rhsqy0,
              T       * KOKKOS_RESTRICT rhsqy1,
              T hextra)
  {

    /**************************************
     *  RHS sketch
     *       _____ _____
     *      |     |     |
     *      |    0|1    |
     *      |_____|_____|
     *
     * ************************************/

    triton::parallel_for( AUTO_LABEL() , size , KOKKOS_LAMBDA (int id) {

      int ix = (id / ncols);  //row id
      int iy = (id % ncols);  //col id
      int rx = (ix * ncols);  //row offset

      bool
      is_top = (ix <= GHOST_CELL_PADDING-1),
      is_btm = (ix >= nrows - GHOST_CELL_PADDING),
      is_lt = (iy <= GHOST_CELL_PADDING-2),
      is_rt = (iy >= ncols - GHOST_CELL_PADDING);

      if (is_top || is_rt || is_lt || is_btm)  //top, right and bottom are not computed. For the X-flux they dont need to be computed. Caution! If domain decomposition changes, this would change (left and right interfaces should be reconsidered)*/
      {
        return;
      }

      int id1, id2;
      T nx, ny;

      id1 = id;
      id2 = rx + iy + 1;
      nx = 1.0;
      ny = 0.0;

      T h1 = h_arr[id1];
      T h2 = h_arr[id2];

      if (h1 > 0.0 || h2 > 0.0)
      {
        int i;
        T alpha[3], beta[3], eigen[3], eigenE[3]={0.0}, eigev[3][3], d1[3]={0.0}, d2[3]={0.0};
        T u1, u2, v1, v2;

        T qx1 = qx_arr[id1];
        T qy1 = qy_arr[id1];
        T z1 = dem[id1];

        T qx2 = qx_arr[id2];
        T qy2 = qy_arr[id2];
        T z2 = dem[id2];

        if (h1 >= hextra)
        {
          u1 = qx1 / h1;
          v1 = qy1 / h1;
        }
        else
        {
          u1 = 0.0;
          v1 = 0.0;
        }
        if (h2 >= hextra)
        {
          u2 = qx2 / h2;
          v2 = qy2 / h2;
        }
        else
        {
          u2 = 0.0;
          v2 = 0.0;
        }

        T sqh1 = sqrth[id1];
        T sqh2 = sqrth[id2];

        T h12 = 0.5*(h1 + h2);
        T dh = h2 - h1;
	T c12 = sqrt(_G_*h12);

        T u12 = (u1*sqh1 + u2 * sqh2) / (sqh1 + sqh2);
        T v12 = (v1*sqh1 + v2 * sqh2) / (sqh1 + sqh2);

        T un = u12 * nx + v12 * ny;

        eigen[0] = un - c12;
        eigen[1] = un;
        eigen[2] = un + c12;

        //entropy correction

        T e1 = u1 * nx + v1 * ny - SQRTG * sqh1;
        T e2 = u2 * nx + v2 * ny - SQRTG * sqh2;
        if (e1<0.0 && e2>0.0)
        {
          eigenE[0] = eigen[0] - e1 * (e2 - eigen[0]) / (e2 - e1);
          eigen[0] = e1 * (e2 - eigen[0]) / (e2 - e1);
        }

        e1 = u1 * nx + v1 * ny + SQRTG * sqh1;
        e2 = u2 * nx + v2 * ny + SQRTG * sqh2;

        if (e1<0.0 && e2>0.0)
        {
          eigenE[2] = eigen[2] - e2 * (eigen[2] - e1) / (e2 - e1);
          eigen[2] = e2 * (eigen[2] - e1) / (e2 - e1);
        }

        eigev[0][0] = 1.0;
        eigev[0][1] = u12 - c12 * nx;
        eigev[0][2] = v12 - c12 * ny;
        eigev[1][0] = 0.0;
        eigev[1][1] = -c12 * ny;
        eigev[1][2] = c12 * nx;
        eigev[2][0] = 1.0;
        eigev[2][1] = u12 + c12 * nx;
        eigev[2][2] = v12 + c12 * ny;

        alpha[0] = 0.5*(dh - (((qx2 - qx1)*nx + (qy2 - qy1)*ny) - un * dh) / c12);
        alpha[1] = (((qy2 - qy1) - v12 * dh)*nx - ((qx2 - qx1) - u12 * dh)*ny) / c12;
        alpha[2] = 0.5*(dh + (((qx2 - qx1)*nx + (qy2 - qy1)*ny) - un * dh) / c12);

        T dz = z2 - z1;
        T l1 = z1 + h1;
        T l2 = z2 + h2;
        T dzp = dz;
        T hp;

        if (dz >= 0.0)
        {
          hp = h1;
          if (l1 < z2)
          {
            dzp = h1;
          }
        }
        else
        {
          hp = h2;
          if (l2 < z1)
          {
            dzp = -h2;
          }
        }


        beta[0] = 0.5*_G_*(hp - 0.5*fabs(dzp))*dzp / c12;

        //wet-wet correction
        hp = h1 + alpha[0];
        if (eigen[0] * eigen[2]<0.0 && hp>0.0 && h1 > 0.0 && h2 > 0.0)  //subcritical
        {
          beta[0] = FMAX(beta[0], eigen[0]/(eigen[0]+eigenE[2])*(alpha[0] * eigen[0] - h1 * dx*0.5 / dt + eigenE[2]*h2));
          beta[0] = FMIN(beta[0], eigen[2]/(eigen[2]+eigenE[0])*(-alpha[2] * eigen[2] + h2 * dx*0.5 / dt + eigenE[0]*h1));
        }

        beta[1] = 0.0;
        beta[2] = -beta[0];

        l1 = hp - beta[0] / eigen[0]; //left intermediate state
        l2 = hp + beta[2] / eigen[2]; //right intermediate state

        if (l2 < -EPS12 && h2 < EPS12)   //send mass contributions to the left if right cell is dry
        {
          d1[0] += (eigen[0] * alpha[0] - beta[0]) + (eigen[2] * alpha[2] - beta[2]);
        }
        else
        {
          if (l1 < -EPS12 && h1 < EPS12)   //send mass contributions to the right if left cell is dry
          {
            d2[0] += (eigen[0] * alpha[0] - beta[0]) + (eigen[2] * alpha[2] - beta[2]);
          }
          else
          {
            for (i = 0; i < 3; i++)   //regular contributions
            {
              if (eigen[i] > 0.0)
              {
                d2[0] += (eigen[i] * alpha[i] - beta[i])*eigev[i][0];
                d2[1] += (eigen[i] * alpha[i] - beta[i])*eigev[i][1];
                d2[2] += (eigen[i] * alpha[i] - beta[i])*eigev[i][2];
              }
              else
              {
                d1[0] += (eigen[i] * alpha[i] - beta[i])*eigev[i][0];
                d1[1] += (eigen[i] * alpha[i] - beta[i])*eigev[i][1];
                d1[2] += (eigen[i] * alpha[i] - beta[i])*eigev[i][2];
              }
            }
          }
        }

        for (i = 0; i < 3; i++)   //entropy
        {
          //entropy
          if (eigenE[i] > 0.0)
          {
            d2[0] += (eigenE[i] * alpha[i])*eigev[i][0];
            d2[1] += (eigenE[i] * alpha[i])*eigev[i][1];
            d2[2] += (eigenE[i] * alpha[i])*eigev[i][2];
          }
          else
          {
            d1[0] += (eigenE[i] * alpha[i])*eigev[i][0];
            d1[1] += (eigenE[i] * alpha[i])*eigev[i][1];
            d1[2] += (eigenE[i] * alpha[i])*eigev[i][2];
          }
        }

        for (i = 0; i < 3; i++)
        {
          if (fabs(d1[i]) < EPS12)
          {
            d1[i] = 0.0;
          }
          if (fabs(d2[i]) < EPS12)
          {
            d2[i] = 0.0;
          }
        }

        rhsh0[id1] = dt * d1[0] / dx;
        rhsqx0[id1] = dt * d1[1] / dx;
        rhsqy0[id1] = dt * d1[2] / dx;

        rhsh1[id2] = dt * d2[0] / dx;
        rhsqx1[id2] = dt * d2[1] / dx;
        rhsqy1[id2] = dt * d2[2] / dx;
      }else{
        rhsh0[id1] = 0.0;
        rhsqx0[id1] = 0.0;
        rhsqy0[id1] = 0.0;

        rhsh1[id2] = 0.0;
        rhsqx1[id2] = 0.0;
        rhsqy1[id2] = 0.0;
      }

    });
  }


/** @brief It calculates flux in y direction.
*
*  @param size Array size
*  @param nrows Number of rows in that domain/subdomain
*  @param ncols Number of columns in that domain/subdomain
*  @param dx Cell size
*  @param dt Time step size
*  @param h_arr Water depth array
*  @param qx_arr Discharge in x direction array
*  @param qy_arr Discharge in y direction array
*  @param dem DEM array in that domain/subdomain (elevation)
*  @param sqrth Square root of water depth array
*  @param rhsh0 RHS array to store partial water depth contributions (east-north)
*  @param rhsh1 RHS array to store partial water depth contributions (west-south)
*  @param rhsqx0 RHS array to store partial discharge in x direction contributions (east-north)
*  @param rhsqx1 RHS array to store partial discharge in x direction contributions (west-south)
*  @param rhsqy0 RHS array to store partial discharge in y direction contributions (east-north)
*  @param rhsqy1 RHS array to store partial discharge in y direction contributions (west-south)
*  @param hextra Minimum depth (tolerance below water is at rest)
*/
  template<std::floating_point T>
  void flux_y(int size, int nrows, int ncols, T dx, T dt,
              T const * KOKKOS_RESTRICT h_arr,
              T const * KOKKOS_RESTRICT qx_arr,
              T const * KOKKOS_RESTRICT qy_arr,
              T const * KOKKOS_RESTRICT dem,
              T const * KOKKOS_RESTRICT sqrth,
              T       * KOKKOS_RESTRICT rhsh0,
              T       * KOKKOS_RESTRICT rhsh1,
              T       * KOKKOS_RESTRICT rhsqx0,
              T       * KOKKOS_RESTRICT rhsqx1,
              T       * KOKKOS_RESTRICT rhsqy0,
              T       * KOKKOS_RESTRICT rhsqy1,
              T hextra)
  {

    /**************************************
     *  RHS sketch
     *       _____
     *      |     |
     *      |     |
     *      |__1__|
     *      |  0  |
     *      |     |
     *      |_____|
     *
     * ************************************/

    triton::parallel_for( AUTO_LABEL() , size , KOKKOS_LAMBDA (int id) {

      int ix = (id / ncols);  //row id
      int iy = (id % ncols);  //col id
      int rx = (ix * ncols);  //row offset

      bool
      is_top = (ix <= GHOST_CELL_PADDING-1),
      is_btm = (ix >= nrows - GHOST_CELL_PADDING+1),
      is_lt = (iy <= GHOST_CELL_PADDING-1),
      is_rt = (iy >= ncols - GHOST_CELL_PADDING);


      if (is_top || is_rt || is_lt || is_btm)   //top, right and left cells are not computed. Bottom is neccesary because we need to compute the N edge, which is equal to the south of rows-1. Caution! If domain decomposition changes, this would change


      {
        return;
      }

      int id1, id2;
      T nx, ny;

      id1 = id;
      id2 = rx - ncols + iy;
      nx = 0.0;
      ny = 1.0;

      T h1 = h_arr[id1];
      T h2 = h_arr[id2];

      if (h1 > 0.0 || h2 > 0.0)
      {
        int i;
        T alpha[3], beta[3], eigen[3], eigenE[3]={0.0}, eigev[3][3], d1[3]={0.0}, d2[3]={0.0};
        T u1, u2, v1, v2;

        T qx1 = qx_arr[id1];
        T qy1 = qy_arr[id1];
        T z1 = dem[id1];

        T qx2 = qx_arr[id2];
        T qy2 = qy_arr[id2];
        T z2 = dem[id2];

        if (h1 >= hextra)
        {
          u1 = qx1 / h1;
          v1 = qy1 / h1;
        }
        else
        {
          u1 = 0.0;
          v1 = 0.0;
        }
        if (h2 >= hextra)
        {
          u2 = qx2 / h2;
          v2 = qy2 / h2;
        }
        else
        {
          u2 = 0.0;
          v2 = 0.0;
        }

        T sqh1 = sqrth[id1];
        T sqh2 = sqrth[id2];

        T h12 = 0.5*(h1 + h2);
        T dh = h2 - h1;
        T c12 = sqrt(_G_*h12);
        T u12 = (u1*sqh1 + u2 * sqh2) / (sqh1 + sqh2);
        T v12 = (v1*sqh1 + v2 * sqh2) / (sqh1 + sqh2);
        T un = u12 * nx + v12 * ny;

        eigen[0] = un - c12;
        eigen[1] = un;
        eigen[2] = un + c12;

        //entropy correction

        T e1 = u1 * nx + v1 * ny - SQRTG * sqh1;
        T e2 = u2 * nx + v2 * ny - SQRTG * sqh2;
        if (e1<0.0 && e2>0.0)
        {
          eigenE[0] = eigen[0] - e1 * (e2 - eigen[0]) / (e2 - e1);
          eigen[0] = e1 * (e2 - eigen[0]) / (e2 - e1);
        }

        e1 = u1 * nx + v1 * ny + SQRTG * sqh1;
        e2 = u2 * nx + v2 * ny + SQRTG * sqh2;

        if (e1<0.0 && e2>0.0)
        {
          eigenE[2] = eigen[2] - e2 * (eigen[2] - e1) / (e2 - e1);
          eigen[2] = e2 * (eigen[2] - e1) / (e2 - e1);
        }

        eigev[0][0] = 1.0;
        eigev[0][1] = u12 - c12 * nx;
        eigev[0][2] = v12 - c12 * ny;
        eigev[1][0] = 0.0;
        eigev[1][1] = -c12 * ny;
        eigev[1][2] = c12 * nx;
        eigev[2][0] = 1.0;
        eigev[2][1] = u12 + c12 * nx;
        eigev[2][2] = v12 + c12 * ny;

        alpha[0] = 0.5*(dh - (((qx2 - qx1)*nx + (qy2 - qy1)*ny) - un * dh) / c12);
        alpha[1] = (((qy2 - qy1) - v12 * dh)*nx - ((qx2 - qx1) - u12 * dh)*ny) / c12;
        alpha[2] = 0.5*(dh + (((qx2 - qx1)*nx + (qy2 - qy1)*ny) - un * dh) / c12);

        T dz = z2 - z1;
        T l1 = z1 + h1;
        T l2 = z2 + h2;
        T dzp = dz;
        T hp;

        if (dz >= 0.0)
        {
          hp = h1;
          if (l1 < z2)
          {
            dzp = h1;
          }
        }
        else
        {
          hp = h2;
          if (l2 < z1)
          {
            dzp = -h2;
          }
        }

        beta[0] = 0.5*_G_*(hp - 0.5*fabs(dzp))*dzp / c12;


        //wet-wet correction
        hp = h1 + alpha[0];
        if (eigen[0] * eigen[2]<0.0 && hp>0.0 && h1 > 0.0 && h2 > 0.0)  //subcritical
        {
          beta[0] = FMAX(beta[0], eigen[0]/(eigen[0]+eigenE[2])*(alpha[0] * eigen[0] - h1 * dx*0.5 / dt + eigenE[2]*h2));
          beta[0] = FMIN(beta[0], eigen[2]/(eigen[2]+eigenE[0])*(-alpha[2] * eigen[2] + h2 * dx*0.5 / dt + eigenE[0]*h1));
        }

        beta[1] = 0.0;
        beta[2] = -beta[0];

        l1 = hp - beta[0] / eigen[0]; //left intermediate state
        l2 = hp + beta[2] / eigen[2]; //right intermediate state

        if (l2 < -EPS12 && h2 < EPS12)   //send mass contributions to the left if right cell is dry
        {
          d1[0] += (eigen[0] * alpha[0] - beta[0]) + (eigen[2] * alpha[2] - beta[2]);
        }
        else
        {
          if (l1 < -EPS12 && h1 < EPS12)   //send mass contributions to the right if left cell is dry
          {
            d2[0] += (eigen[0] * alpha[0] - beta[0]) + (eigen[2] * alpha[2] - beta[2]);
          }
          else
          {
            for (i = 0; i < 3; i++)   //regular contributions
            {
              if (eigen[i] > 0.0)
              {
                d2[0] += (eigen[i] * alpha[i] - beta[i])*eigev[i][0];
                d2[1] += (eigen[i] * alpha[i] - beta[i])*eigev[i][1];
                d2[2] += (eigen[i] * alpha[i] - beta[i])*eigev[i][2];
              }
              else
              {
                d1[0] += (eigen[i] * alpha[i] - beta[i])*eigev[i][0];
                d1[1] += (eigen[i] * alpha[i] - beta[i])*eigev[i][1];
                d1[2] += (eigen[i] * alpha[i] - beta[i])*eigev[i][2];
              }
            }
          }

        }

        for (i = 0; i < 3; i++)   //entropy
        {
          //entropy
          if (eigenE[i] > 0.0)
          {
            d2[0] += (eigenE[i] * alpha[i])*eigev[i][0];
            d2[1] += (eigenE[i] * alpha[i])*eigev[i][1];
            d2[2] += (eigenE[i] * alpha[i])*eigev[i][2];
          }
          else
          {
            d1[0] += (eigenE[i] * alpha[i])*eigev[i][0];
            d1[1] += (eigenE[i] * alpha[i])*eigev[i][1];
            d1[2] += (eigenE[i] * alpha[i])*eigev[i][2];
          }
        }

        for (i = 0; i < 3; i++)
        {
          if (fabs(d1[i]) < EPS12)
          {
            d1[i] = 0.0;
          }
          if (fabs(d2[i]) < EPS12)
          {
            d2[i] = 0.0;
          }
        }

        //in the Y-solver flux we have to accumulate over rhs to NOT overwrite the X-flux values
        rhsh0[id1] += dt * d1[0] / dx;
        rhsqx0[id1] += dt * d1[1] / dx;
        rhsqy0[id1] += dt * d1[2] / dx;

        rhsh1[id2] += dt * d2[0] / dx;
        rhsqx1[id2] += dt * d2[1] / dx;
        rhsqy1[id2] += dt * d2[2] / dx;
      }
    });
  }

} // namespace Kernels

#endif // TRITON_KERNELS_FLUX_H
