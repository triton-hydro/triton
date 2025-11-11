/** @file main.cpp
 *  @brief Main file containing the driver 
 *
 *  This contains the subroutines and eventually any
 *  macros, constants, etc. needed for the driver
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


#include <iostream>
#include <string>
#include <float.h>
#include <utility>
#include <map>
#include <fstream>
#include <vector>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <dirent.h>
#include <cmath>
#include <cstring>

#ifdef KOKKOS_ENABLE_OPENMP
#include <omp.h>
#endif

#include "mpi.h"
#include "Ensify.h"

using namespace std;

#include "constants.h"
#include "supertimer.h"
#include "string_utils.h"
#include "mpi_utils.h"
#include "config_utils.h"
#include "inflow.h"
#include "extbc.h"
#include "matrix.h"
#include "dem_utils.h"
#include "output.h"
#include "triton.h"
#include "kernels.h"


/** @brief Main function. This is the main function of the program.
*
*  @param argc Argument count
*  @param argv Pointer array which points to each argument passed to the program. The program runs with cfg filename and number of threads (only for OpenMP version)
*  @return 0
*/

int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);

#ifdef ENSEMBLE_BUILD
    ensify::init(argc, argv);
    std::cerr << IN "Running TRITON in ensemble mode" << std::endl;
#endif

  Kokkos::initialize();
  {
    MPI_Comm_size(ENSIFY_COMM_WORLD, &size);
    MPI_Comm_rank(ENSIFY_COMM_WORLD, &rank);

#ifdef ENSEMBLE_BUILD
    Triton::triton<value_t> model(ensify::argc(), ensify::argv());
#else
    Triton::triton<value_t> model(argc, argv);
#endif

    //initialize
    model.initialize(rank, size);
    //simulate
    model.simulate();
  }
  Kokkos::finalize();
#ifdef ENSEMBLE_BUILD
  ensify::finalize();
#endif
	MPI_Finalize();
	return 0;
}
