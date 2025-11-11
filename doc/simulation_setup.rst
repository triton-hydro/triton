.. _simulation_setup:

TRITON Setup
============

Project Layout
--------------

The TRITON source tree contains the following top-level directories and files:

.. code-block:: text

   triton/
   ├── doc/
   ├── src/
   ├── external/
   ├── input/
   │   ├── allatoona/
   │   └── ...
   ├── test/
   ├── cmake/
   ├── Makefile
   └── README.md

Key directories:

- **input/** – Case data and sample configuration files (e.g., ``allatoona.cfg``).  
- **src/** – Core TRITON simulation source code.  
- **doc/** – Documentation and API reference.  
- **external/** – Third-party dependencies (e.g., Kokkos).  
- **test/** – Regression test suite.  
- **cmake/** – Build modules and machine files.  
- **Makefile / README.md** – Build helpers and project overview.  

.. tip::

   For running your own cases, focus on the **input/** directory.  
   Other folders (``src/``, ``test/``, ``cmake/``) are primarily for developers.


Input File Types
----------------

TRITON uses two main categories of input data:

- **Raster grids** – spatial data for the computational domain  
  (e.g., DEM, Manning’s roughness, runoff zones).  
  Formats: Esri ASCII (``.asc``) or TRITON binary (``.bin``).  

- **Tabular text files** – plain text time series or coordinate lists  
  (e.g., hydrographs, inflow locations, boundary conditions).  
  Common extensions: ``.hyg``, ``.src``, ``.extbc``, ``.txt``.  

The table below summarizes supported input files and their purpose:

+-------------------------+----------------------------+-----------------------------------------------+
| File Type               | Formats / Extensions       | Purpose                                       |
+=========================+============================+===============================================+
| Digital Elevation Model | ``.asc``, ``.bin`` (DEM)   | Defines topography of the domain              |
+-------------------------+----------------------------+-----------------------------------------------+
| Manning’s n map         | ``.asc``, ``.bin`` (MANN)  | Surface roughness coefficients per grid cell  |
+-------------------------+----------------------------+-----------------------------------------------+
| Runoff map              | ``.asc``, ``.bin`` (RMAP)  | Partition of domain into runoff areas         |
+-------------------------+----------------------------+-----------------------------------------------+
| Streamflow hydrograph   | ``.hyg``                   | Time–discharge input at inflow points         |
+-------------------------+----------------------------+-----------------------------------------------+
| Runoff hydrograph       | ``.hyg``                   | Time–runoff rate per runoff area              |
+-------------------------+----------------------------+-----------------------------------------------+
| Inflow locations        | ``.src``                   | Cartesian coordinates of inflow sources       |
+-------------------------+----------------------------+-----------------------------------------------+
| External boundaries     | ``.extbc``                 | Defines open/closed boundaries and conditions |
+-------------------------+----------------------------+-----------------------------------------------+
| Observation locations   | ``.txt``                   | Points where hydrographs are extracted        |
+-------------------------+----------------------------+-----------------------------------------------+

.. important::
   All raster files must share the same **extent, resolution, and grid alignment**
   as the DEM. Any mismatch will cause TRITON to fail at startup.

.. note::
   The DEM (.asc) file requires the six header lines (refer to Topography section for more detail), while MANN and RMAP file should not include the header. 
   
.. note::
   TRITON can write outputs in **GeoTIFF (``.tif``)** format for easy GIS visualization,  
   but GeoTIFF is **not accepted as input**.


Input Overview (Visual)
-----------------------

.. image:: _static/input_overview.gif
   :alt: TRITON input overview
   :width: 100%


Data Standards
--------------

- Use a projected coordinate system.  
- SI units (meters, seconds, m³/s).  
