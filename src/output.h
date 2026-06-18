/** @file output.h
 *  @brief Header containing the Output class
 *
 *  This contains the subroutines and eventually any
 *  macros, constants, etc. needed for Output class
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


#ifndef OUTPUT_H
#define OUTPUT_H

#include <sstream>
#include <iomanip>
#include "constants.h"
#include "matrix.h"

#ifdef TRITON_GDAL
#include <gdal.h>  
#include <gdal_priv.h>  
#include <ogr_spatialref.h>  
#include <ogr_geometry.h>  
#include <cpl_conv.h> // For geotiff output
#endif

#include "Ensify.h"

namespace Output
{
	template<class T>
	class output	/**< Ths class handles all data outputs in file. */
	{
	public:
		
/** @brief Constructor.
*
*/	
		output() {};


/** @brief Destructor. Releases any allocated memory.
*
*/
		~output();
		
		
/** @brief It initializes anything related to outputs in file.
*
*  @param rows Rows in subdomain
*  @param cols Columns in subdomain
*  @param xll X coordinate of the origin
*  @param yll Y coordinate of the origin
*  @param cellsize Cell size
*  @param rank Current sub domain id
*  @param size Number of sub domains
*  @param project_dir Main project directory
*  @param outfile_pattern Output file name pattern
*  @param time_series_flag Flag to output time series or not
*  @param cfg_content Contents of input cfg file
*  @param output_option Determines how to write data
*/		
		void init(int rows, int cols, T xll, T yll, T cellsize, int rank, int size, std::string project_dir, std::string output_folder, std::string outfile_pattern, int time_series_flag, std::string cfg_content, std::string output_option);
		
		
/** @brief It initializes time series outputs in a file.
*
*  @param num_of_obs_points number of local observation points
*  @param num_of_obs_points_global number of global observation points
*  @param relative_obs_index Relative array of indexes of local observation cells for the global array
*  @param observation_cells Local cell index
*  @param observation_cells_global Global cell index
*  @param print_option Which data to write
*  @param checkpoint_id Current checkpoint id
*/			
		void init_time_series(int num_of_obs_points, int num_of_obs_points_global, std::vector<int> relative_obs_index, Constants::sources_list_t observation_cells, Constants::sources_list_t observation_cells_global, std::string print_option, int checkpoint_id);
		
		
/** @brief It calculates which data to output in file. Also prints checkpoint id.
*
*  @param h_arr Water depth data
*  @param qx_arr Discharge in x direction data
*  @param qy_arr Discharge in y direction data
*  @param output_format Format of output files
*  @param projection Projection system
*  @param print_option Which data to write
*  @param print_id Current checkpoint id
*  @param it_count Number of iterations so far
*  @param max_value_h Max value of water depth data
*  @param max_value_print_option Which max value data to write
*/		
		void write_output(Matrix::matrix<T>& h_arr, Matrix::matrix<T>& qx_arr, Matrix::matrix<T>& qy_arr, std::string output_format, std::string projection, std::string print_option, int print_id, int it_count, T simtime, T average_dt, Matrix::matrix<T>& max_value_h, std::string max_value_print_option);
		
		
/** @brief It outputs a specific data array's full domain in a single ascii file. 
*
*  @param arr Subdomain data
*  @param what_mat Data type
*  @param print_id Current checkpoint id
*/		
		void write_output_ascii_sequential(Matrix::matrix<T>& arr, std::string what_mat, int print_id);
		
		
/** @brief It outputs a specific data array's sub domain in a ascii file. All subdomain outputs seperately in different file.
*
*  @param arr Subdomain data
*  @param what_mat Data type
*  @param print_id Current checkpoint id
*/			
		void write_output_ascii_parallel(Matrix::matrix<T>& arr, std::string what_mat, int print_id);
		
		
/** @brief It outputs a specific data array's full domain in a single binary file. 
*
*  @param arr Subdomain data
*  @param what_mat Data type
*  @param print_id Current checkpoint id
*/		
		void write_output_binary_sequential(Matrix::matrix<T>& arr, std::string what_mat, int print_id);
		
		
/** @brief It outputs a specific data array's sub domain in a binary file. All subdomain outputs seperately in different file.
*
*  @param arr Subdomain data
*  @param what_mat Data type
*  @param print_id Current checkpoint id
*  @param projection Spatial reference system projection string  
*/			
		void write_output_binary_parallel(Matrix::matrix<T>& arr, std::string what_mat, int print_id);


#ifdef TRITON_GDAL
/** @brief It outputs a specific data array's full domain in a single GeoTIFF file. 
*
*  @param arr Subdomain data
*  @param what_mat Data type
*  @param print_id Current checkpoint id
*  @param projection Spatial reference system projection string  
*/		
		void write_output_geotiff_sequential(Matrix::matrix<T>& arr, std::string what_mat, int print_id, std::string projection);
		
		
/** @brief It outputs a specific data array's sub domain in a GeoTIFF file. All subdomain outputs seperately in different file.
*
*  @param arr Subdomain data
*  @param what_mat Data type
*  @param print_id Current checkpoint id
*/			
		void write_output_geotiff_parallel(Matrix::matrix<T>& arr, std::string what_mat, int print_id, std::string projection);

/** @brief Writes a Virtual Raster Table (VRT) file for combining multiple raster outputs
*
*  @param what_mat Data type identifier
*  @param print_id Current checkpoint id
*  @param all_rows Vector containing row information for each subdomain
*  @param raster_cols Number of columns in the raster
*  @param xll Lower left X coordinate
*  @param yll Lower left Y coordinate
*  @param cellsize Cell size/resolution
*  @param projection Spatial reference system projection string
*  @param file_dir Output directory path
*/
		void write_output_vrt(const std::string &what_mat, int print_id,
				const std::vector<int> &all_rows, int raster_cols,
				double xll, double yll, double cellsize,
				const std::string &projection,
				const std::string &file_dir);

#endif
		
		
/** @brief It calculates output file name.
*
*  @param what Data type
*  @param subdir Output format directory
*  @param print_id Current checkpoint id
*  @param extension File extension
*  @return File name
*/	
		std::string get_mat_path(std::string what, std::string root_dir, std::string subdir, int print_id, std::string extension);
		

/** @brief It outputs time series data in a file.
*
*  @param arr Subdomain data
*  @param what_mat Data type
*  @param simtime Current time of simulation
*/		
		void output_time_series(T *value_obs, std::string what_mat, T simtime);

/** @brief It outputs time series data in a file.
*
*  @param h_arr_obs Water depth observation data
*  @param qx_arr_obs Discharge in x direction observation data
*  @param qy_arr_obs Discharge in y direction observation data
*  @param simtime Current time of simulation
*  @param print_option Which data to write
*/	

	
	void write_observation_data( T *h_arr_obs, T *qx_arr_obs, T *qy_arr_obs, T simtime, std::string print_option);

/** @brief It calculates content of updated configuration and outputs it in a file.
*
*  @param simtime Current time of simulation
*  @param print_id Current checkpoint id
*  @param average_dt Average time step size from the last output
*  @param it_count Total number of iterations so far
*/
		void output_cfg(T simtime, int print_id, T average_dt, int it_count);
		
		
/** @brief It calculates all custom timer value and output them.
*
*  @param st Timer object
*/		
		void write_times(SuperTimer::super_timer st, int print_id);
		
		
/** @brief It calculates average time of each timer for all MPI processes.
*
*  @param a Time array
*  @param n Size
*  @return Average value
*/		
		T average(T a[], int n);

/** @brief It writes the evolution of subdomain dimensions if dynamic load balancing is enabled.
*
*  @param pd Partition data
*  @param print_id Print output id
*/		
	void write_domain_decomposition(MpiUtils::partition_data_t pd, int print_id);
	




	private:
		int rows_;	/**< Number of rows in a subdomain */
		int cols_;	/**< Number of columns in a sub domain */
		T xll_;	/**< X coordinate of the origin */
		T yll_;	/**< Y coordinate of the origin */
		T cellsize_;	/**< Size of a cell */
		int rank_;	/**< Current subdomain id */
		int size_;	/**< Total number of sub domain */
		int time_series_flag_;	/**< Flag to determine output time series or not. If true, output the time series. */
		std::string project_dir_;	/**< Main project directory */
		std::string output_folder_;	/**< Output directory */
		std::string outfile_pattern_;	/**< Output file directory and name pattern */
		std::string cfg_content_;	/**< Contents of the input cfg (Configuration) file */
		std::string output_option_;	/**< Strategy to use for outputting into files. PAR for parallel outputs or SEQ for sequential outputs. PAR saves each MPI partitions subdomain in separate files and SEQ saves the whole domain into one file. */

		int num_of_obs_points_;	/**< Number of observation points per subdomain */
		int num_of_obs_points_global_;	/**< Number of observation points in the global domain */
		std::vector<int> relative_obs_index_;	/**< relative index position of observation cells per subdomain wrt to the global domain*/
		std::vector<int> time_series_index_relative_; /** relative index position of each observation cell in the global array after gathering*/
		int* relative_obs_index_global_;	/**< relative index position of observation cells in the global domain*/
		int* obs_points_per_subdomain;	/**< array of size MPI ranks containing the number of observation points per subdomain*/
		Constants::sources_list_t observation_cells_;	/**< Index position of observation cells in local domain */
		Constants::sources_list_t observation_cells_global_;	/**< Index position of observation cells in global domain */


	
	public:
		int cur_proc_data_size = 0;	/**< Number of cells in current subdomain */
		int *recvcounts = NULL;	/**< Array to hold every subdomains cell count */
		long long total_data_size = 0;	/**< Number of cells in main domain */
		int *displs = NULL;	/**< Position array to hold each sub domains starting point in main domain */
		T *total_data_arr = NULL;	/**< Main domains data or collection data of every subdomain */
		int *total_data_arr_int = NULL;	/**< Main domains data or collection data of every subdomain */
		int *displs_time_series = NULL ; /**< Position array to hold each sub domains starting point in main domain for time series */
		T *total_data_time_series = NULL ; /**< Main domain data for time series */
		
		

	};


	template<class T>
	output<T>::~output()
	{
		if (recvcounts != NULL)
		delete[] recvcounts;
		if (displs != NULL)
		delete[] displs;
		if (total_data_arr != NULL)
		delete[] total_data_arr;		
		if (displs_time_series != NULL)
		delete[] displs_time_series;
		if (total_data_time_series != NULL)
		delete[] total_data_time_series;		

	}


	template<typename T>
	void output<T>::init(int rows, int cols, T xll, T yll, T cellsize, int rank, int size, std::string project_dir, std::string output_folder, std::string outfile_pattern, int time_series_flag, std::string cfg_content, std::string output_option)
	{

		cur_proc_data_size = 0;
		if (recvcounts != NULL)
		delete[] recvcounts;
		total_data_size = 0;
		if (displs != NULL)
		delete[] displs;
		if (total_data_arr != NULL)
		delete[] total_data_arr;
		if (total_data_arr_int != NULL)
		delete[] total_data_arr_int;

		rows_ = rows;
		cols_ = cols;
		xll_ = xll;
		yll_ = yll;
		cellsize_ = cellsize;
		rank_ = rank;
		size_ = size;
		project_dir_ = project_dir;
		output_folder_ = output_folder;
		outfile_pattern_ = outfile_pattern;
		time_series_flag_ = time_series_flag;
		cfg_content_ = cfg_content;
		output_option_ = output_option;

		if (size == 1)
		{
			cur_proc_data_size = cols_ * rows_;
		}
		else if (rank_ == 0 || rank_ == size_ - 1)
		{
			cur_proc_data_size = cols_ * (rows_ - GHOST_CELL_PADDING);
		}
		else
		{
			cur_proc_data_size = cols_ * (rows_ - 2*GHOST_CELL_PADDING);
		}

		if (rank_ == 0)
		recvcounts = new int[size];
		MPI_Gather(&cur_proc_data_size, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, ENSIFY_COMM_WORLD);

		if (rank_ == 0)
		{
			displs = new int[size];
			displs[0] = 0;
			total_data_size += (long long) recvcounts[0];

			for (int i = 1; i < size_; i++)
			{
				total_data_size += (long long)recvcounts[i];
				displs[i] = displs[i - 1] + (long long)recvcounts[i - 1];
			}
			
			total_data_arr = new T[total_data_size];
			total_data_arr_int = new int[total_data_size];
		}

		MPI_Bcast(&total_data_size, 1, MPI_INT, 0, ENSIFY_COMM_WORLD);
	}


	template<typename T>
	void output<T>::init_time_series(int num_of_obs_points, int num_of_obs_points_global, std::vector<int> relative_obs_index, Constants::sources_list_t observation_cells, Constants::sources_list_t observation_cells_global, std::string print_option, int checkpoint_id)
	{

		if (displs_time_series != NULL)
		delete[] displs_time_series;
		if (total_data_time_series != NULL)
		delete[] total_data_time_series;	

		num_of_obs_points_ = num_of_obs_points;
		relative_obs_index_ = relative_obs_index;
		num_of_obs_points_global_=num_of_obs_points_global;
		observation_cells_ = observation_cells;
		observation_cells_global_ = observation_cells_global;

		if(rank_==0){
			obs_points_per_subdomain= new int[size_];
		}
		MPI_Gather(&num_of_obs_points_, 1, MPI_INT, obs_points_per_subdomain, 1, MPI_INT, 0, ENSIFY_COMM_WORLD);
		
		if (rank_ == 0)
		{
			displs_time_series = new int[size_];
			displs_time_series[0] = 0;

			for (int i = 1; i < size_; i++)
			{
				displs_time_series[i] = displs_time_series[i - 1] + obs_points_per_subdomain[i - 1];
			}
			
			total_data_time_series = new T[num_of_obs_points_global];
		}

		//auxiliary array to convert it from vector and to pass it to MPI_Gatherv
		int *relative_local_array = &relative_obs_index[0];

		relative_obs_index_global_ = NULL;
		if(rank_ == 0){
			relative_obs_index_global_ = (int*)malloc(num_of_obs_points_global_ * sizeof(int));
		}

    	MPI_Gatherv(relative_local_array, num_of_obs_points_, MPI_INT, relative_obs_index_global_, obs_points_per_subdomain, displs_time_series, MPI_INT, 0, ENSIFY_COMM_WORLD);

		if(rank_ == 0){
			for (int i = 0; i < num_of_obs_points_global_; i++){
				for (int j = 0; j < num_of_obs_points_global_; j++){
					if(i==relative_obs_index_global_[j]){
						time_series_index_relative_.push_back(j);
						break;
					}
				}
			}
		}

		std::string root_dir = project_dir_ + "/" + output_folder_ + "/";
		DIR* dir;
		if(root_dir.empty())
		{
			dir = opendir(".");
		}
		else
		{
			dir = opendir(root_dir.c_str());
		}
		if (!dir)
		{
			mkdir(root_dir.c_str(), S_IRWXU);
		}
		else
		closedir(dir);
		root_dir.pop_back();

		std::string outdir = project_dir_ + "/" + output_folder_ + "/" + TIME_SERIES_DIR + "/";
		DIR* dir2;
		if(outdir.empty())
		{
			dir2 = opendir(".");
		}
		else
		{
			dir2 = opendir(outdir.c_str());
		}
		if (!dir2)
		{
			mkdir(outdir.c_str(), S_IRWXU);
		}
		else
		closedir(dir2);

		if (rank_ == 0 && checkpoint_id == 0)
		{
			if (print_option.find("h") != std::string::npos){
				std::string filedir = outdir + "H_at_Xsec.txt";

				std::string str = "Time(s)";
				for (int i = 0; i < num_of_obs_points_global_; i++)
				{
					str = str + "," + "H" + "_at_Point_" + std::to_string(i + 1);
				}
				str = str + "\n";
				std::ofstream output(filedir);
				output << str;
				output.close();
			}
			if (print_option.find("u") != std::string::npos){
				std::string filedir = outdir + "QX_at_Xsec.txt";

				std::string str = "Time(s)";
				for (int i = 0; i < num_of_obs_points_global_; i++)
				{
					str = str + "," + "QX" + "_at_Point_" + std::to_string(i + 1);
				}
				str = str + "\n";
				std::ofstream output(filedir);
				output << str;
				output.close();
			}
			if (print_option.find("v") != std::string::npos){
				std::string filedir = outdir + "QY_at_Xsec.txt";

				std::string str = "Time(s)";
				for (int i = 0; i < num_of_obs_points_global_; i++)
				{
					str = str + "," + "QY" + "_at_Point_" + std::to_string(i + 1);
				}
				str = str + "\n";
				std::ofstream output(filedir);
				output << str;
				output.close();
			}
		}

		if (size_ > 1)
		{
			MPI_Barrier(ENSIFY_COMM_WORLD);
		}

	}


	template<typename T>
	void output<T>::write_output(Matrix::matrix<T>& h_arr, Matrix::matrix<T>& qx_arr, Matrix::matrix<T>& qy_arr, std::string output_format, std::string projection, std::string print_option, int print_id, int it_count, T simtime, T average_dt, Matrix::matrix<T>& max_value_h, std::string max_value_print_option)
	{
		if (strcmp(output_format.c_str(), "ASC") == 0)
		{
			if (print_option.find("h") != std::string::npos)
			{
				if(strcmp(output_option_.c_str(), "SEQ") == 0)
				{
					write_output_binary_sequential(h_arr, "H", print_id);
					write_output_ascii_sequential(h_arr, "H", print_id);
				}
				else
				{
					write_output_binary_parallel(h_arr, "H", print_id);
					write_output_ascii_parallel(h_arr, "H", print_id);
				}
			}

			if (print_option.find("u") != std::string::npos)
			{
				if(strcmp(output_option_.c_str(), "SEQ") == 0)
				{
					write_output_binary_sequential(qx_arr, "QX", print_id);
					write_output_ascii_sequential(qx_arr, "QX", print_id);
				}
				else
				{
					write_output_binary_parallel(qx_arr, "QX", print_id);
					write_output_ascii_parallel(qx_arr, "QX", print_id);
				}
			}

			if (print_option.find("v") != std::string::npos)
			{
				if(strcmp(output_option_.c_str(), "SEQ") == 0)
				{
					write_output_binary_sequential(qy_arr, "QY", print_id);
					write_output_ascii_sequential(qy_arr, "QY", print_id);
				}
				else
				{
					write_output_binary_parallel(qy_arr, "QY", print_id);
					write_output_ascii_parallel(qy_arr, "QY", print_id);
				}
			}
			
			
			if (max_value_print_option.size() > 0)
			{
				if (max_value_print_option.find("h") != std::string::npos)
				{
					if(strcmp(output_option_.c_str(), "SEQ") == 0)
					{
						write_output_binary_sequential(max_value_h, "MH", print_id);
						write_output_ascii_sequential(max_value_h, "MH", print_id);
					}
					else
					{
						write_output_binary_parallel(max_value_h, "MH", print_id);
						write_output_ascii_parallel(max_value_h, "MH", print_id);
					}
				}
			}
		}
		if (strcmp(output_format.c_str(), "BIN") == 0)
		{
			if (print_option.find("h") != std::string::npos)
			{
				if(strcmp(output_option_.c_str(), "SEQ") == 0)
				{
					write_output_binary_sequential(h_arr, "H", print_id);
				}
				else
				{
					write_output_binary_parallel(h_arr, "H", print_id);
				}
			}

			if (print_option.find("u") != std::string::npos)
			{
				if(strcmp(output_option_.c_str(), "SEQ") == 0)
				{
					write_output_binary_sequential(qx_arr, "QX", print_id);
				}
				else
				{
					write_output_binary_parallel(qx_arr, "QX", print_id);
				}
			}

			if (print_option.find("v") != std::string::npos)
			{
				if(strcmp(output_option_.c_str(), "SEQ") == 0)
				{
					write_output_binary_sequential(qy_arr, "QY", print_id);
				}
				else
				{
					write_output_binary_parallel(qy_arr, "QY", print_id);
				}
			}
			
			if (max_value_print_option.size() > 0)
			{
				if (max_value_print_option.find("h") != std::string::npos)
				{
					if(strcmp(output_option_.c_str(), "SEQ") == 0)
					{
						write_output_binary_sequential(max_value_h, "MH", print_id);
					}
					else
					{
						write_output_binary_parallel(max_value_h, "MH", print_id);
					}
				}
			}
		}
#ifdef TRITON_GDAL
		if (strcmp(output_format.c_str(), "GTIFF") == 0)
		{
			if (print_option.find("h") != std::string::npos)
			{
				if(strcmp(output_option_.c_str(), "SEQ") == 0)
				{
					write_output_geotiff_sequential(h_arr, "H", print_id, projection);
					write_output_binary_sequential(h_arr, "H", print_id);      // write binary as well for hotstart 
				}
				else
				{
					write_output_geotiff_parallel(h_arr, "H", print_id, projection);
					write_output_binary_parallel(h_arr, "H", print_id);  // write binary as well for hotstart 
			}
		}

			if (print_option.find("u") != std::string::npos)
			{
				if(strcmp(output_option_.c_str(), "SEQ") == 0)
				{
					write_output_geotiff_sequential(qx_arr, "QX", print_id, projection);
					write_output_binary_sequential(qx_arr, "QX", print_id);      // write binary as well for hotstart 
				}
				else
				{
					write_output_geotiff_parallel(qx_arr, "QX", print_id, projection);
					write_output_binary_parallel(qx_arr, "QX", print_id);  // write binary as well for hotstart 
				}
			}

			if (print_option.find("v") != std::string::npos)
			{
				if(strcmp(output_option_.c_str(), "SEQ") == 0)
				{
					write_output_geotiff_sequential(qy_arr, "QY", print_id, projection);
					write_output_binary_sequential(qy_arr, "QY", print_id);      // write binary as well for hotstart 
				}
				else
				{
					write_output_geotiff_parallel(qy_arr, "QY", print_id, projection);
					write_output_binary_parallel(qy_arr, "QY", print_id);  // write binary as well for hotstart 
				}
			}
			
			if (max_value_print_option.size() > 0)
			{
				if (max_value_print_option.find("h") != std::string::npos)
				{
					if(strcmp(output_option_.c_str(), "SEQ") == 0)
					{
						write_output_geotiff_sequential(max_value_h, "MH", print_id, projection);
						write_output_binary_sequential(max_value_h, "MH", print_id);      // write binary as well for hotstart 
					}
					else
					{
						write_output_geotiff_parallel(max_value_h, "MH", print_id, projection);
						write_output_binary_parallel(max_value_h, "MH", print_id);  // write binary as well for hotstart 
					}
				}
			}
		}
#endif
		if (rank_ == 0)
		{

			{
				std::ostringstream oss;
				oss << BLUE << "[" << print_id << "]" << RESET << " File written at time " << simtime << " seconds.\n";
				std::cerr << oss.str();
			}
			output_cfg(simtime, print_id, average_dt, it_count);
			std::ofstream cidfile("cid");
			if (cidfile.is_open())
			{
				cidfile << print_id;
			}
			cidfile.close();
		}

	}
	

	template<typename T>
	std::ostream& operator<<(std::ostream& out, Matrix::matrix<T>& M)
	{
		int m = M.get_num_rows();
		int n = M.get_num_cols();
		int off = GHOST_CELL_PADDING;
		for (int i = off; i < m - off; i++)
		{
			for (int j = off; j < n - off; j++)
			{
				out << (T)M(i, j);

				if (j < (n - off) - off)
				{
					out << " ";
				}
			}
			out << std::endl;
		}

		return out;
	}


	template<typename T>
	std::string output<T>::get_mat_path(std::string what, std::string root_dir, std::string subdir, int print_id, std::string extension)
	{
		std::string format = outfile_pattern_ + extension;
		std::vector<char> buf(256);

		std::snprintf(
		&buf[0], buf.size(), format.c_str(),
		root_dir.c_str(),
		subdir.c_str(),
		what.c_str(),
		print_id,
		rank_
		);

		return std::string(&buf[0]);
	}

	template<typename T>
	void output<T>::output_cfg(T simtime, int print_id, T average_dt, int it_count)
	{
		std::string str2 = cfg_content_;
		std::istringstream ss(str2);
		std::string line;
		while (getline(ss, line))
		{
			if (line.find("sim_start_time=") != std::string::npos)
			{
				size_t startPos = str2.find("sim_start_time=");
				std::ostringstream strOss;
				strOss << std::fixed << std::setprecision(std::numeric_limits<T>::max_digits10) << simtime;
				std::string strObj = strOss.str();
				str2.replace(startPos, line.length(), "sim_start_time=" + strObj);
			}
			else if (line.find("checkpoint_id=") != std::string::npos)
			{
				size_t startPos = str2.find("checkpoint_id=");
				str2.replace(startPos, line.length(), "checkpoint_id=" + std::to_string(print_id));
			}
			else if (line.find("time_step=") != std::string::npos)
			{
				size_t startPos = str2.find("time_step=");
				std::ostringstream strOss;
				strOss << std::fixed << std::setprecision(std::numeric_limits<T>::max_digits10) << average_dt;
				std::string strObj = strOss.str();
				str2.replace(startPos, line.length(), "time_step=" + strObj);
			}
			else if (line.find("it_count=") != std::string::npos)
			{
				size_t startPos = str2.find("it_count=");
				str2.replace(startPos, line.length(), "it_count=" + std::to_string(it_count));
			}
		}
		std::string outdir = project_dir_ + "/" + output_folder_ + "/" + CFG_DIR + "/";
		DIR* dir;
		if(outdir.empty())
		{
			dir = opendir(".");
		}
		else
		{
			dir = opendir(outdir.c_str());
		}
		if (!dir)
		{
			mkdir(outdir.c_str(), S_IRWXU);
		}
		else
		closedir(dir);

		std::string filedir = outdir + "config_" + std::to_string(print_id) + ".cfg";

		std::ofstream output(filedir);
		output << str2;
		output.close();
	}


	template<typename T>
	void output<T>::write_times(SuperTimer::super_timer st, int print_id)
	{

		T compute_time = st.get_custom_time(COMPUTE_TIME);
		T mpi_time = st.get_custom_time(MPI_TIME);
		T io_time = st.get_custom_time(IO_TIME);
		T resize_time = st.get_custom_time(RESIZE_TIME);
		T simulation_time = st.get_custom_time(SIMULATION_TIME);
		T total_time = st.get_custom_time(TOTAL_TIME);
		T other_time = simulation_time - compute_time - mpi_time - io_time - resize_time;
		T init_time = total_time - simulation_time;

		T *compute_time_all = new T[size_];
		T *mpi_time_all = new T[size_];
		T *io_time_all = new T[size_];
		T *simulation_time_all = new T[size_];
		T *total_time_all = new T[size_];
		T *other_time_all = new T[size_];
		T *init_time_all = new T[size_];
		T *resize_time_all = new T[size_];


		MPI_Gather(&compute_time, 1, MPI_DATA_TYPE, &compute_time_all[rank_], 1, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
		MPI_Gather(&mpi_time, 1, MPI_DATA_TYPE, &mpi_time_all[rank_], 1, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
		MPI_Gather(&io_time, 1, MPI_DATA_TYPE, &io_time_all[rank_], 1, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
		MPI_Gather(&simulation_time, 1, MPI_DATA_TYPE, &simulation_time_all[rank_], 1, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
		MPI_Gather(&total_time, 1, MPI_DATA_TYPE, &total_time_all[rank_], 1, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
		MPI_Gather(&other_time, 1, MPI_DATA_TYPE, &other_time_all[rank_], 1, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
		MPI_Gather(&init_time, 1, MPI_DATA_TYPE, &init_time_all[rank_], 1, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
		MPI_Gather(&resize_time, 1, MPI_DATA_TYPE, &resize_time_all[rank_], 1, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
		
		if (size_ > 1)
		{
			MPI_Barrier(ENSIFY_COMM_WORLD);
		}

		if(rank_ == 0)
		{
			std::string outdir;
			std::string filedir;

			//not the final state
			if(print_id!=-1){
				std::string outdir = project_dir_ + "/" + output_folder_ + "/performance/";
				DIR* dir;
				if(outdir.empty())
				{
					dir = opendir(".");
				}
				else
				{
					dir = opendir(outdir.c_str());
				}
				if (!dir)
				{
					mkdir(outdir.c_str(), S_IRWXU);
				}
				else
				closedir(dir);

				filedir = outdir + "performance" + std::to_string(print_id) + ".txt";

			}else{
				//the final state
				std::string outdir = project_dir_ + "/" + output_folder_ + "/";
				filedir = outdir + "performance.txt";
			}
			std::ofstream output(filedir);
			output << "%Rank, Compute, MPI, IO, Resize, Other, Simulation, Init, Total" << std::endl;
			
			for(int j=0;j<size_;j++){
				output << std::setprecision(4) << j << ", " << compute_time_all[j] << ", " <<  mpi_time_all[j] << ", " <<	io_time_all[j] << ", " <<	resize_time_all[j] << ", " << other_time_all[j] << ", " 
				<< simulation_time_all[j] << ", " << init_time_all[j] <<  ", " << total_time_all[j] << std::endl;
			}
			output << std::setprecision(4) << "Average" << ", " << average(compute_time_all,size_) << ", " <<  average(mpi_time_all,size_) << ", " <<	 average(io_time_all,size_) << ", " << average(resize_time_all,size_) << ", " <<  average(other_time_all,size_) << ", " <<  average(simulation_time_all,size_) << ", " <<  average(init_time_all,size_) <<  ", " << average(total_time_all,size_) << std::endl;

			output.close();
		}
		
	}
	
	
	template<typename T>
	T output<T>::average(T a[], int n) 
	{ 
		T sum = 0; 
		for (int i=0; i<n; i++) 
		sum+= a[i]; 

		return sum/n; 
	}	 
	template<typename T>
	void output<T>::write_domain_decomposition(MpiUtils::partition_data_t pd, int print_id)
	{
		if(rank_ == 0)
		{

			std::string outdir = project_dir_ + "/" + output_folder_ + "/domain_decomposition/";
			DIR* dir;
			if(outdir.empty())
			{
				dir = opendir(".");
			}
			else
			{
				dir = opendir(outdir.c_str());
			}
			if (!dir)
			{
				mkdir(outdir.c_str(), S_IRWXU);
			}
			else
			closedir(dir);

			std::string filedir = outdir + "domain_decomposition" + std::to_string(print_id) + ".txt";
			std::ofstream output(filedir);
			output << "%Rank,Rows,Cols" << std::endl;

			for(int j=0;j<size_;j++){
				output << j << ","  << pd.part_dims[j].first-2*GHOST_CELL_PADDING << ","  << pd.part_dims[j].second  - 2*GHOST_CELL_PADDING << std::endl;
			}
			output.close();

		}
		
		 if (size_ > 1)
		 {
					MPI_Barrier(ENSIFY_COMM_WORLD);
		 }



	}
}

#include "output_ascii.h"
#include "output_binary.h"
#include "output_geotiff.h"
#include "output_timeseries.h"

#endif
