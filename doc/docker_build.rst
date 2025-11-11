.. _docker_build:

Building Docker Images
========================

The **Dockerfile** in the top-level TRITON directory provides an example of a TRITON Docker image.

To build a TRITON Docker image from the **Dockerfile**, run the following command:

.. code-block:: bash
   
    docker build -t triton:v1 .

This image is a simple example that supports CPU simulations using the MPICH MPI library.
