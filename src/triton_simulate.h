// Extracted from src/triton.h — Stage 3 header split.
// Contains simulation-loop methods of Triton::triton<T>.

#ifndef TRITON_TRITON_SIMULATE_H
#define TRITON_TRITON_SIMULATE_H

#include "triton.h"

namespace Triton {

  template<std::floating_point T>
  void triton<T>::simulate()
  {
    compute_init_dt();

    if(rank==0) [[unlikely]] {
      std::cerr << OK "Simulation starts" << std::endl;
    }

    MPI_Barrier(ENSIFY_COMM_WORLD);


    st.start(SIMULATION_TIME);


    T xll = dem.get_xll_corner();
    T yll = dem.get_yll_corner();
    T cellsize = dem.get_cell_size();

    out.init(rows, cols, xll, yll, cellsize, rank, size, project_dir, arglist.output_folder, arglist.outfile_pattern, arglist.time_series_flag, cfg_content, arglist.output_option);

    if (arglist.time_series_flag)
    {
	out.init_time_series(num_of_obs_points, arglist.observation_x_loc.size(), relative_obs_index, observation_cells, observation_cells_global, arglist.print_option, arglist.checkpoint_id);
	//first data written at the beginning of the simulation
	get_observation_data_to_host();
	out.write_observation_data(host_obs_h, host_obs_qx, host_obs_qy, simtime, arglist.print_option);
    }


    global_dt = arglist.time_step;
    average_dt = 0.0;
    local_dt = arglist.time_step;
    int it_count = arglist.it_count;
    int it_count_average = 0;
    int print_id = arglist.checkpoint_id;
    T last_print_obs_time = simtime; //in case there is no hotstart, simtime is zero

    //this is to allow simtime different from zero without checkpointing
    if(print_id==0 && simtime>0.0){
      print_id=simtime/arglist.print_interval;
    }

    while (simtime < arglist.sim_duration)
    {
      it_count++;
      it_count_average++;

      if (!arglist.time_increment_fixed)
      {
        compute_local_dt();
        compute_global_dt(print_id);
      }
      compute_new_state();

      simtime += global_dt;
      average_dt+=global_dt;

	if(it_count%arglist.it_print == 0){
		if(rank==0) [[unlikely]] {
			std::cerr << INFO " Time: " << simtime << "\tdt: " << average_dt/it_count_average << "\tit: " << it_count << std::endl;
			it_count_average=0;
			average_dt=0.0;
		}
	}


	if (arglist.time_series_flag && simtime - last_print_obs_time >= arglist.print_observation) {
    	// variable to print observation data files
    	last_print_obs_time += arglist.print_observation;
		//transfer only the observation data points to the host
		st.start(COMPUTE_TIME);
		get_observation_data_to_host();
		st.stop(COMPUTE_TIME);

		st.start(IO_TIME);
		out.write_observation_data(host_obs_h, host_obs_qx, host_obs_qy, simtime, arglist.print_option);
		//out.write_observation_data(host_vec[OBSH], host_vec[OBSQX], host_vec[OBSQY], simtime, arglist.print_option);
		st.stop(IO_TIME);
	}

      if (simtime >= arglist.print_interval * (print_id + 1))
      {
        print_id++;

        st.start(COMPUTE_TIME);
        gpuMemcpyAsync(host_vec[H], device_vec[H], nbytes, gpuMemcpyDeviceToHost, streams);
        gpuMemcpyAsync(host_vec[QX], device_vec[QX], nbytes, gpuMemcpyDeviceToHost, streams);
        gpuMemcpyAsync(host_vec[QY], device_vec[QY], nbytes, gpuMemcpyDeviceToHost, streams);
        if (arglist.max_value_print_option.size() > 0)
        {
          gpuMemcpyAsync(host_vec[MAXH], device_vec[MAXH], nbytes, gpuMemcpyDeviceToHost, streams);
        }
        gpuStreamSynchronize(streams);
        st.stop(COMPUTE_TIME);

        st.start(IO_TIME);
	out.write_output(sub_hin, sub_qxin, sub_qyin, arglist.output_format, arglist.projection, arglist.print_option, print_id, it_count, simtime, average_dt, sub_max_value_h, arglist.max_value_print_option);
        st.stop(IO_TIME);

        #if WRITE_PERFORMANCE
          st.stop(SIMULATION_TIME);
          st.stop(TOTAL_TIME);
          out.write_times(st, print_id);
          st.start(SIMULATION_TIME);
          st.start(TOTAL_TIME);
        #endif

        //mandatory in case of hotstart
        if(strcmp(arglist.domain_decomposition.c_str(), TYPE_DYNAMIC)==0 && size > 1){
          out.write_domain_decomposition(pd,print_id);
        }

        //we want to avoid repartitioning in the last iteration when simtime=arglist.sim_duration
        if(strcmp(arglist.domain_decomposition.c_str(), TYPE_DYNAMIC)==0 && print_id%arglist.factor_interval_domain_decomposition==0 && size > 1 && simtime < arglist.sim_duration)
        {
          st.start(RESIZE_TIME);
          new_domain_decomposition();
          st.stop(RESIZE_TIME);
        }

      }

    }
    //write the last state in the observation data files
    if (arglist.time_series_flag) {
	//transfer only the observation data points to the host
	st.start(COMPUTE_TIME);
	get_observation_data_to_host();
	st.stop(COMPUTE_TIME);

	st.start(IO_TIME);
	out.write_observation_data(host_obs_h, host_obs_qx, host_obs_qy, simtime, arglist.print_option);
	st.stop(IO_TIME);
    }


    st.stop(SIMULATION_TIME);
    st.stop(TOTAL_TIME);

    out.write_times(st, -1);

    if(rank==0) [[unlikely]] {
      std::cerr << INFO " Time: " << simtime << "\tdt: " << average_dt/it_count_average << "\tit: " << it_count << std::endl;
      std::cerr << OK "Simulation ends" << std::endl;
    }


  }


  template<std::floating_point T>
  void triton<T>::compute_init_dt()
  {
    T global_init_dt;

  	 if(arglist.time_series_flag){
    	init_dt=FMIN(arglist.print_observation,arglist.print_interval);
    }else{
		init_dt=arglist.print_interval;
	 }
	 if (arglist.num_runoffs > 0){
      init_dt=fmin(init_dt,roff.get_time_at(1)-roff.get_time_at(0));
    }
    if(num_of_src>0){
      init_dt=fmin(init_dt,hyg.get_time_at(1)-hyg.get_time_at(0));
    }
    init_dt*=0.01;

    if(size > 1){
      st.start(MPI_TIME);
      MPI_Allreduce(&init_dt, &global_init_dt, 1, MPI_DATA_TYPE, MPI_MIN, MPI_COMM_WORLD);
      st.stop(MPI_TIME);
      init_dt=global_init_dt;
    }

  }


  template<std::floating_point T>
  void triton<T>::compute_local_dt()
  {
    st.start(COMPUTE_TIME);
    Kernels::compute_dt_and_sqrt(rows*cols, cell_size, device_vec[QX], device_vec[QY], device_vec[H], device_vec[SQRTH], device_vec[DT], arglist.courant, arglist.hextra);
    local_dt = Kernels::find_min_dt(rows*cols, device_vec[DT]);
    gpuStreamSynchronize(streams);
    st.stop(COMPUTE_TIME);
  }


  template<std::floating_point T>
  void triton<T>::compute_global_dt(int print_id)
  {
    if (size > 1)
    {
      st.start(BALANCING_MPI_TIME);
      st.start(MPI_TIME);
      MPI_Allreduce(&local_dt, &global_dt, 1, MPI_DATA_TYPE, MPI_MIN, ENSIFY_COMM_WORLD);
      st.stop(MPI_TIME);
      st.stop(BALANCING_MPI_TIME);
    }
    else
    {
      global_dt = local_dt;
    }

    if (global_dt >= MAX_VALUE - 1.0) [[unlikely]]
    {
      global_dt = init_dt;
    }

    if (simtime + global_dt > arglist.print_interval * (print_id + 1))
    {
      global_dt = arglist.print_interval * (print_id + 1) - simtime;
    }
  }


  template<std::floating_point T>
  void triton<T>::compute_new_state()
  {
    st.start(COMPUTE_TIME);

    Kernels::flux_x(rows*cols, rows, cols, cell_size, global_dt,
    device_vec[H], device_vec[QX], device_vec[QY], device_vec[DEM], device_vec[SQRTH],
    device_vec[RHSH0], device_vec[RHSH1], device_vec[RHSQX0], device_vec[RHSQX1], device_vec[RHSQY0], device_vec[RHSQY1], arglist.hextra);

    Kernels::flux_y(rows*cols, rows, cols, cell_size, global_dt,
    device_vec[H], device_vec[QX], device_vec[QY], device_vec[DEM], device_vec[SQRTH],
    device_vec[RHSH0], device_vec[RHSH1], device_vec[RHSQX0], device_vec[RHSQX1], device_vec[RHSQY0], device_vec[RHSQY1], arglist.hextra);

    Kernels::update_cells(rows*cols, rows, cols, global_dt,
    device_vec[H], device_vec[QX], device_vec[QY], device_vec[DEM], device_vec[NMAN],
    device_vec[RHSH0], device_vec[RHSH1], device_vec[RHSQX0], device_vec[RHSQX1], device_vec[RHSQY0], device_vec[RHSQY1], arglist.hextra);

    if (arglist.num_runoffs > 0)
    {
      if(simtime > roff.get_time_at(roff.get_num_inflow_rows()-1))
      {
        index_row_runoff=roff.get_num_inflow_rows()-1;
      }
      else
      {
        if (simtime > roff.get_time_at(index_row_runoff + 1))
        {
          index_row_runoff++;
        }
      }

      Kernels::update_runoff(rows*cols, rows, cols, global_dt,
      device_vec_int[RUNID], index_row_runoff, roff.get_num_inflow_rows(), device_vec[RUNIN], device_vec[H], device_vec[QX], device_vec[QY], arglist.hextra);
    }

    if (num_of_src > 0)
    {
      bool check_flag = false;
      int idx_high = hyg.get_num_inflow_rows() - 1;
      for (int i = idx_low; i < idx_high; i++)
      {
        if (hyg.get_time_at(i + 1) > simtime)
        {
          idx_low = i;
          idx_high = i + 1;
          check_flag = true;
          break;
        }
      }
      if (!check_flag)
      {
        idx_low = idx_high;
      }

      Kernels::compute_flow(num_of_src, device_vec[HYGT], device_vec[HYGV],
      cell_size, global_dt, simtime, idx_low, idx_high, device_vec[H], device_vec_int[SRCP]);
    }


    if(arglist.open_boundaries){

        Kernels::copy_info_to_exterior_boundaries_west_east(2*rows*GHOST_CELL_PADDING, rows, cols, device_vec[H], device_vec[QX], device_vec[QY]);
        Kernels::copy_info_to_exterior_boundaries_north_south(cols*GHOST_CELL_PADDING, rows, cols, device_vec[H], device_vec[QX], device_vec[QY],rank, size);

    }

    if (num_of_extbc > 0 && num_extbc_cells > 0)
    {
      Kernels::compute_extbc_values(num_extbc_cells, rows, cols, global_dt, device_vec[H], device_vec[QX], device_vec[QY], device_vec[DEM], device_vec[NMAN], device_vec_int[BCRELATIVEINDEX], device_vec_int[BCTYPE], device_vec_int[BCINDEXSTART], device_vec_int[BCNROWSVARS], device_vec[EXTBCV1], device_vec[EXTBCV2], simtime, rank, size);

    }

    Kernels::wet_dry(rows*cols, rows, cols, global_dt, device_vec[H], device_vec[QX], device_vec[QY], device_vec[DEM], device_vec[MAXH], arglist.hextra,size);



    if (size > 1)
    {
      Kernels::halo_copy_from_gpu(2 * cols*GHOST_CELL_PADDING, rows, cols, device_vec[H], device_vec[QX], device_vec[QY], device_vec[HALO]);

      if (arglist.gpu_direct_flag)
      {
        gpuStreamSynchronize(streams);
        st.stop(COMPUTE_TIME);

        st.start(BALANCING_MPI_TIME);
        st.start(MPI_TIME);
        MpiUtils::exchange(device_vec[HALO], 12*GHOST_CELL_PADDING, cols, rank, size, USE_HALO);
        st.stop(MPI_TIME);
        st.stop(BALANCING_MPI_TIME);

        st.start(COMPUTE_TIME);
      }
      else
      {
        gpuMemcpyAsync(host_vec[HALO], device_vec[HALO], nbytes_halo, gpuMemcpyDeviceToHost, streams);
        gpuStreamSynchronize(streams);
        st.stop(COMPUTE_TIME);

        st.start(BALANCING_MPI_TIME);
        st.start(MPI_TIME);
        MpiUtils::exchange(host_vec[HALO], 12*GHOST_CELL_PADDING, cols, rank, size, USE_HALO);
        st.stop(MPI_TIME);
        st.stop(BALANCING_MPI_TIME);

        st.start(COMPUTE_TIME);
        gpuMemcpyAsync(device_vec[HALO], host_vec[HALO], nbytes_halo, gpuMemcpyHostToDevice, streams);
      }

      Kernels::halo_copy_to_gpu(2 * cols*GHOST_CELL_PADDING, rows, cols, device_vec[H], device_vec[QX], device_vec[QY], device_vec[HALO]);

      Kernels::wet_dry_qy_halo(2 * cols*GHOST_CELL_PADDING, rows, cols, device_vec[H], device_vec[QY], device_vec[DEM], arglist.hextra);

    }


    st.stop(COMPUTE_TIME);
  }

	template<std::floating_point T>
	void triton<T>::get_observation_data_to_host()
	{
		if (arglist.print_option.find("h") != std::string::npos)
		{
			Kernels::copy_observation_points (num_of_obs_points, device_vec[H], device_vec[OBSH], device_vec_int[OBSRELATIVEINDEX]);
			gpuMemcpyAsync(host_vec[OBSH], device_vec[OBSH], nbytes_obs, gpuMemcpyDeviceToHost, streams);
			gpuStreamSynchronize(streams);
		}
		if (arglist.print_option.find("u") != std::string::npos)
		{
			Kernels::copy_observation_points (num_of_obs_points, device_vec[QX], device_vec[OBSQX], device_vec_int[OBSRELATIVEINDEX]);
			gpuMemcpyAsync(host_vec[OBSQX], device_vec[OBSQX], nbytes_obs, gpuMemcpyDeviceToHost, streams);
			gpuStreamSynchronize(streams);
		}
		if (arglist.print_option.find("v") != std::string::npos)
		{
			Kernels::copy_observation_points(num_of_obs_points, device_vec[QY], device_vec[OBSQY], device_vec_int[OBSRELATIVEINDEX]);
			gpuMemcpyAsync(host_vec[OBSQY], device_vec[OBSQY], nbytes_obs, gpuMemcpyDeviceToHost, streams);
			gpuStreamSynchronize(streams);
		}
	}

} // namespace Triton

#endif // TRITON_TRITON_SIMULATE_H
