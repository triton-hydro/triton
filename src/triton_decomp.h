// Extracted from src/triton.h — Stage 3 header split.
// Contains domain-decomposition methods of Triton::triton<T>.

#ifndef TRITON_TRITON_DECOMP_H
#define TRITON_TRITON_DECOMP_H

#include "triton.h"

namespace Triton {

  template<std::floating_point T>
  void triton<T>::new_domain_decomposition()
  {

    if(MPI_time_based_domain_decomposition()){;

      reset_arrays();

      process_source_locations();

      process_observation_cells();

      process_boundary_condition();

      partition_matrix_files_dynamic();

      process_runoff();
      create_host_aux_vectors();

      create_host_vectors();
      create_device_vectors();

      //a call to out.init is again neccessary to set the output configuration
	T xll = dem.get_xll_corner();
	T yll = dem.get_yll_corner();
	T cellsize = dem.get_cell_size();
	out.init(rows, cols, xll, yll, cellsize, rank, size, project_dir, arglist.output_folder, arglist.outfile_pattern, arglist.time_series_flag, cfg_content, arglist.output_option);

      if (arglist.time_series_flag)
      {
	out.init_time_series(num_of_obs_points, arglist.observation_x_loc.size(), relative_obs_index, observation_cells, observation_cells_global, arglist.print_option, arglist.checkpoint_id);

      }


    }


  }

  template<std::floating_point T>
  int triton<T>::MPI_time_based_domain_decomposition()
  {
    int *dyn_rows = new int[size];
    T *mpi_time_all = new T[size];
    T sumMPI;
    T mpi_time = st.get_custom_time(BALANCING_MPI_TIME);
    int sum_rows;
    int flag=0;


    MPI_Gather(&mpi_time, 1, MPI_DATA_TYPE, &mpi_time_all[rank], 1, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);

    if(rank==0){
      sumMPI=0.0;
      for(int j=0;j<size;j++){
        sumMPI+=mpi_time_all[j];
      }

      sum_rows=0;
      for(int j=0;j<size;j++){
	T factor = mpi_time_all[j]*size/sumMPI - 1.0;
        if(fabs(factor)>0.05){ //greater than 5%
          flag=1;
        }
        //0.1 is a weight factor to enforce gradual changes in domain size from one step to another
        dyn_rows[j]=pd.part_dims[j].first + (int) floor((factor)*pd.rows/size*0.1);
        //at least 2 real rows per subdomain
        dyn_rows[j]=std::max(dyn_rows[j],2+2*GHOST_CELL_PADDING);
        sum_rows+=dyn_rows[j]-2*GHOST_CELL_PADDING;
      }

      if(sum_rows<=pd.rows){
        int rem = pd.rows - sum_rows;
        if(rem>0){
          for(int j=0;j<rem;j++){
            dyn_rows[j]++;
          }
        }
      }else{
        std::cerr << ERROR "Re-partitioning algorithm is wrong" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
    MPI_Barrier(ENSIFY_COMM_WORLD);

    MPI_Bcast(&flag, 1, MPI_INT, 0, ENSIFY_COMM_WORLD);

    if(flag==0){
      st.restart(BALANCING_MPI_TIME);
      return 0;
    }

    MPI_Bcast(dyn_rows, size, MPI_INT, 0, ENSIFY_COMM_WORLD);


    for(int i=0;i<pd.size;i++){
      pd.part_dims[i].first=dyn_rows[i];
    }

    st.restart(BALANCING_MPI_TIME);

    delete[] dyn_rows;
    delete[] mpi_time_all;


    return 1;

  }


  template<std::floating_point T>
  void triton<T>::reset_arrays()
  {

    delete[] host_vec[EXTBCV2];
    delete[] host_vec[EXTBCV1];
    delete[] host_vec[RUNIN];
    delete[] host_vec[HYGV];
    delete[] host_vec[HYGT];
    delete[] host_vec[DT];
    delete[] host_vec[HALO];
    delete[] host_vec[SQRTH];
    delete[] host_vec[RHSQY1];
    delete[] host_vec[RHSQY0];
    delete[] host_vec[RHSQX1];
    delete[] host_vec[RHSQX0];
    delete[] host_vec[RHSH1];
    delete[] host_vec[RHSH0];

    delete[] host_vec_int[BCNROWSVARS];
    delete[] host_vec_int[BCINDEXSTART];
    delete[] host_vec_int[BCTYPE];
    delete[] host_vec_int[BCRELATIVEINDEX];
    delete[] host_vec_int[RUNID];
    delete[] host_vec_int[SRCP];

    //not neccessary since we are inside output so we already copied this data to the CPU
    /*gpuMemcpyAsync(host_vec[H], device_vec[H], nbytes, gpuMemcpyDeviceToHost, streams);
    gpuMemcpyAsync(host_vec[QX], device_vec[QX], nbytes, gpuMemcpyDeviceToHost, streams);
    gpuMemcpyAsync(host_vec[QY], device_vec[QY], nbytes, gpuMemcpyDeviceToHost, streams);
    if (arglist.max_value_print_option.size() > 0)
    {
      gpuMemcpyAsync(host_vec[MAXH], device_vec[MAXH], nbytes, gpuMemcpyDeviceToHost, streams);
    }
    gpuStreamSynchronize(streams);*/

    gpuStreamDestroy(streams);
    while (!device_vec.empty())
    {
      gpuFree(device_vec.back());
      device_vec.pop_back();
    }
    while (!device_vec_int.empty())
    {
      gpuFree(device_vec_int.back());
      device_vec_int.pop_back();
    }

  }


  template<std::floating_point T>
  void triton<T>::partition_matrix_files_dynamic()
  {
    if(strcmp(arglist.input_option.c_str(), "SEQ")==0){ //sequential
      sub_dem.resize(1,1);
      sub_nin.resize(1,1);

      sub_dem = MpiUtils::scatter_exchange(dem.get_data(), pd, rank);
      sub_nin = MpiUtils::scatter_exchange(nin.get_data(), pd, rank);

      if(arglist.runoff_map.size() > 0)
      {
        sub_rin = MpiUtils::scatter_exchange_int(rin.get_data(), pd, rank);
      }

    }else{
      //gather dem
      if (rank == 0)
      {
        MPI_Gatherv(sub_dem.get_address_at(0, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
      }
      else
      {
        MPI_Gatherv(sub_dem.get_address_at(GHOST_CELL_PADDING, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
      }
      sub_dem.resize(1,1);
      sub_dem = MpiUtils::scatter_exchange(out.total_data_arr, pd, rank);

      //gather nin
      if (rank == 0)
      {
        MPI_Gatherv(sub_nin.get_address_at(0, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
      }
      else
      {
        MPI_Gatherv(sub_nin.get_address_at(GHOST_CELL_PADDING, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
      }
      sub_nin.resize(1,1);
      sub_nin = MpiUtils::scatter_exchange(out.total_data_arr, pd, rank);

      if(arglist.runoff_map.size() > 0)
      {
        //gather rmap
        if (rank == 0)
        {
          MPI_Gatherv(sub_rin.get_address_at(0, 0), out.cur_proc_data_size, MPI_INTEGER, out.total_data_arr_int, out.recvcounts, out.displs, MPI_INTEGER, 0, ENSIFY_COMM_WORLD);
        }
        else
        {
          MPI_Gatherv(sub_rin.get_address_at(GHOST_CELL_PADDING, 0), out.cur_proc_data_size, MPI_INTEGER, out.total_data_arr_int, out.recvcounts, out.displs, MPI_INTEGER, 0, ENSIFY_COMM_WORLD);
        }
        sub_rin.resize(1,1);
        sub_rin = MpiUtils::scatter_exchange_int(out.total_data_arr_int, pd, rank);
      }


    }

    rows = sub_dem.get_num_rows();
    cols = sub_dem.get_num_cols();

    sub_dem.set_nrows(sub_dem.get_num_rows());
    sub_dem.set_ncols(sub_dem.get_num_cols());
    sub_dem.set_cell_size(dem.get_cell_size());
    sub_dem.set_xll_corner(dem.get_xll_corner());
    sub_dem.set_yll_corner(dem.get_yll_corner());
    sub_dem.set_no_data_value(dem.get_no_data_value());


    //gather H
    if (rank == 0)
    {
      MPI_Gatherv(sub_hin.get_address_at(0, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
    }
    else
    {
      MPI_Gatherv(sub_hin.get_address_at(GHOST_CELL_PADDING, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
    }

    sub_hin.resize(1,1);
    sub_hin = MpiUtils::scatter_exchange(out.total_data_arr, pd, rank);


    //gather QX
    if (rank == 0)
    {
      MPI_Gatherv(sub_qxin.get_address_at(0, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
    }
    else
    {
      MPI_Gatherv(sub_qxin.get_address_at(GHOST_CELL_PADDING, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
    }
    sub_qxin.resize(1,1);
    sub_qxin = MpiUtils::scatter_exchange(out.total_data_arr, pd, rank);

    //gather QY
    if (rank == 0)
    {
      MPI_Gatherv(sub_qyin.get_address_at(0, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
    }
    else
    {
      MPI_Gatherv(sub_qyin.get_address_at(GHOST_CELL_PADDING, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
    }
    sub_qyin.resize(1,1);
    sub_qyin = MpiUtils::scatter_exchange(out.total_data_arr, pd, rank);


    //gather MAXH
    if (rank == 0)
    {
      MPI_Gatherv(sub_max_value_h.get_address_at(0, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
    }
    else
    {
      MPI_Gatherv(sub_max_value_h.get_address_at(GHOST_CELL_PADDING, 0), out.cur_proc_data_size, MPI_DATA_TYPE, out.total_data_arr, out.recvcounts, out.displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
    }
    sub_max_value_h.resize(1,1);
    sub_max_value_h = MpiUtils::scatter_exchange(out.total_data_arr, pd, rank);

    if(rank == 0){
      std::cerr << OK "Data has been re-partitioned" << std::endl;
    }

  }

} // namespace Triton

#endif // TRITON_TRITON_DECOMP_H
