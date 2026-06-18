// Extracted from src/matrix.h — Stage 3 header split.
// Contains load_from_ascii_file / load_from_binary_file overloads.

#ifndef TRITON_MATRIX_IO_H
#define TRITON_MATRIX_IO_H

#include "matrix.h"
#include <string>
#include <fstream>
#include <vector>

namespace Matrix {

	template<Arithmetic T>
	void matrix<T>::load_from_ascii_file(std::string& filepath)
	{
		int j = 0;
		int rownum = 0;
		std::tuple<int, int> dims = get_dims_2d(filepath);

		std::ifstream infile(filepath.c_str());

		if (!infile.is_open())
		{
			std::cerr << ERROR "Error reading file: " << filepath << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			std::cerr << IN "Reading file " << filepath << std::endl;
		}

		std::string line;

		std::getline(infile, line);

		while (!infile.eof())
		{
			std::vector<std::string> row = StringUtils::split(line, ' ');

			if (rownum == 0)
			{
				this->rows_ = std::get<1>(dims);
				this->cols_ = std::get<0>(dims);

				this->set_size(this->rows_, this->cols_);
				this->zero_fill();
			}
			else
			{
				std::string val;
				std::vector<std::string>::iterator strit = row.begin();
				j = 0;

				for (; strit != row.end(); strit++, j++)
				{
					val = *strit;

					if (val.find(".") != std::string::npos)
					{
						this->data_[(long long)this->cols_ * (rownum - 1) + j] = atof(val.c_str());
					}
					else
					{
						this->data_[(long long)this->cols_ * (rownum - 1) + j] = atoi(val.c_str());
					}
				}
			}
			rownum++;
			std::getline(infile, line);
		}
		infile.close();
		std::cerr << OK "File " << filepath << " read" << std::endl;
	}


	template<Arithmetic T>
	void matrix<T>::load_from_ascii_file(int rows, int cols, std::string& filepath)
	{
		std::tuple<int, int> dims = get_dims_2d(filepath);
		if (rows != std::get<1>(dims) || cols != std::get<0>(dims))
		{
			std::cerr << std::string("Invalid dimension of ") + filepath + ". (row,col): (" + std::to_string(std::get<1>(dims)) + "," + std::to_string(std::get<0>(dims)) + ")\n";
			exit(EXIT_FAILURE);
		}

		this->set_size(rows, cols);

		int i = 0;
		int percentage=10;

		std::ifstream infile(filepath);

		if (!infile.is_open())
		{
			std::cerr << ERROR "Error reading file: " << filepath << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			std::cerr << IN "Reading file " << filepath << std::endl;
		}

		std::string line;

		while (infile.good())
		{
			int j = 0;
			std::getline(infile, line);
			std::vector<std::string> row = StringUtils::split(line, ' ');
			std::string val;
			std::vector<std::string>::iterator strit = row.begin();

			for (; strit != row.end(); strit++, j++)
			{
				if(j>cols-1){
					std::cerr << std::endl << ERROR "Error reading file: " << filepath << ". More than one space as separator?. Check row " << i+1 << std::endl;
					exit(EXIT_FAILURE);
				}

				val = *strit;

				if(val.find(".") != std::string::npos)
				{
					this->data_[((long long)this->cols_ * i) + j] = (T)atof(val.c_str());
				}
				else
				{
					this->data_[((long long)this->cols_ * i) + j] = (T)atoi(val.c_str());
				}
			}
			i++;
			//this is to show the percentage (by 10%) for large files
			if((long long)cols*rows>1e7 && (i*100/rows > percentage)){
				if(percentage==10){
					std::cerr << "     " ;
				}
				std::cerr << percentage << "% ";
				percentage+=10;
				if(percentage==100){
					std::cerr << std::endl;
				}
			}

		}
		infile.close();
		std::cerr << OK "File " << filepath << " read" << std::endl;

	}


	template<Arithmetic T>
	void matrix<T>::load_from_ascii_file(int rows, int cols, std::string& filepath, int header_size)
	{

		this->set_size(rows, cols);

		int i = 0;
		std::ifstream infile(filepath);

		if (!infile.is_open())
		{
			std::cerr << ERROR "Error reading file: " << filepath << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			std::cerr << IN "Reading file " << filepath << std::endl;
		}

		std::string line;

		int line_number = 0;
		int percentage=10;
		while (infile.good())
		{
			std::getline(infile, line);

			line_number++;
			if (line_number <= header_size)
			continue;

			int j = 0;
			std::vector<std::string> row = StringUtils::split(line, ' ');
			std::string val;
			std::vector<std::string>::iterator strit = row.begin();

			for (; strit != row.end(); strit++, j++)
			{
				if(j>cols-1){
					std::cerr << std::endl << ERROR "Error reading file: " << filepath << ". More than one space as separator?. Check row " << i+1 << std::endl;
					exit(EXIT_FAILURE);
				}

				val = *strit;

				if(val.find(".") != std::string::npos)
				{
					this->data_[((long long)this->cols_ * i) + j] = (T)atof(val.c_str());
				}
				else
				{
					this->data_[((long long)this->cols_ * i) + j] = (T)atoi(val.c_str());
				}
			}
			i++;
			//this is to show the percentage (by 10%) for large files
			if((long long)cols*rows>1e7 && (i*100/rows > percentage)){
				if(percentage==10){
					std::cerr << "     " ;
				}
				std::cerr << percentage << "% ";
				percentage+=10;
				if(percentage==100){
					std::cerr << std::endl;
				}
			}
		}
		infile.close();
		std::cerr << OK "File " << filepath << " read" << std::endl;

	}


	template<Arithmetic T>
	void matrix<T>::load_from_binary_file(int rows, int cols, std::string& filepath)
	{
		this->set_size(rows, cols);
		std::ifstream infile(filepath, std::ios::binary);

		if (!infile.is_open())
		{
			std::cerr << ERROR "Error reading file: " << filepath << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			std::cerr << IN "Reading file " << filepath << std::endl;
		}

		T *arr = new T [BIN_DEFAULT_HEADER_SIZE];
		infile.read( (char*) arr, sizeof(T) * BIN_DEFAULT_HEADER_SIZE );
		int file_row = (int)arr[BIN_ROW_ID];
		int file_col = (int)arr[BIN_COL_ID];

		if(rows!= file_row || cols!=file_col)
		{
			infile.close();
			std::cerr << ERROR "Invalid Matrix dimensions" << std::endl;
			exit(EXIT_FAILURE);

		}

		infile.seekg(sizeof(T) * BIN_DEFAULT_HEADER_SIZE, std::ios::beg);
		infile.read((char*)this->data_, sizeof(T) * rows * (long long)cols);

		infile.close();
		std::cerr << OK "File " << filepath << " read" << std::endl;

	}


	template<Arithmetic T>
	void matrix<T>::load_from_binary_file(int rows, int cols, std::string& filepath, int header_size)
	{

		this->set_size(rows, cols);
		std::ifstream infile(filepath, std::ios::binary);

		if (!infile.is_open())
		{
			std::cerr << ERROR "Error reading file: " << filepath << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			std::cerr << IN "Reading file " << filepath << std::endl;
		}

		infile.seekg(sizeof(T) * header_size, std::ios::beg);
		infile.read((char*)this->data_, sizeof(T) * rows * (long long)cols);

		infile.close();
		std::cerr << OK "File " << filepath << " read" << std::endl;

	}

} // namespace Matrix

#endif // TRITON_MATRIX_IO_H
