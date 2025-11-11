.. _machine_file:

Machine Configuration File
============================

A machine configuration file includes machine-specific settings used during TRITON build and execution.

TRITON’s build system searches the machine configuration file based on the ``MACHINE``, ``COMPILER``, and ``BACKEND`` CMake command-line arguments. If one or more of the arguments are omitted, TRITON uses default settings.

TRITON machine configuration files are located in ``<TRITON top directory>/cmake/machines/<MACHINE>`` and follow the filename syntax:
``COMPILER_BACKEND.sh``

Machine Configuration Variables
----------------------------------

All machine configuration variables are derived from the :ref:`CMake Command-line Arguments <cmake_arguments>`.

The only difference is that all machine configuration variables are prefixed with **TRITON_**, and their values have lower priority than the CMake command-line arguments.

For example, if **TRITON_COMPILER_FLAGS** in the machine configuration file is set to **-O2** and the corresponding **COMPILER_FLAGS** in the CMake command-line arguments is set to **-O3**, then the final value used for C++ compilation will be **-O3**.

The following list shows the available machine configuration variables. See :ref:`CMake Command-line Arguments <cmake_arguments>` for details.

* ``TRITON_COMPILER``
* ``TRITON_BACKEND``
* ``TRITON_ARCH``
* ``TRITON_RUN_COMMAND``
* ``TRITON_COMPILER``
* ``TRITON_COMPILER_FLAGS``
* ``TRITON_COMPILER_FLAGS_APPEND``
* ``TRITON_LINKER_FLAGS``
* ``TRITON_LINKER_FLAGS_APPEND``
* ``TRITON_DEBUG``
* ``TRITON_BUILD_TESTS``
* ``TRITON_ENSEMBLE_BUILD``

**Example Machine Configuration File**

.. code-block:: bash
   
   #!/bin/bash
   
   export TRITON_BACKEND=SERIAL
   export TRITON_COMPILER=mpic++
   export TRITON_RUN_COMMAND="mpirun -n 8"

.. note::
   use **export** in bash or similar in other shell to elevate a local variable
   to an environment variable


Using Environment Variables and Configuration Hierarchy
---------------------------------------------------------

User can set these variables directly in  environment and overrides the values specified in a machine file. For example, to specify ``COMPILER_FLAGS`` via an environment variable:

.. code-block:: bash

   # In a shell
   export TRITON_COMPILER_FLAGS="-O3"

TRITON applies configuration settings using the following priority order (from highest to lowest):

1. Values provided directly in CMake command-line arguments
2. Environment variables (e.g., ``TRITON_COMPILER_FLAGS``)
3. Machine configuration files

This hierarchy ensures that explicit user input overrides environment settings and defaults.


Creating Custom Machine Configuration File
---------------------------------------------------------

Users may need to create their own machine configuration files if the pre-configured files do not support their system.

A TRITON machine configuration file is a shell script, where you can include any valid shell commands along with TRITON variables. See other sections of this page and the :ref:`CMake Command-line Arguments <cmake_arguments>` for using TRITON variables.

One way to simplify creating a machine file is to copy an existing file to your system and modify it according to your needs.

Once created, you can provide the TRITON build system with the path to the machine file using the CMake command-line argument:
**`-DMACHINE="/path/to/machinefile"`**

If the machine file is placed in `<TRITON_DIR>/cmake/machines/<MACHINE>` with the filename format explained on this page, it can be used with the CMake command-line arguments **MACHINE**, **COMPILER**, and **BACKEND**.
