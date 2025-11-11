.. _configuration_variable_index:

Configuration Variable Index
============================

.. note::
   For a full walkthrough of each configuration block with examples,
   see :ref:`configuration_reference`.

Short, alphabetized reference for all configuration variables. Each row links to the section where it is explained.

----------------------------

.. index::
   pair: configuration; checkpoint_id
   pair: configuration; const_mann
   pair: configuration; courant
   pair: configuration; dem_filename
   pair: configuration; domain_decomposition
   pair: configuration; extbc_dir
   pair: configuration; extbc_file
   pair: configuration; factor_interval_domain_decomposition
   pair: configuration; gpu_direct_flag
   pair: configuration; h_infile
   pair: configuration; hextra
   pair: configuration; hydrograph_filename
   pair: configuration; input_format
   pair: configuration; it_count
   pair: configuration; it_print
   pair: configuration; n_infile
   pair: configuration; num_extbc
   pair: configuration; num_runoffs
   pair: configuration; num_sources
   pair: configuration; observation_loc_file
   pair: configuration; open_boundaries
   pair: configuration; outfile_pattern
   pair: configuration; output_format
   pair: configuration; output_option
   pair: configuration; print_interval
   pair: configuration; print_observation
   pair: configuration; print_option
   pair: configuration; projection
   pair: configuration; qx_infile
   pair: configuration; qy_infile
   pair: configuration; runoff_filename
   pair: configuration; runoff_map
   pair: configuration; sim_duration
   pair: configuration; sim_start_time
   pair: configuration; src_loc_file
   pair: configuration; time_increment_fixed
   pair: configuration; time_series_flag
   pair: configuration; time_step

.. list-table:: Configuration variables (A–Z)
   :header-rows: 1
   :widths: 22 18 60
   :align: left
   :class: sd-table

   * - Variable
     - Section
     - Details
   * - checkpoint_id
     - :ref:`simulation_control`
     - Restart index. 0 means fresh start, greater than 0 restarts from that checkpoint.
   * - const_mann
     - :ref:`surface_roughness`
     - Constant Manning’s n for the entire domain if no raster is provided.
   * - courant
     - :ref:`misc_params`
     - CFL number. Keep at or below 0.5.
   * - dem_filename
     - :ref:`topography`
     - Path to the DEM raster that defines the grid for all other rasters.
   * - domain_decomposition
     - :ref:`misc_params`
     - Partitioning mode for parallel runs: ``static`` or ``dynamic``.
   * - extbc_dir
     - :ref:`external_boundaries`
     - Optional directory containing files referenced by ``extbc_file``.
   * - extbc_file
     - :ref:`external_boundaries`
     - Table of external boundary segments and parameters.
   * - factor_interval_domain_decomposition
     - :ref:`misc_params`
     - Update frequency used when domain decomposition is ``dynamic``.
   * - gpu_direct_flag
     - :ref:`misc_params`
     - CUDA aware MPI toggle. 0 off, 1 on.
   * - h_infile
     - :ref:`initial_conditions`
     - Initial water depth raster. Optional.
   * - hextra
     - :ref:`misc_params`
     - Depth tolerance in meters below which velocities are set to zero.
   * - hydrograph_filename
     - :ref:`hydrologic_forcing`
     - Streamflow hydrographs. First column is time in hours, others are discharges in m³/s.
   * - input_format
     - :ref:`io_formats`
     - Input raster format: ASC or BIN.
   * - it_count
     - :ref:`misc_params`
     - Internal counter, usually left at 0.
   * - it_print
     - :ref:`output_control`
     - Iteration interval for diagnostic log messages.
   * - n_infile
     - :ref:`surface_roughness`
     - Raster of Manning’s n values aligned with the DEM.
   * - num_extbc
     - :ref:`external_boundaries`
     - Number of external boundary segments.
   * - num_runoffs
     - :ref:`hydrologic_forcing`
     - Number of runoff zones in the domain.
   * - num_sources
     - :ref:`hydrologic_forcing`
     - Number of streamflow inflow points.
   * - observation_loc_file
     - :ref:`output_control`
     - XY locations for time series outputs, in projected meters.
   * - open_boundaries
     - :ref:`misc_params`
     - Global switch to open domain edges; ignored when explicit boundaries are defined.
   * - outfile_pattern
     - :ref:`io_formats`
     - Naming convention for output files.
   * - output_format
     - :ref:`io_formats`
     - Output raster format: ASC, BIN, or GTIFF.
   * - output_option
     - :ref:`io_formats`
     - Parallel output mode: ``SEQ`` single files, ``PAR`` per subdomain.
   * - print_interval
     - :ref:`output_control`
     - Time in seconds between raster outputs.
   * - print_observation
     - :ref:`output_control`
     - Switch to write observation outputs.
   * - print_option
     - :ref:`output_control`
     - Which raster fields to output: ``h`` or ``huv``.
   * - projection
     - :ref:`io_formats`
     - EPSG or WKT used only when writing GTIFF outputs.
   * - qx_infile
     - :ref:`initial_conditions`
     - Initial x discharge raster. Optional.
   * - qy_infile
     - :ref:`initial_conditions`
     - Initial y discharge raster. Optional.
   * - runoff_filename
     - :ref:`hydrologic_forcing`
     - Runoff hydrographs. First column is time in hours, others are mm/hr per zone.
   * - runoff_map
     - :ref:`hydrologic_forcing`
     - Raster of runoff zone IDs aligned with the DEM.
   * - sim_duration
     - :ref:`simulation_control`
     - Total simulation length in seconds.
   * - sim_start_time
     - :ref:`simulation_control`
     - Start time in seconds.
   * - src_loc_file
     - :ref:`hydrologic_forcing`
     - XY coordinates for inflow sources that match hydrograph column order.
   * - time_increment_fixed
     - :ref:`simulation_control`
     - 0 uses adaptive timestep, 1 uses fixed timestep.
   * - time_series_flag
     - :ref:`output_control`
     - 1 enables time series outputs at observation points.
   * - time_step
     - :ref:`simulation_control`
     - Fixed timestep in seconds used when ``time_increment_fixed = 1``.
