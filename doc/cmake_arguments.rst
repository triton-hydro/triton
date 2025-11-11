.. _cmake_arguments:

CMake Command-line Arguments
==================================

Using CMake
-----------------------------------------

TRITON uses CMake as its build system. CMake is an open-source, cross-platform build system generator that manages the build process of software projects.

Its command-line syntax is shown below:

.. code-block:: bash

  cmake <path-to-source> [options] 

Controlling TRITON Compilation and Execution
--------------------------------------------

To provide users with controls over TRITON build and execution process,
TRITON accepts the following **OPTIONAL** CMake command-line arguments.


``-DMACHINE=<predefined-machine-name|path-to-custom-machinefile>``
   This argument specifies the name of the machine. The machine name can be either one of the predefined configurations located in the <TRITON_HOME>/cmake/machines directory or the path to a custom machine configuration file. If this argument is omitted, the build system automatically attempts to detect the machine name and use it if found. If no match is found, the name defaults to the operating system name, such as Linux or Darwin.

.. code-block:: bash

  # for the machine detected or the current OS, default compiler, and default backend
  cmake ${TRITON_HOME}

  # for frontier machine, default compiler, and default backend
  cmake ${TRITON_HOME} -DMACHINE=frontier
   
If a user needs to use a custom machine file, set **MACHINE** to the path of that file.
In this case, the **COMPILER** and **BACKEND** command-line arguments will not be used
for machine file search. See :ref:`Machine Configuration File <machine_file>` to learn
how to create a custom machine file.

.. code-block:: bash

  # for custom machine file
  cmake ${TRITON_HOME} -DMACHINE=/my/custom/machine/file

``-DCOMPILER=<predefined-compiler-name>``
   This argument sets the name of compiler. The name of compiler is pre-defined at `<TRITON_HOME>/cmake/machines/<MACHINE>` directory. If this argument is omitted, the name is set `default`, whose actual compiler is determined by TRITON.
   
.. code-block:: bash

  # for gnu compiler and the machine detected or the current OS
  cmake ${TRITON_HOME} -DCOMPILER=gnu

``-DBACKEND=<predefined-backend-name>``
   This argument sets the name of backend. The name of backend is pre-defined at `<TRITON_HOME>/cmake/machines/<MACHINE>` directory. If this argument is omitted, the name is set `default`, whose actual backend is determined by TRITON.

.. code-block:: bash

  # for CUDA backend and the machine detected or the current OS
  cmake ${TRITON_HOME} -DBACKEND=CUDA

The pre-defined names are derived from Kokkos-supported backends including:

* CUDA
* HIP
* SYCL
* OPENMP
* OPENMPTARGET
* THREADS
* SERIAL

.. note::
    Not all compilers and backends are supported on pre-defined systems. See the machine-files in `<TRITON_HOME>/cmake/machines/<MACHINE>` directory to see what compilers and backends are supported..

.. note::
    The filename of the machine-files in `<TRITON_HOME>/cmake/machines/<MACHINE>` directory has the format of `<COMPILER>_<BACKEND>.sh`.
   
``-DARCH=<predefined-hardware-architecture-name>``
   This argument sets the CPU and GPU architecture, following the names defined in Kokkos. For example, ORNL’s Frontier GPU system uses ``AMD_GFX90A``.

``-DRUN_COMMAND="<job-lanuch-command>"``
   This argument sets the MPI job launcher and additional arguments such as ``srun -n 4`` for specifying the number of compute processes.

``-DCOMPILER_FLAGS="<cxx-compiler-flags>"``
   This argument overwrites C++ compiler options.

``-DCOMPILER_FLAGS_APPEND="<cxx-compiler-flags>"``
   This argument appends additional C++ compiler options to those specified in ``COMPILER_FLAGS``.

``-DLINKER_FLAGS="<linker-flags>"``
   This argument overwrites linker options.

``-DLINKER_FLAGS_APPEND="<linker-flags>"``
   This argument appends additional linker options to those specified in ``LINKER_FLAGS``.

``-DDEBUG=<ON|OFF>``
   This argument enables TRITON debugging feature.

``-DBUILD_TESTS=<ON|OFF>``
   This argument enables TRITON ctest feature. See the following ``Building TRITON CTest Cases`` section for details.

``-DENSEMBLE_BUILD<ON|OFF>``
   This argument enables TRITON ensemble build. See the :ref:`Running TRITON in Ensemble Mode <ensemble_run>` for more details.

Using Environmental Variables
-----------------------------------------

User can override the CMake variables defined in the selected machine file using environmetal variables.
This feature could be convinient to change some CMake variables defined in a machine file temporarly.
For example, user may turn on debugging feature temporary by running the command before running TRITON:

.. code-block:: bash

    export TRITON_DEBUG=ON

Note that **TRITON_** is prepended to **DEBUG** CMake command-line option. This rule applies to
all CMake command-line options explained above.

There are three methods for users to control TRITON compilation and execution: 1) machine files, 2) environment variables, and 3) CMake command-line options. The latter overrides the previous methods, giving CMake command-line settings the highest priority.

CMake Command-line Examples
-----------------------------------------

The following examples illustrate how to invoke CMake with different options:

.. code-block:: bash

   # assuming a Linux system

   # create and change to the TRITON build directory
   mkdir build
   cd build
   
   # the default compiler and backend are selected for the host OS or the detected machine
   # search for Linux/default_default.sh if no machine is detected
   cmake ..
   
   # the default compiler and backend are selected for the Frontier system
   # search for frontier/default_default.sh
   cmake .. -DMACHINE=frontier
   
   # the Cray compiler and default backend are selected for the host OS or the detected machine
   # search for Linux/cray_default.sh if no machine is detected
   cmake .. -DCOMPILER=cray
   
   # the default compiler and HIP backend are selected for the host OS or the detected machine
   # search for Linux/default_HIP.sh if no machine is detected
   cmake .. -DBACKEND=HIP
   
   # the default compiler and HIP backend are selected for the Frontier system
   # search for frontier/default_HIP.sh
   cmake .. -DMACHINE=frontier -DBACKEND=HIP
   
   # the Cray compiler and HIP backend are selected for the Frontier system
   # search for frontier/cray_HIP.sh
   cmake .. -DMACHINE=frontier -DCOMPILER=cray -DBACKEND=HIP


Building TRITON CTest Cases
--------------------------------------------

You can enable optional components such as the TRITON test cases by specifying additional CMake arguments during configuration.

`-DBUILD_TESTS=ON`

When this argument is set, TRITON will generate CTest test cases in the build directory. These tests can be executed to verify the installation and ensure that the model runs correctly on your system.

Example:

.. code-block:: bash

   cmake .. -DBUILD_TESTS=ON

After configuring with `-DBUILD_TESTS=ON` and building `triton.exe` using `triton_build.sh`, you can run all tests in the build directory with:

.. code-block:: bash

   ./triton_ctest.sh

The following is an example of the test summary output displayed on the screen when running `triton_ctest.sh`.

.. code-block:: bash

    Test project /home/triton/build
        Start 1: PARABOLOID
    1/6 Test #1: PARABOLOID .......................   Passed    5.58 sec
        Start 2: BIT4BIT-PARABOLOID
    2/6 Test #2: BIT4BIT-PARABOLOID ...............   Passed    0.37 sec
        Start 3: CIRCULAR-DAMBREAK
    3/6 Test #3: CIRCULAR-DAMBREAK ................   Passed    1.95 sec
        Start 4: BIT4BIT-CIRCULAR-DAMBREAK
    4/6 Test #4: BIT4BIT-CIRCULAR-DAMBREAK ........   Passed    0.54 sec
        Start 5: ALLATOONA
    5/6 Test #5: ALLATOONA ........................   Passed    2.48 sec
        Start 6: BIT4BIT-ALLATOONA
    6/6 Test #6: BIT4BIT-ALLATOONA ................   Passed    1.33 sec

    100% tests passed, 0 tests failed out of 6

    Total Test time (real) =  12.29 sec

If any tests fail, please check ``Testing/Temporary/LastTest.log`` in the build directory for detailed output.
