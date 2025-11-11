.. _triton_run:

Advanced User Configuration: Parallel Input
-------------------------------------------

For very large domains (> 5 billion grid cells), inputs must be **split** for 
parallel use. TRITON provides the ``scriptSplitASCII`` tool for preprocessing.

**Step 1. Configure ``scriptSplitASCII``**

Open the script and set the following parameters:

- **TRITON_DIR**: TRITON installation directory  
- **NFILES**: number of splits (**must equal MPI ranks**)  
- **INPUT_DEM**: DEM input file  
- **IS_MANN**: ``YES`` or ``NO`` (whether a Manning file is provided)  
- **INPUT_MANN**: Manning input file (required if ``IS_MANN=YES``)  
- **IS_RMAP**: ``YES`` or ``NO`` (whether a runoff map is provided)  
- **INPUT_RMAP**: runoff map input (required if ``IS_RMAP=YES``)  
- **OUTPUT_FORMAT**: ``ASC`` or ``BIN``  
- **ASCII2BIN_FOLDER**: folder for ``ascii2bin`` output (if ``BIN``)  
- **ASCII2BIN_RMAP_FOLDER**: folder for ``ascii2bin_rmap`` output (if ``BIN`` + RMAP)  

**Step 2. Run the script.**

**Step 3. Update the configuration file**

Example:

.. code-block:: text

   dem_filename="input/dem/bin/par/case03"
   header_filename="input/dem/bin/par/case03.header"
   n_infile="input/mann/bin/par/case03"
   runoff_map="input/runoff/bin/par/case03_runoff"
   input_format=BIN
   input_option=PAR

**Step 4. Run TRITON**

Use the same number of MPI ranks as ``NFILES`` when launching TRITON.  

.. important::
   Splitting input files is only required for **very large cases** that cannot 
   be handled as single rasters. For typical domains, standard (non-split) inputs 
   are sufficient.
