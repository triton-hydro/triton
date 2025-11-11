.. _docker_run:

Running TRITON using Docker
==================================

Docker images can be run on most systems, including Linux, macOS, and Windows. To do so, the host machine must have Docker Engine (Docker) installed. This section provides step-by-step instructions on running TRITON using Docker for all supported operating systems. 

TRITON Docker image can be downloaded from Docker Hub using the following command:

.. code-block:: bash

    docker pull grnydawn/triton-mpich  

The Docker image is configured to run simple CPU simulations inside a Docker container, without GPU support.

The following example runs a pre-configured Paraboloid simulation:

.. code-block:: bash

    mkdir -p output  

    docker run --rm \
        -v "./output:/app/triton/build/output" \
        grnydawn/triton-mpich triton_run.sh ./input/paraboloid/paraboloid.cfg  

**Explanation of options:**

* `--rm` : Automatically removes the container after it exits.
* `-v` : Mounts a host directory (`./output`) into the container at `/app/triton/build/output`.
* `grnydawn/triton-mpich` : The Docker image being used.
* `triton_run.sh` : A script inside the container.
* `./input/paraboloid/paraboloid.cfg` : Argument passed to the script, a configuration file for the Triton executable.

Specifying a configuration file
--------------------------------
 
To use a configuration file, and input data, in a host, it is required to mount the host directory  into
container.

Prepare input data and a configuration file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create a host directory and copy data and configuration file in the directory.

.. code-block:: bash

    mkdir -p host_input  

    cp -rf /my/input/data/* host_input
    cp -rf /my/config/file.cfg host_input

Mount the host directory into the container
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Add the `-v` option to `docker run` to mount a host input directory into the container.

.. code-block:: bash

    docker run --rm \
        -v "./output:/app/triton/build/output" \
        -v "./host_input:/app/triton/build/myinput" \
        grnydawn/triton-mpich triton_run.sh ./myinput/file.cfg

Note that the `host_input` directory on the host is mounted into `/app/triton/build/myinput` inside the container, and the path to `file.cfg` is specified as `./myinput/file.cfg`, relative to the container’s working directory `/app/triton/build`.

Therefore, in this example, the input data file paths inside `file.cfg` should start with `./myinput`.
