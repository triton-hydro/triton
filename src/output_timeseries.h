// Extracted from src/output.h — Stage 3 header split.
// Contains output_time_series and write_observation_data.

#ifndef TRITON_OUTPUT_TIMESERIES_H
#define TRITON_OUTPUT_TIMESERIES_H

#include <sstream>
#include "output.h"

namespace Output {

	template<typename T>
	void output<T>::output_time_series(T *value_obs, std::string what_mat, T simtime)
	{
		std::string outdir = project_dir_ + "/" + output_folder_ + "/" + TIME_SERIES_DIR + "/";
		std::string filedir = outdir + what_mat + "_at_Xsec.txt";

		T* value_obs_global = NULL;

		if(rank_ == 0){
			value_obs_global = (T*)malloc(num_of_obs_points_global_ * sizeof(T));
		}

    	MPI_Gatherv(value_obs, num_of_obs_points_, MPI_DATA_TYPE, value_obs_global, obs_points_per_subdomain, displs_time_series, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);

		if(rank_ == 0){
			std::string str = std::to_string(simtime);
			std::ofstream out(filedir, std::ios::app);
			for (int i = 0; i < num_of_obs_points_global_; i++)
			{
				str = str + "," + std::to_string(value_obs_global[time_series_index_relative_[i]]);
			}
			str = str + "\n";
			out << str;
			out.close();
		}

		if(rank_ == 0){
			free(value_obs_global);
		}

		if (size_ > 1)
		{
			MPI_Barrier(ENSIFY_COMM_WORLD);
		}

	}

	template<typename T>
	void output<T>::write_observation_data( T *h_arr_obs, T *qx_arr_obs, T *qy_arr_obs, T simtime, std::string print_option)
	{
		if (print_option.find("h") != std::string::npos)
		{
			output_time_series(h_arr_obs, "H", simtime);
		}
		if (print_option.find("u") != std::string::npos)
		{
			output_time_series(qx_arr_obs, "QX", simtime);
		}
		if (print_option.find("v") != std::string::npos)
		{
			output_time_series(qy_arr_obs, "QY", simtime);
		}
		if (rank_ == 0)
		{
			{
				std::ostringstream oss;
				oss << BLUE << "[OBS]" << RESET << " Observation data written at time " << simtime << " seconds.\n";
				std::cerr << oss.str();
			}
		}


	}

} // namespace Output

#endif // TRITON_OUTPUT_TIMESERIES_H
