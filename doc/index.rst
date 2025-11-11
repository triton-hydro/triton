Welcome to TRITON
=================

The **Two-dimensional Runoff Inundation Toolkit for Operational Needs (TRITON)** is an open-source, computationally efficient two-dimensional flood modeling toolkit that scales from a laptop to supercomputers. It solves the full shallow-water equations on CPUs and GPUs to produce fast, reproducible flood-inundation maps.

.. note::

   Key reference: `Morales-Hernández et al., 2021 <https://doi.org/10.1016/j.envsoft.2021.105034>`_,  
   *Environmental Modelling & Software*.  
   See also :doc:`papers` for a full list of publications.


Why TRITON
==========

Use TRITON for fast and accurate simulation of flood wave propagation and surface inundation.

.. grid:: 2
   :gutter: 2

   .. grid-item::
      :columns: 6

      - **Open-source, Physics-based hydrodynamic flood model**
      - **Solves the full 2D shallow-water equations**
      - **Accelerated on modern CPUs and GPUs, including multi-GPU systems**
      - **Scales from a single laptop to leadership-class supercomputers**
      - **Captures backwater effects and both fluvial (riverine) and pluvial (flash flood) events**

   .. grid-item::
      :columns: 6

      .. only:: html

         .. raw:: html

            <video class="doc-anim" src="_static/trimmed.mp4"
                   autoplay loop muted playsinline controls
                   style="max-width:100%; height:auto;" title="Example TRITON flood-inundation simulation"></video>
            <p style="font-size: 0.9em; text-align: center; margin-top: 0.5em;">
            Example simulation of flood-inundation dynamics.
            </p>

      .. only:: latex

         .. image:: _static/triton_video_placeholder.png
            :alt: TRITON animation (see HTML version for video)

         Example simulation of flood-inundation dynamics.


Quick Links
-----------

.. grid:: 5
   :gutter: 2

   .. grid-item-card:: Get Started
      :link: introduction
      :link-type: doc
      :text-align: center

      Learn more about TRITON.

   .. grid-item-card:: Build & Install
      :link: installation
      :link-type: doc
      :text-align: center

      Build from source or use prebuilt containers.

   .. grid-item-card:: Examples & Case Studies
      :link: casestudy
      :link-type: doc
      :text-align: center

      Explore test cases and real-world flood simulations.

   .. grid-item-card:: Publications
      :link: papers
      :link-type: doc
      :text-align: center

      Read research studies and applications.

   .. grid-item-card:: Support
      :link: https://triton.ornl.gov/contact/
      :text-align: center

      Get help or report an issue.


Showcase
========

TRITON supports a wide range of flood modeling applications. Below are a few representative examples.

.. grid:: 2
   :gutter: 2

   .. grid-item-card:: Large-scale flooding
      :text-align: center

      .. image:: _static/Missouri.png
         :alt: Large-scale flood extent
         :class: doc-hero

      Basin- and watershed-scale flood simulations.

   .. grid-item-card:: Inundation forecasting
      :text-align: center

      .. image:: _static/triton_5.png
         :alt: Flood forecast animation
         :class: doc-hero

      Short-term flood forecasts for emergency response.

   .. grid-item-card:: Probabilistic inundation mapping
      :text-align: center

      .. image:: _static/probabilistic.png
         :alt: Probabilistic flood hazard mapping
         :class: doc-hero

      Ensemble-based hazard maps.

   .. grid-item-card:: Dam-break simulations
      :text-align: center

      .. image:: _static/taum_sauk.png
         :alt: Dam break flood wave
         :class: doc-hero

      High-resolution dam-break simulations.


Project Website
===============

`TRITON <https://triton.ornl.gov>`_


.. toctree::
   :maxdepth: 1
   :caption: Getting Started
   :hidden:

   introduction
   installation
   releases
   cmake_arguments
   


.. toctree::
   :maxdepth: 1
   :caption: Configure
   :hidden:

   simulation_setup
   configuration_reference
   configuration_variable_index
   triton_run
   docker_run
   ensemble_run
   casestudy

.. toctree::
   :maxdepth: 1
   :caption: Developer and API
   :hidden:

   docguide
   docker_build
   machine_file

.. toctree::
   :maxdepth: 1
   :caption: Resources
   :hidden:

   papers
   contact
