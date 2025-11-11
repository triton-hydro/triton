.. _configuration_reference:

Configuration Reference
=======================

.. note::
   For a quick alphabetical lookup of all configuration parameters, 
   see :ref:`configuration_variable_index`.

A TRITON configuration file (``.cfg``) defines everything needed to launch a run:

* **Input data**: paths to DEMs, roughness maps, and hydrographs  
* **Numerical settings**: timestep control, CFL limit, tolerances  
* **Forcing and boundaries**: streamflow, runoff, and external conditions  
* **Outputs**: format, frequency, and file naming conventions  

Each case directory under ``input/`` contains its own configuration file.  
For example, the Allatoona case provides:

.. code-block:: text

   input/allatoona/allatoona.cfg

This file is a good starting template: adjust paths and parameters to match your own domain.

.. important::
   Unless an absolute path is given, all file paths in the configuration are
   resolved **relative to your working directory** (where you launch TRITON),
   not the location of the ``.cfg`` file itself.


.. _configuration_overview:

Configuration Overview
----------------------


The following sections describe each configuration block in detail, 
using ``input/allatoona/allatoona.cfg`` as a concrete example.
The configuration file specifies all aspects of a TRITON run, grouped into these blocks.

.. contents::
   :local:
   :depth: 2


.. _io_paths:

Input and Output Folders
~~~~~~~~~~~~~~~~~~~~~~~~

Two global parameters define the base locations for inputs and outputs.  
All other file paths in the configuration are interpreted relative to these folders.

.. index:: single: input_folder
.. index:: pair: configuration; input_folder

- **input_folder**: base directory for input files (DEMs, maps, hydrographs).

  Example (Allatoona):

  .. code-block:: text

     input_folder="input"

.. index:: single: output_folder
.. index:: pair: configuration; output_folder

- **output_folder**: directory where TRITON writes results.

  Example (Allatoona):

  .. code-block:: text

     output_folder="output"

.. important::
   Absolute paths should not be used for the input and output folder locations.  
   The locations of these folders are resolved **relative to the TRITON executable**.


.. _topography:

Topography
~~~~~~~~~~

The first and most important input is the **Digital Elevation Model (DEM)**, which 
defines the spatial extent and resolution of the simulation domain.

.. index:: single: dem_filename
.. index:: pair: configuration; dem_filename

- **dem_filename**: path to the Digital Elevation Model file.

  Example (Allatoona):

  .. code-block:: text

     dem_filename="input/allatoona/allatoona.dem"

DEM files can be provided in **Esri ASCII raster (ASC)** or **TRITON binary (BIN)**
format. Conversion utilities between ASCII and binary are provided in
the ``tools/`` directory. (#tosort).

**ASCII DEM format**

An ASCII DEM begins with a six-line header followed by the elevation matrix.  
For example, the Allatoona DEM starts with:

.. code-block:: text

   ncols         591
   nrows         673
   xllcorner     719559.01581497
   yllcorner     3765449.3800973
   cellsize      30
   NODATA_value  -9999
   305.2047 301.1886 297.5595 297.4224 302.5769 ...

- The six header lines define the grid dimensions, spatial reference, resolution, and NODATA value.  
- The matrix that follows contains elevations for ``nrows × ncols`` cells.  

.. important::
   - TRITON requires the DEM (and all other rasters such as Manning’s n and runoff maps) 
     to be in a **projected coordinate system** with units in **meters**.  
   - All rasters must have the **same grid size, extent, resolution, and alignment** 
     as the DEM. Any mismatch will cause TRITON to fail at startup.  

.. note::
   The NODATA field is required by the ESRI ASCII raster format. TRITON does not
   apply any special treatment to NODATA values — they are read literally as part
   of the matrix. If negative values such as ``-9999`` are present, they will be
   interpreted as very deep pits, which may destabilize a simulation.

.. _io_formats:

Input and Output Formats
~~~~~~~~~~~~~~~~~~~~~~~~

TRITON supports multiple file formats for inputs and outputs.  
The following parameters control how rasters are read and written.

.. index:: single: input_format
.. index:: pair: configuration; input_format

- **input_format**: format of input rasters.  
  Options:  
  - ``ASC``: ASCII grid (readable, easy to debug)  
  - ``BIN``: TRITON binary format (compact, faster I/O)  

  Example:

  .. code-block:: text

     input_format=ASC

.. index:: single: output_format
.. index:: pair: configuration; output_format

- **output_format**: format of output rasters.  
  Options:  
  - ``ASC``: ASCII grid  
  - ``BIN``: TRITON binary  
  - ``GTIFF``: GeoTIFF (GIS-ready, supports georeferencing)  

  Example:

  .. code-block:: text

     output_format=BIN

.. index:: single: projection
.. index:: pair: configuration; projection

- **projection** *(required if ``output_format=GTIFF``)*: EPSG code or WKT string
  used to embed spatial reference in GeoTIFF outputs. Ignored for ASC and BIN.

  Example:

  .. code-block:: text

     projection="EPSG:32614"

  .. note::
     Must be a valid EPSG code (e.g. ``EPSG:4326``) or a complete WKT definition.  
     If missing or invalid, GeoTIFF outputs will lack spatial reference.

.. index:: single: output_option
.. index:: pair: configuration; output_option

- **output_option**: how outputs are written in parallel runs.  
  Options:  
  - ``SEQ``: gather all subdomains into a single file per variable  
  - ``PAR``: write one file per subdomain (faster, especially on clusters)  

  Example:

  .. code-block:: text

     output_option=SEQ

  .. tip::
     With ``output_option=PAR`` and ``output_format=GTIFF``, TRITON also writes
     a ``.vrt`` (Virtual Raster) file. The ``.vrt`` references all tiles and
     appears as a seamless raster in GIS tools without duplicating data.

.. index:: single: outfile_pattern
.. index:: pair: configuration; outfile_pattern

- **outfile_pattern**: naming convention for output files. Placeholders are
  replaced by case name, variable, and timestep indices.  
  Normally left at default.

  Example:

  .. code-block:: text

     outfile_pattern="%s/%s/%s_%02d_%02d"

.. _hydrologic_forcing:

Hydrologic Forcing
~~~~~~~~~~~~~~~~~~

TRITON can be driven by **streamflow hydrographs** at specified inflow locations,
by **runoff hydrographs** over distributed zones, or by a combination of both.  
The Allatoona example demonstrates both types of inputs.

**Streamflow Hydrograph**

.. index:: single: num_sources
.. index:: pair: configuration; num_sources

- **num_sources**: number of inflow points (streamflow sources).  
  Must match the number of discharge columns in the hydrograph file.

  Example (Allatoona):

  .. code-block:: text

     num_sources=2

.. index:: single: hydrograph_filename
.. index:: pair: configuration; hydrograph_filename

- **hydrograph_filename**: path to the file containing streamflow hydrographs.

  Example (Allatoona):

  .. code-block:: text

     hydrograph_filename="input/allatoona/allatoona.hyg"

  File format:  
  - Column 1 = time (hours)  
  - Columns 2…N = discharge (m³/s), one per source  
  - Lines beginning with ``%`` are treated as comments and ignored

  Example file (``.hyg``):

  .. code-block:: text

     % Hydrograph
     % Time(hr) Q1(cms) Q2(cms)
     0,1.787598324,6.142665492
     3,2.966903878,9.077155463
     6,3.602809745,12.16067761

.. index:: single: src_loc_file
.. index:: pair: configuration; src_loc_file

- **src_loc_file**: file with the Cartesian coordinates of inflow locations.  
  Row order must match the column order in ``hydrograph_filename``.

  Example (Allatoona):

  .. code-block:: text

     src_loc_file="input/allatoona/allatoona.src"

  Example file (``.src``):

  .. code-block:: text

     % X  Y  (projected meters)
     735404.711,3780498.492
     736616.216,3776851.088

.. note::
   - Units: time in **hours**, discharge in **m³/s**.  
   - ``num_sources`` must equal the number of discharge columns in the hydrograph.  
   - Comment lines starting with ``%`` are allowed and ignored by the reader.

**Runoff Hydrograph**

.. index:: single: num_runoffs
.. index:: pair: configuration; num_runoffs

- **num_runoffs**: number of distinct runoff zones in the domain.  
  Must match the number of runoff columns in the hydrograph file.

  Example (Allatoona):

  .. code-block:: text

     num_runoffs=156

.. index:: single: runoff_filename
.. index:: pair: configuration; runoff_filename

- **runoff_filename**: path to the file containing runoff hydrographs.

  Example (Allatoona):

  .. code-block:: text

     runoff_filename="input/allatoona/allatoona.roff"

  File format:  
  - Column 1 = time (hours)  
  - Columns 2…N = runoff rates (mm/hour), one per zone  
  - Number of runoff columns must equal ``num_runoffs``  
  - Lines beginning with ``%`` are treated as comments and ignored  

  Example file (``.roff``):

  .. code-block:: text

     % Runoff Hydrograph
     % Time(h) Zone0(mm/hr) Zone1(mm/hr) ...
     0.0,0.0,0.0,0.0,...
     0.5,0.1,0.0,0.0,...
     1.0,0.3,0.1,0.0,...

.. index:: single: runoff_map
.. index:: pair: configuration; runoff_map

- **runoff_map**: raster assigning each grid cell to a runoff zone.  
  Zone IDs must be **integers starting from 0** up to ``num_runoffs-1``.  
  The map must align exactly with the DEM.

  Example (Allatoona):

  .. code-block:: text

     runoff_map="input/allatoona/allatoona.rmap"

.. important::
   - The runoff map must use the **same grid, resolution, and projection** as the DEM.  
   - Zone IDs in ``runoff_map`` must correspond exactly to the columns in the 
     runoff hydrograph file.  
   - Zones are numbered **starting at 0**.  
   - Units: time in **hours**, runoff in **mm/hour**.
   - Note that .asc file raster should not include header. 

.. note::
   Streamflow and runoff forcing can be used **independently** or **together**.  

   Examples:  
   - A dam-break test may use only streamflow hydrographs.  
   - A watershed-scale flood may combine distributed runoff with a few streamflow 
     sources.  

   TRITON superimposes all sources during simulation. Be careful to avoid 
   **double-counting inflows** if both streamflow and runoff are applied 
   in overlapping regions.


.. _surface_roughness:

Surface Roughness (Manning’s n)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TRITON accounts for surface roughness using Manning’s n coefficients.  
Roughness can be specified either as a **spatial raster** or as a **constant value**.

.. index:: single: n_infile
.. index:: pair: configuration; n_infile

- **n_infile**: path to a raster of Manning’s n values.  
  - Must have the same grid, extent, and projection as the DEM  
  - Supported formats: ASCII (``.asc``) or binary (``.bin``). Note that .asc file raster should not include header. 

  Example (Allatoona):

  .. code-block:: text

     n_infile="input/allatoona/allatoona.mann"

.. index:: single: const_mann
.. index:: pair: configuration; const_mann

- **const_mann**: constant Manning’s n value applied to the entire domain.  

  Example:

  .. code-block:: text

     const_mann=0.035

.. important::
   - If ``n_infile`` is provided, it overrides ``const_mann``.  
   - The Manning raster must align exactly with the DEM grid.  

.. _external_boundaries:

External Boundaries
~~~~~~~~~~~~~~~~~~~

By default, TRITON treats all domain edges as **closed**.  
To allow inflow/outflow, you must define external boundaries.

.. index:: single: num_extbc
.. index:: pair: configuration; num_extbc

- **num_extbc**: number of boundary segments.  
  Must equal the number of rows in ``extbc_file``.

  Example (Allatoona):

  .. code-block:: text

     num_extbc=2

.. index:: single: extbc_file
.. index:: pair: configuration; extbc_file

- **extbc_file**: file listing boundary segments.  

  Example (Allatoona):

  .. code-block:: text

     extbc_file="input/allatoona/allatoona.extbc"

.. index:: single: extbc_dir
.. index:: pair: configuration; extbc_dir

- **extbc_dir**: directory containing boundary-related files.  

  Example (Allatoona):

  .. code-block:: text

     extbc_dir="input/allatoona/"

**Boundary File Format**

Each row in ``extbc_file`` defines one boundary segment with:  

1. **Type (integer code)**  
2. **X1, Y1**: coordinates of the first endpoint (projected meters)  
3. **X2, Y2**: coordinates of the second endpoint (projected meters)  
4. **BC parameter(s)**: depends on boundary type  

Boundary type codes:

+------+--------------------------------------------+
| Code | Meaning                                    |
+======+============================================+
| 0    | Free flow (supercritical outflow)          |
+------+--------------------------------------------+
| 1    | Level vs. time (requires time–level file)  |
+------+--------------------------------------------+
| 2    | Normal slope (requires slope value)        |
+------+--------------------------------------------+
| 3    | Froude number (requires Froude value)      |
+------+--------------------------------------------+

Example (``"input/allatoona/allatoona.extbc"``):  

.. code-block:: text

   % BC Type, X1, Y1, X2, Y2, BC
   3,719569.048,3785624.114,723849.375,3785624.114,0.5
   3,719569.048,3785624.114,719569.048,3782029.877,0.5

- Here both boundaries are **Type 3 (Froude number)**, so the last column (``0.5``) 
  specifies the Froude value.  
- Coordinates are in meters, same projected CRS as the DEM.  

Example for a **Type 1** time–level file referenced by a boundary row:

.. code-block:: text

   % time (hr), water level (m)
   0.0,255.0
   1.0,255.2
   2.0,255.5


.. note::
   If explicit boundaries are defined (``num_extbc > 0``), they override the 
   global ``open_boundaries`` switch **for the specified segments only**.  
   Other domain edges remain subject to the global setting.

.. _simulation_control:

Simulation Control
~~~~~~~~~~~~~~~~~~

This block sets the simulation timing and timestep control.

.. index:: single: sim_start_time
.. index:: pair: configuration; sim_start_time

- **sim_start_time**: simulation start time (seconds).  

  Example:

  .. code-block:: text

     sim_start_time=0

.. index:: single: sim_duration
.. index:: pair: configuration; sim_duration

- **sim_duration**: total simulation length (seconds).  

  Example (Allatoona):

  .. code-block:: text

     sim_duration=432000   # 5 days

.. index:: single: checkpoint_id
.. index:: pair: configuration; checkpoint_id

- **checkpoint_id**: restart index.  
  Options:  
  - ``0`` = start fresh  
  - ``>0`` = restart from the specified checkpoint  

.. index:: single: time_increment_fixed
.. index:: pair: configuration; time_increment_fixed

- **time_increment_fixed**: timestep mode.  
  Options:  
  - ``0`` = adaptive timestep (default, CFL-controlled)  
  - ``1`` = fixed timestep (use ``time_step``)  

.. index:: single: time_step
.. index:: pair: configuration; time_step

- **time_step**: fixed timestep size (seconds).  
  Used **only if** ``time_increment_fixed=1``.

  Example:

  .. code-block:: text

     time_step=5.0

.. note::
   Adaptive timestepping (``time_increment_fixed=0``) is recommended for most cases.  

.. important::
   Stability is controlled by the **Courant number** (``courant`` in 
   :ref:`misc_params`).  
   Keep ``courant <= 0.5`` for reliable runs.

.. _output_control:

Output Control
~~~~~~~~~~~~~~

This block controls raster outputs, observation time series, and logging.

.. index:: single: print_option
.. index:: pair: configuration; print_option

- **print_option**: raster output fields.  
  Options:  
  - ``h`` = water depth only  
  - ``huv`` = water depth + discharges (qx, qy)  

  Example (Allatoona):

  .. code-block:: text

     print_option=h

.. index:: single: print_interval
.. index:: pair: configuration; print_interval

- **print_interval**: time interval between raster outputs (seconds).  

  Example:

  .. code-block:: text

     print_interval=1800   # every 30 minutes

.. index:: single: time_series_flag
.. index:: pair: configuration; time_series_flag

- **time_series_flag**: enable writing hydrograph time series at observation points.  
  Options:  
  - ``0`` = disabled  
  - ``1`` = enabled  

.. index:: single: observation_loc_file
.. index:: pair: configuration; observation_loc_file

- **observation_loc_file**: path to the file with observation point coordinates.  

  Example file:

  .. code-block:: text

     %X-Location,Y-Location
     727430.089,3772474.295

.. index:: single: print_observation
.. index:: pair: configuration; print_observation

- **print_observation**: interval (seconds) for writing time series at observation points.  
  - Independent of ``print_interval`` (raster outputs)  
  - Can be set smaller than ``print_interval`` to record hydrographs more frequently  
    without the cost of writing large raster files  

  Example:

  .. code-block:: text

     print_observation=300   # every 5 minutes



.. index:: single: it_print
.. index:: pair: configuration; it_print

- **it_print**: iteration interval for diagnostic log messages to console/log.  

  Example:

  .. code-block:: text

     it_print=100

.. note::
   - Raster outputs are controlled by ``print_option`` and ``print_interval``.  
   - Time series outputs require both ``time_series_flag=1`` **and** a valid 
     ``observation_loc_file``.  
   - ``print_observation`` sets the interval for time series output. Use a finer 
     interval (e.g., 5 min) to capture detailed hydrographs, while keeping 
     ``print_interval`` (rasters) coarser (e.g., 30–60 min) for efficiency.  
   - This balance avoids excessive raster output while still providing dense 
     time series data for analysis.  
   - Console/log verbosity is controlled by ``it_print``.

.. _initial_conditions:

Initial Conditions
~~~~~~~~~~~~~~~~~~

TRITON starts dry unless initial condition files are provided.

.. index:: single: h_infile
.. index:: pair: configuration; h_infile
.. index:: single: qx_infile
.. index:: pair: configuration; qx_infile
.. index:: single: qy_infile
.. index:: pair: configuration; qy_infile

- **h_infile**: initial depth raster  
- **qx_infile**: initial discharge (x)  
- **qy_infile**: initial discharge (y)  

Example:

.. code-block:: text

   h_infile="input/allatoona/allatoona.inith"
   qx_infile="input/allatoona/allatoona.initqx"
   qy_infile="input/allatoona/allatoona.inityq"

In Allatoona these are empty, so it starts dry.

.. note::
   These rasters must match the DEM grid. Typically used for hot start or after spinup.


.. _misc_params:

Miscellaneous Parameters
~~~~~~~~~~~~~~~~~~~~~~~~

Advanced settings for stability and parallel performance.

.. index:: single: courant
.. index:: pair: configuration; courant

- **courant**: Courant–Friedrichs–Lewy (CFL) number controlling stability.  
  Recommended ≤ 0.5.  

  Example:

  .. code-block:: text

     courant=0.5

.. index:: single: hextra
.. index:: pair: configuration; hextra

- **hextra**: depth tolerance (meters).  
  Below this threshold, velocities are set to zero to avoid instabilities.

  Example:

  .. code-block:: text

     hextra=0.001

.. index:: single: gpu_direct_flag
.. index:: pair: configuration; gpu_direct_flag

- **gpu_direct_flag**: GPU-aware MPI communication.  
  Options:  
  - ``0`` = off (safe default)  
  - ``1`` = on (use only with CUDA-aware MPI builds)  

.. index:: single: domain_decomposition
.. index:: pair: configuration; domain_decomposition

- **domain_decomposition**: domain partitioning mode.  
  Options:  
  - ``static`` = fixed partitioning (default)  
  - ``dynamic`` = partitions updated periodically  

.. index:: single: factor_interval_domain_decomposition
.. index:: pair: configuration; factor_interval_domain_decomposition

- **factor_interval_domain_decomposition**: update frequency for ``dynamic`` 
  domain decomposition (integer factor of ``print_interval``).

  Example:

  .. code-block:: text

     factor_interval_domain_decomposition=10

.. index:: single: open_boundaries
.. index:: pair: configuration; open_boundaries

- **open_boundaries**: global switch for external boundary conditions.  
  Options:  
  - ``0`` = closed boundaries (default)  
  - ``1`` = all domain edges open  

  .. note::
     If explicit external boundaries are defined (``num_extbc > 0``), they override 
     this global switch **for the specified segments only**. Other edges remain 
     subject to the global setting.

.. index:: single: it_count
.. index:: pair: configuration; it_count

- **it_count**: internal iteration counter.  
  Normally left at ``0`` (used internally for restarts/checkpoints).

.. _quick_sanity_checks:

Quick Sanity Checks
-------------------

- DEM, Manning, and runoff map share the same grid and CRS.  
- Input rasters are ASC/BIN. If outputs are GTIFF, set ``projection="EPSG:xxxx"``.  
- Hydrograph columns = ``num_sources``; runoff columns = ``num_runoffs``.  
- Inflow order matches hydrograph column order.  
- Boundary count = ``num_extbc``; coords in projected meters.  
- ``courant <= 0.5``; adaptive timestep recommended.  


.. note::
   Unless an absolute path is given, all file paths in the configuration are
   resolved relative to the directory where you launch the run (your working
   directory), not necessarily the location of the ``.cfg`` file.
