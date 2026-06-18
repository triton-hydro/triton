/** @file triton.h
 *  @brief Header containing the Triton class
 *
 *  This contains the subroutines and eventually any
 *  macros, constants, etc. needed for Triton class
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


#ifndef TRITON_H
#define TRITON_H
#include <concepts>
#include "kokkos_utils.h"
#include "kernels.h"
#include "output.h"
#include "mpi_utils.h"

#include "Ensify.h"

namespace Triton
{
  template<std::floating_point T>
  class triton  /**< Main class to perform the simulation. */
  {
  public:

/** @brief Constructor.
*
*  @param argc Number of arguments
*  @param argv Arguments
*/
    triton(int argc, char* argv[]);


/** @brief Destructor. Releases any allocated memory.
*
*/
    ~triton();


/** @brief It initializes the simulation.
*
*  @param rank_ Subdomain id
*  @param size_ Number of subdomain
*/
    void initialize(int rank_, int size_);


/** @brief It starts the simulation. It is the main simulation fuction.
*
*/
    void simulate();


  private:
    int rank; /**< Current subdomain id */
    int size; /**< Total sumber of subdomains */
    int rows; /**< Number of rows in current subdomain */
    int cols; /**< Number of columns in current subdomain */
    T xll;	/**< X coordinate of the origin */
    T yll;	/**< Y coordinate of the origin */
    T cellsize;	/**< Size of a cell */
    int org_rows; /**< Number of rows in original domain without ghost cells */
    int org_cols; /**< Number of columns in original domain without ghost cells */
    int num_of_src; /**< Number of flow locations in current subdomain */
    int num_of_obs_points;  /**< Number of observations points in current subdomain */
    int num_of_extbc; /**< Number of external boundary conditions */
    int num_extbc_cells;  /**< Number of cells in all external boundary conditions */
    int index_row_runoff; /**< Index to keep track current runoff row id */
    int idx_low = 0;  /**< Index to keep track lower index id when updating flow locations */
    int checkpoint_id;  /**< Current checkpoint id */

    int host_src_pos_arr_size;  /**< Flow locations position container array size */
    int host_hyg_time_arr_size; /**< Hydrograph time values container array size */
    int host_hyg_val_arr_size;  /**< Hydrograph flow values container array size */
    int host_bc_cells_size; /**< Boundary condition cells container array size */
    int host_bc_vars_arr_size;  /**< Boundary condition variables container array size */
    int host_runoff_intensity_arr_size; /**< Runoff intensity container array size */
    int host_reduce_dt_arr_sz;  /**< Time step size per cell container array size */
    int host_halo_arr_size; /**< Halo cells container array size (water depth and discharges)*/
    int host_obs_cells_size;	/**< Observation cells container array size */
    int nbytes; /**< Subdomain size in bytes */
    int nbytes_halo;  /**< Halo cells bundle size in bytes */
    int nbytes_obs;	/**< Observation cells bundle size in bytes */

    std::vector<int> relative_obs_index;  /**< Relative index position of observation cells per subdomain wrt to the global domain*/

    T simtime;  /**< Current simulation time */
    T cell_size;  /**< Cell size of the grid */
    T local_dt; /**< Time step size in current subdomain */
    T global_dt;  /**< Time step size in the whole domain */
    T average_dt; /**< Average time step size dince the last output */
    T init_dt;  /**< Time step size in case the domain is dry and the time step size is chosen dynamically*/

    std::string project_dir;  /**< Project directory */
    std::string cfg_content;  /**< Content of the input cfg file */
    std::string cfg_dir;  /**< Directory of input cfg file */

    SuperTimer::super_timer st; /**< Time object to keep truct every custom timer */
    ConfigUtils::arguments<T> arglist;  /**< Object that holds all arguments and values from input cfg file */
    Hydrograph::hydrograph<T> hyg;  /**< Object that hold flow locations update data from hydrograph input file */
    Hydrograph::hydrograph<T> roff; /**< Object that hold runoff input data */
    Constants::sources_list_t observation_cells;  /**< Local cell index information of all observation cells */
    Constants::sources_list_t observation_cells_global; /**< Global cell index information of all observation cells */
    MpiUtils::partition_data_t pd;  /**< Partition information of all subdomains */
    MpiUtils::partition_data_t pd_aux;  /**< Auxuliar partition information of all subdomains in case of dynamic decomposition and parallel input */
    DemFile::dem_file<T> dem; /**< Main domain's DEM file information and data */
    DemFile::dem_file<T> sub_dem; /**< Current subdomain's DEM file information and data */
    Matrix::matrix<T> hin;  /**< Main domain's initial depth file data */
    Matrix::matrix<T> uin;  /**< Main domain's initial flux X data */
    Matrix::matrix<T> vin;  /**< Main domain's initial flux Y data */
    Matrix::matrix<T> nin;  /**< Main domain's initial manning data */
    Matrix::matrix<T> hot_hin;  /**< Main domain's water depth checkpoint data */
    Matrix::matrix<T> hot_qxin; /**< Main domain's flux X checkpoint data */
    Matrix::matrix<T> hot_qyin; /**< Main domain's flux Y checkpoint data */
    Matrix::matrix<T> sub_hin;  /**< Current subdomain's water depth data */
    Matrix::matrix<T> sub_qxin; /**< Current subdomain's flux X data */
    Matrix::matrix<T> sub_qyin; /**< Current subdomain's flux Y data */
    Matrix::matrix<T> sub_nin;  /**< Crrent subdomain's manning data */
    Matrix::matrix<T> sub_hot_hin;  /**< Current subdomain's water depth checkpoint data */
    Matrix::matrix<T> sub_hot_qxin; /**< Current subdomain's discharge X checkpoint data */
    Matrix::matrix<T> sub_hot_qyin; /**< Current subdomain's discharge Y checkpoint data */
    Matrix::matrix<T> sub_max_value_h;  /**< Current subdomain's max water depth of each cell */
    Matrix::matrix<T> hot_max_value_h;  /**< Main domain's max water depth checkpoint data */

    Matrix::matrix<int> rin;  /**< Main domain's runoff data */
    Matrix::matrix<int> sub_rin;  /**< Current subdomain's runoff data */

    int* host_src_pos_arr;  /**< Current subdomains flow locations position array */
    int* host_relative_bc_index;  /**< Boundary condition cells relative positions in current subdomain */
    int* host_bc_type;  /**< Boundary condition types of each boundary cells */
    int* host_bc_start_index; /**< Separate boundary condition start indexes */
    int* host_bc_nrows_vars;  /**< Boundary condition variable of every rows */
    int* host_runoff_id_arr;  /**< Array that contains runoff ids */
    int* host_relative_obs_index;	/**< Observation data relative positions in current subdomain */

    T* host_hyg_time_arr; /**< Hydrograph time values container array */
    T* host_hyg_val_arr;  /**< Hydrograph flow values container array */
    T* host_extbc_var1_arr; /**< Boundary condition variables container array */
    T* host_extbc_var2_arr; /**< Boundary condition variables container array*/
    T* host_runoff_intensity_arr; /**< Runoff intensity container array*/
    T* host_halo_arr; /**< Halo cells container array*/
    T* host_sqrth_arr;  /**< Square root value array of every cells water depth */
    T* host_dt_values_arr;  /**< Time step size array of every cells */
    T* host_rhsh0;  /**< Intermediate raster array to hold partial water depth */
    T* host_rhsh1;  /**< Intermediate raster array to hold partial water depth */
    T* host_rhsqx0; /**< Intermediate raster array to hold partial flux X */
    T* host_rhsqx1; /**< Intermediate raster array to hold partial flux X */
    T* host_rhsqy0; /**< Intermediate raster array to hold partial flux Y */
    T* host_rhsqy1; /**< Intermediate raster array to hold partial flux Y */
    T* host_obs_h;	/**< Observation point water depth values */
    T* host_obs_qx;	/**< Observation point flux X values */
    T* host_obs_qy;	/**< Observation point flux Y values */

    std::vector<T*> host_vec; /**< Vector that contains all floating point array to use in simulation. */
    std::vector<int*> host_vec_int; /**< Vector that contains all integer array to use in simulation. */

    Output::output<T> out; /**Object to manage output files. */

    gpuStream_t streams;  /**< Cuda stream */
    std::vector<T*> device_vec; /**< Device vector that contains all floating point array to use in simulation. */
    std::vector<int*> device_vec_int; /**< Device vector that contains all integer array to use in simulation. */

/** @brief This function is used to compute the time step size in the case the domain is dry and the dynamic CFL condition is set. It is computed according to the hydrograph interval, runoff interval and print interval divided by 100.
*
*/
    void compute_init_dt();



/** @brief This function is used to calculate minimum time step size for each sub domain.
*
*/
    void compute_local_dt();


/** @brief This function is used to calculate minimum time step size between all sub domain.
*
*  @param print_id Current checkpoint id
*/
    void compute_global_dt(int print_id);


/** @brief This function is used to compute next state. All main computation is done inside this function.
*
*/
    void compute_new_state();

/** @brief This function is used to get observation data to host.
*
*/
		void get_observation_data_to_host();


/** @brief It calculates a cell's column index.
*
*  @param src_x X location
*  @param xllc X coordinate of the origin
*  @param cell_size_ Cell size
*  @return The corresponding value
*/
    int calc_src_col(T src_x, T xllc, T cell_size_);


/** @brief It calculates a cell's row index.
*
*  @param src_y Y location
*  @param yllc Y coordinate of the origin
*  @param cell_size_ Cell size
*  @param nrows Number of rows
*  @return The corresponding value
*/
    int calc_src_row(T src_y, T yllc, T cell_size_, int nrows);


/** @brief This function reads from configuration file, process it for further use.
*
*  @param cfg_dir Cfg file directory
*  @param checkpoint_id Current checkpoint id
*/
    void read_configuration(std::string cfg_dir, int checkpoint_id);


/** @brief This function reads all flow locations information.
*
*/
    void read_inflows();


/** @brief This function reads the header of the dem files
*
*/
    void read_header_dem_files();


/** @brief This function reads all matrix type data file in sequential mode.
*
*/
    void read_matrix_files_sequential();

/** @brief This function reads all matrix type data file in parallel mode.
*
*/
    void read_matrix_files_parallel();


/** @brief This function processes all flow locations and partition them in different subdomain.
*
*/
    void process_source_locations();


/** @brief This function reads all observation cells information.
*
*/
    void process_observation_cells();


/** @brief This function reads all boundary conditions and allow different subdomain capability.
*
*/
    void process_boundary_condition();


/** @brief This function processes all the matrix file and partition them into different sub domain.
*
*/
    void partition_matrix_files();


/** @brief This function processes runoff if available.
*
*/
    void process_runoff();


/** @brief This function creates additional arrays needed for simulation.
*
*/
    void create_host_aux_vectors();


/** @brief This function creates host vector for simulation combining all needed arrays.
*
*/
    void create_host_vectors();


/** @brief This function creates device vector for simulation combining all needed arrays.
*
*/
    void create_device_vectors();


/** @brief This function creates new load balance domain decomposition.
*
*/
    void new_domain_decomposition();

    /** @brief This function gathers all the information from dynamic partition, creates the new partition based on previous MPI times and broadcasts the information from process 0 to the rest of the processes.
*
*/
    int MPI_time_based_domain_decomposition();

    /** @brief This function resets previous arrays.
*
*/
    void reset_arrays();


/** @brief This function processes all the matrix file and partition them into different sub domain for dynamic balancing
*
*/
    void partition_matrix_files_dynamic();



  };

} // namespace Triton

#include "triton_init.h"
#include "triton_decomp.h"
#include "triton_simulate.h"

#endif
