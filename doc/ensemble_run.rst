.. _ensemble_run:

Running TRITON in Ensemble Mode
==================================

The ensemble run feature allows users to execute multiple TRITON runs within a single job submission, saving considerable time and effort. This feature is based on **Ensify** (`https://github.com/mrnorman/Ensify <https://github.com/mrnorman/Ensify>`_). 

STEP 1: Build TRITON for Ensemble Run
---------------------------------------

To build TRITON for an ensemble run, add **-DENSEMBLE_BUILD=ON** to the CMake command line along with the other arguments shown below.

.. code-block:: bash

    cmake -DENSEMBLE_BUILD=ON <other cmake arguments>
    ./triton_build.sh 

STEP 2: Configure Ensemble Run
---------------------------------------

The TRITON ensemble configuration file is a YAML file used to define an ensemble run, using the syntax explained below.

.. code-block:: yaml

    # rank_beg , rank_end , arguments_list , stdout_filename , stderr_filename
    ensembles:  [
                    [ 0, 7, ["./ensemble1.cfg", ...] , "ens1.out" , "ens1.err" ],
                    [ 8, 16, ["./ensemble2.cfg", ...] , "ens2.out" , "ens2.err" ],
                    ...
                ]

Ensembles are defined as a list of ensemble entries. Each ensemble entry includes the following fields in order: rank_beg, rank_end, arguments_list, stdout_filename, and stderr_filename. In the example above, the first two entries specify the first entry for MPI ranks 0–7 and the second entry for MPI ranks 8–16, respectively.

See `Ensify ensemble input format <https://github.com/mrnorman/Ensify#enssify-ensemble-input-format>`_ for details.

STEP 3: Run TRITON in Ensemble Mode
---------------------------------------

Running TRITON in ensemble mode is the same as a regular run, except that the ensemble configuration file is the only argument to the triton.exe executable. The ensemble configuration file is the one defined in the previous step.

.. code-block:: bash

    source ./triton_env.sh
    <MPI command and arguments> ./triton.exe <ensemble configuration file>

As an example of an ensemble run, **triton_ensrun.sh** is created with a sample configuration for the Paraboloid case.
