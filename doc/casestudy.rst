.. _casestudy:

TRITON Case Studies
=====================

TRITON Version 1
----------------

Version 1 (`v1.final <https://code.ornl.gov/hydro/triton/-/tree/v1.final?ref_type=tags>`_) 
distributed a set of benchmark and real-world test cases, described in detail by 
:ref:`Morales-Hernández et al. (2021) <morales2021>`:

- **Case 01: TaumSauk** — historical dam-break problem.
- **Case 02a–02d: Paraboloid** — idealized benchmarks at multiple resolutions  
  (0.04 m, 0.02 m, 0.01 m, 0.005 m) for stability, balancing, and convergence  
- **Case 03: Runoff** — watershed scale application demonstrating distributed runoff inputs, inflow hydrographs, and external boundaries  
- **Case 04–05: Hurricane Harvey** — a large-scale real-event inundation simulations  
  at 30 m and 10 m resolution for Hurriane Harvey, Texas in 2017.  

Together, these cases demonstrated TRITON’s ability to handle both controlled 
benchmarks and complex real-world floods, showcasing stability, convergence, 
robustness, and scalability across diverse conditions.


TRITON Version 2
----------------

In Version 2, the test cases have been reorganized for clarity and to reduce the 
size of the main repository. Instead of bundling everything together, the core 
repository now highlights three representative examples:

- **allatoona** — corresponds to Case 03 (*Runoff*) from Version 1, illustrating 
  TRITON’s ability to capture fluvial and pluvial floods. 
- **paraboloid** — corresponds to Case 02a from Version 1; the higher-resolution 
  paraboloid suite (02a–02d) is now provided in the companion `triton_testcases` repo  
- **circular** — a new dam-break benchmark introduced in Version 2 (detailed 
  description forthcoming)  

Additional large and data-intensive cases are distributed via the 
`triton_testcases` repository, including:

- **paraboloid_suite** — Case 02a–02d from Version 1
- **harvey_suite** — Cases 04 and 05 from Version 1
- **centralTN** — Setup for Central TN 2021 flooding described in :ref:`Gangrade et al. (2023) <gangrade2023>`  

All of these can be accessed from the companion repository:

`https://code.ornl.gov/hydro/triton_testcases.git <https://code.ornl.gov/hydro/triton_testcases.git>`_



Case Mapping from Version 1 to Version 2
----------------------------------------

The table below shows how the Version 1 test cases correspond to Version 2, and 
whether they are provided in the main TRITON repository or in the companion 
`triton_testcases` repository.

.. list-table::
   :header-rows: 1
   :widths: 20 20 20 40

   * - Version 1
     - Version 2
     - Location
     - Notes
   * - Case 01: TaumSauk
     - —
     - Version 1 release
     - Historical dam-break benchmark; included in v1 only
   * - Case 02a–02d: Paraboloid
     - paraboloid
     - main repo; `triton_testcases/paraboloid_suite/`
     - Base paraboloid in main repo; extended resolution variants in `paraboloid_suite/`
   * - Case 03: Runoff
     - allatoona
     - main repo
     - Watershed-scale runoff capability demonstration
   * - Case 04: Harvey 30 m
     - —
     - `triton_testcases/harvey_suite/`
     - Hurricane Harvey flood simulation at 30 m resolution
   * - Case 05: Harvey 10 m
     - —
     - `triton_testcases/harvey_suite/`
     - Hurricane Harvey flood simulation at 10 m resolution
   * - —
     - circular
     - main repo
     - New dam-break benchmark introduced in Version 2
   * - —
     - Central Tennessee
     - `triton_testcases/centralTN/`
     - Regional-scale flood event; described in :ref:`Gangrade et al. (2023) <gangrade2023>`


Case Descriptions
-----------------

**Allatoona**  
Formerly Case 03 (*Runoff*) in Version 1, this case represents a small watershed 
in Georgia, USA. It demonstrates TRITON’s **runoff-driven modeling capability**, 
where distributed runoff inputs are combined with streamflow sources.  
For new users, Allatoona is a recommended starting point to understand TRITON’s 
basic configuration and outputs.  
See :ref:`Morales-Hernández et al. (2021) <morales2021>` for details.

**Paraboloid**  
Based on the Version 1 paraboloid suite (Cases 02a–02d), this benchmark evaluates 
**numerical stability, wet/dry boundary handling, and well-balanced flux–source 
treatment**. The setup consists of a frictionless paraboloid basin with a rotating 
velocity field. After one period, the solution should return to its initial state.  
The base paraboloid case is provided in the main repo, while the extended resolution 
variants are available in `triton_testcases/paraboloid_suite/`.  
Further description is in :ref:`Morales-Hernández et al. (2021) <morales2021>`.

**Circular (Dam Break)**  
A new benchmark introduced in Version 2. This case is designed to evaluate TRITON’s 
ability to handle abrupt hydraulic transitions in a simplified circular geometry.  
*Detailed documentation is forthcoming.*

**Central Tennessee Flood**  
A new regional-scale case study representing the devastating 2021 Central Tennessee 
flood event. It demonstrates TRITON’s **scalability to large, data-intensive domains** 
and its ability to integrate complex hydrologic forcing.  
The setup is available in `triton_testcases/centralTN/`.  
See :ref:`Gangrade et al. (2023) <gangrade2023>` for a full description.


Relevant Papers
---------------

For a complete list of TRITON-related publications, see :doc:`papers`.
