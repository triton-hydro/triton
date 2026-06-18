// Extracted from src/output.h — Stage 3 header split.
// Contains write_output_geotiff_* and write_output_vrt.
// GDAL-guarded.

#ifndef TRITON_OUTPUT_GEOTIFF_H
#define TRITON_OUTPUT_GEOTIFF_H

#include <sstream>
#include <iomanip>
#include "output.h"

namespace Output {

#ifdef TRITON_GDAL
	template<typename T>
	void output<T>::write_output_geotiff_sequential(Matrix::matrix<T>& arr, std::string what_mat, int print_id, std::string projection)
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


			std::string filepath = get_mat_path(what_mat, root_dir, std::string(GEO_DIR), print_id, ".tif");
			std::string file_dir = project_dir_ + "/" + output_folder_ + "/" + GEO_DIR + "/";

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

			// Initialize GDAL
			GDALAllRegister();

			int total_cols = cols_;
			int total_rows = total_data_size / total_cols;
			int off = GHOST_CELL_PADDING;

			int raster_rows = total_rows - 2 * off;
			int raster_cols = total_cols - 2 * off;

			GDALDriver* poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
			if (poDriver == nullptr)
			{
				std::cerr << "Unable to obtain GeoTIFF driver\n";
				return;
			}

			GDALDataset* poDataset = poDriver->Create(filepath.c_str(), raster_cols, raster_rows, 1, GDT_Float32, nullptr);
			if (poDataset == nullptr)
			{
				std::cerr << std::string("Error creating the GeoTIFF file in ") + filepath + "\n";
				return;
			}

			// Geo-referenced transformation. GeoTiff uses top left corner as origin
			// T adfGeoTransform[6] = { xOrigin, pixelWidth, xRotation, yOrigin, yRotation, pixelHeight };
			T yll = yll_;
			T xll = xll_;
			T pixel_size = cellsize_;
			T y_ul = yll + raster_rows * pixel_size; // Upper left corner y coordinate
			// Convert to double for GDAL
			double adfGeoTransform[6] = {
				static_cast<double>(xll),
				static_cast<double>(pixel_size),
				0.0,
				static_cast<double>(y_ul),
				0.0,
				static_cast<double>(-pixel_size)
			};
		poDataset->SetGeoTransform(adfGeoTransform);

			// Assign coordinate system
			OGRSpatialReference oSRS;
			oSRS.SetFromUserInput(projection.c_str()); // Pass projection string from user #edited SG
			char* pszSRSWKT = nullptr;
			oSRS.exportToWkt(&pszSRSWKT);
			poDataset->SetProjection(pszSRSWKT);
			CPLFree(pszSRSWKT);

			GDALRasterBand* poBand = poDataset->GetRasterBand(1);

			for (int i = 0; i < raster_rows; i++)
			{
				float *pafWriteline = (float *)CPLMalloc(sizeof(float) * raster_cols);
				for (int j = 0; j < raster_cols; j++)
				{
					pafWriteline[j] = total_data_arr[(i + off) * (long long)total_cols + j + off];
				}
				CPLErr err = poBand->RasterIO(GF_Write, 0, i, raster_cols, 1, pafWriteline, raster_cols, 1, GDT_Float32, 0, 0);
				if (err != CE_None)
				{
					std::cerr << std::string("Error writing to raster in row ") + std::to_string(i) + "\n";
				}
				CPLFree(pafWriteline);
			}

			GDALClose(poDataset);
		}

		if (size_ > 1)
		{
			MPI_Barrier(ENSIFY_COMM_WORLD);
		}
	}


	template<typename T>
	void output<T>::write_output_geotiff_parallel(Matrix::matrix<T>& arr, std::string what_mat, int print_id, std::string projection)
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


		std::string filepath = get_mat_path(what_mat, root_dir, GEO_DIR, print_id, ".tif");
		std::string file_dir = project_dir_ + "/" + output_folder_ + "/" + GEO_DIR + "/";

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

		// Initialize GDAL
		GDALAllRegister();

		int off = GHOST_CELL_PADDING;
		int raster_rows = rows_ - 2 * off; // number of local rows without ghost cells for each subdomain
		int raster_cols = cols_ - 2 * off;

		// Gather number of rows from all processes to calculate y_ul correctly
		std::vector<int> all_rows(size_);
		MPI_Allgather(&raster_rows, 1, MPI_INT, all_rows.data(), 1, MPI_INT, ENSIFY_COMM_WORLD);
		int rows_below = 0;
		for (int r = rank_; r < size_; ++r) {
			rows_below += all_rows[r];
		}

		GDALDriver* poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
		if (poDriver == nullptr)
		{
			std::cerr << "Unable to obtain GeoTIFF driver\n";
			return;
		}

		GDALDataset* poDataset = poDriver->Create(filepath.c_str(), raster_cols, raster_rows, 1, GDT_Float32, nullptr);
		if (poDataset == nullptr)
		{
			std::cerr << std::string("Error creating GeoTIFF file in ") + filepath + "\n";
			return;
		}


		// Geo-referenced transformation. GeoTiff uses top left corner as origin
		// T adfGeoTransform[6] = { xOrigin, pixelWidth, xRotation, yOrigin, yRotation, pixelHeight };
		T yll = yll_;
		T xll = xll_;
		T pixel_size = cellsize_;
		T y_ul = yll + rows_below * pixel_size; // Upper left corner y coordinate
		//T adfGeoTransform[6] = { xll, pixel_size, 0.0, y_ul, 0.0, -pixel_size };

		// Convert to double for GDAL
		double adfGeoTransform[6] = {
			static_cast<double>(xll),
			static_cast<double>(pixel_size),
			0.0,
			static_cast<double>(y_ul),
			0.0,
			static_cast<double>(-pixel_size)
		};

		poDataset->SetGeoTransform(adfGeoTransform);

		// Assign coordinate system
		OGRSpatialReference oSRS;
		oSRS.SetFromUserInput(projection.c_str()); // Pass projection string from user
		char* pszSRSWKT = nullptr;
		oSRS.exportToWkt(&pszSRSWKT);
		poDataset->SetProjection(pszSRSWKT);
		CPLFree(pszSRSWKT);

		GDALRasterBand* poBand = poDataset->GetRasterBand(1);

		for (int i = 0; i < raster_rows; i++)
		{
			float *pafWriteline = (float *)CPLMalloc(sizeof(float) * raster_cols);
			for (int j = 0; j < raster_cols; j++)
			{
				pafWriteline[j] = *arr.get_address_at(i,j); // This way we access the value of the pointer that points to the memory address (i,j) of the matrix, which is what we are interested in.
			}

			CPLErr err = poBand->RasterIO(GF_Write, 0, i, raster_cols, 1, pafWriteline, raster_cols, 1, GDT_Float32, 0, 0);
			if (err != CE_None)
			{
				std::cerr << std::string("Error writing to raster in row ") + std::to_string(i) + "\n";
			}
			CPLFree(pafWriteline);
		}

		GDALClose(poDataset);

		// Ensure all processes have finished writing before creating VRT
		MPI_Barrier(ENSIFY_COMM_WORLD);
		if (rank_ == 0) {
			write_output_vrt(what_mat, print_id, all_rows, raster_cols, xll_, yll_, cellsize_, projection, file_dir);
		}
	}

	template <typename T>
	void output<T>::write_output_vrt(const std::string &what_mat, int print_id,
				const std::vector<int> &all_rows, int raster_cols,
				double xll, double yll, double cellsize,
				const std::string &projection,
				const std::string &file_dir)
	{
		// Total number of rows in the full domain
		int total_rows = 0;
		for (int r : all_rows) total_rows += r;

		// VRT file name
		std::ostringstream vrt_oss;
		vrt_oss << file_dir << what_mat << "_" << std::setw(2) << std::setfill('0') << print_id << ".vrt";
		std::string vrt_name = vrt_oss.str();

		std::ofstream vrt(vrt_name);
		vrt << "<VRTDataset rasterXSize=\"" << raster_cols
			<< "\" rasterYSize=\"" << total_rows << "\">\n";

		// GeoTransform
		double y_ul = yll + total_rows * cellsize;
		vrt << "  <GeoTransform> "
			<< xll << ", " << cellsize << ", 0.0, "
			<< y_ul << ", 0.0, " << -cellsize
			<< " </GeoTransform>\n";

		// Spatial Reference
		vrt << "  <SRS>" << projection << "</SRS>\n";

		vrt << "  <VRTRasterBand dataType=\"Float32\" band=\"1\">\n";
		vrt << "    <ColorInterp>Gray</ColorInterp>\n";

		int offset = 0;
		for (size_t r = 0; r < all_rows.size(); ++r) {
			std::ostringstream tif_oss;
			tif_oss << what_mat << "_" << std::setw(2) << std::setfill('0') << print_id << "_" << std::setw(2) << std::setfill('0') << r << ".tif";
			std::string tif_name = tif_oss.str();

			vrt << "    <SimpleSource>\n";
			vrt << "      <SourceFilename relativeToVRT=\"1\">" << tif_name << "</SourceFilename>\n";
			vrt << "      <SourceBand>1</SourceBand>\n";
			vrt << "      <SourceProperties RasterXSize=\"" << raster_cols
				<< "\" RasterYSize=\"" << all_rows[r]
				<< "\" DataType=\"Float32\" BlockXSize=\"" << raster_cols
				<< "\" BlockYSize=\"1\" />\n";
			vrt << "      <SrcRect xOff=\"0\" yOff=\"0\" xSize=\"" << raster_cols
				<< "\" ySize=\"" << all_rows[r] << "\" />\n";
			vrt << "      <DstRect xOff=\"0\" yOff=\"" << offset
				<< "\" xSize=\"" << raster_cols
				<< "\" ySize=\"" << all_rows[r] << "\" />\n";
			vrt << "    </SimpleSource>\n";

			offset += all_rows[r];
		}

		vrt << "  </VRTRasterBand>\n";
		vrt << "</VRTDataset>\n";
		vrt.close();
	}


#endif

} // namespace Output

#endif // TRITON_OUTPUT_GEOTIFF_H
