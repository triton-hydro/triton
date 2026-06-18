// Extracted from src/output.h — Stage 3 header split.
// Contains write_output_ascii_{sequential,parallel}.

#ifndef TRITON_OUTPUT_ASCII_H
#define TRITON_OUTPUT_ASCII_H

#include "output.h"

namespace Output {

	template<typename T>
	void output<T>::write_output_ascii_sequential(Matrix::matrix<T>& arr, std::string what_mat, int print_id)
	{
		if(rank_ == 0)
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

			std::string filepath = get_mat_path(what_mat, root_dir, ASCII_DIR, print_id, ".out");
			std::string file_dir = project_dir_ + "/" + output_folder_ + "/" + ASCII_DIR + "/";

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

			std::ofstream mat;
			mat.precision(6);
			mat.open((filepath).c_str());
			mat << std::fixed;

			int off = GHOST_CELL_PADDING;
			int total_cols = cols_;
			int total_rows = total_data_size/ total_cols;
			for(int i=off; i<total_rows-off; i++)
			{
				for(int j=off; j<total_cols-off;j++)
				{
					mat << total_data_arr[i*(long long)total_cols+j];

					if (j < total_cols - off - 1)
					{
						mat << " ";
					}
				}
				mat << std::endl;
			}
			mat.close();
		}
		if (size_ > 1)
		{
			MPI_Barrier(ENSIFY_COMM_WORLD);
		}
	}


	template<typename T>
	void output<T>::write_output_ascii_parallel(Matrix::matrix<T>& arr, std::string what_mat, int print_id)
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

		std::string filepath = get_mat_path(what_mat, root_dir, ASCII_DIR, print_id, ".out");
		std::string file_dir = project_dir_ + "/" + output_folder_ + "/" + ASCII_DIR + "/";

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

		std::ofstream mat;
		mat.precision(6);
		mat.open((filepath).c_str());
		mat << std::fixed << arr;
		mat.close();


	}

} // namespace Output

#endif // TRITON_OUTPUT_ASCII_H
