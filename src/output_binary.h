// Extracted from src/output.h — Stage 3 header split.
// Contains write_output_binary_{sequential,parallel}.

#ifndef TRITON_OUTPUT_BINARY_H
#define TRITON_OUTPUT_BINARY_H

#include "output.h"

namespace Output {

	template<typename T>
	void output<T>::write_output_binary_sequential(Matrix::matrix<T>& arr, std::string what_mat, int print_id)
	{
		if (rank_ == 0)
		{
			MPI_Gatherv(arr.get_address_at(0, 0), cur_proc_data_size, MPI_DATA_TYPE, total_data_arr, recvcounts, displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
		}
		else
		{
			MPI_Gatherv(arr.get_address_at(GHOST_CELL_PADDING, 0), cur_proc_data_size, MPI_DATA_TYPE, total_data_arr, recvcounts, displs, MPI_DATA_TYPE, 0, ENSIFY_COMM_WORLD);
		}

		if (rank_ == 0)
		{
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


			std::string filepath = get_mat_path(what_mat, root_dir, BIN_DIR, print_id, ".out");
			std::string file_dir = project_dir_ + "/" + output_folder_ + "/" + BIN_DIR + "/";

			DIR* dir2;
			if(file_dir.empty())
			{
				dir2 = opendir(".");
			}
			else
			{
				dir2 = opendir(file_dir.c_str());
			}
			if (!dir2)
			{
				mkdir(file_dir.c_str(), S_IRWXU);
			}
			else
			closedir(dir2);

			std::ofstream mat((filepath).c_str(), std::ios::binary);

			int total_cols = cols_;
			int total_rows = total_data_size/ total_cols;
			int off = GHOST_CELL_PADDING;

			T put_rows_value = (T)(total_rows - 2 * off);
			T put_cols_value = (T)(total_cols - 2 * off);

			mat.write((char*) &put_rows_value, sizeof(T));
			mat.write((char*) &put_cols_value, sizeof(T));

			for(int i=off; i<total_rows-off; i++)
			{
				mat.write((char*) &total_data_arr[i*(long long)total_cols+off], (total_cols-2*off) * sizeof(T));
			}
			mat.close();
		}
		if (size_ > 1)
		{
			MPI_Barrier(ENSIFY_COMM_WORLD);
		}
	}


	template<typename T>
	void output<T>::write_output_binary_parallel(Matrix::matrix<T>& arr, std::string what_mat, int print_id)
	{
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


		std::string filepath = get_mat_path(what_mat, root_dir, BIN_DIR, print_id, ".out");
		std::string file_dir = project_dir_ + "/" + output_folder_ + "/" + BIN_DIR + "/";

		DIR* dir2;
		if(file_dir.empty())
		{
			dir2 = opendir(".");
		}
		else
		{
			dir2 = opendir(file_dir.c_str());
		}
		if (!dir2)
		{
			mkdir(file_dir.c_str(), S_IRWXU);
		}
		else
		closedir(dir2);

		std::ofstream mat((filepath).c_str(), std::ios::binary);
		int off = GHOST_CELL_PADDING;

		T put_rows_value = (T)(rows_ - 2 * off);
		T put_cols_value = (T)(cols_ - 2 * off);

		mat.write((char*) &put_rows_value, sizeof(T));
		mat.write((char*) &put_cols_value, sizeof(T));

		for(int i=off; i<rows_-off; i++)
		{
			mat.write((char*)arr.get_address_at(i,off), (cols_-2*off) * sizeof(T));
		}

		mat.close();
	}

} // namespace Output

#endif // TRITON_OUTPUT_BINARY_H
