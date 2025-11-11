.. _docguide:

TRITON Documentation Guide
===========================

This section explains how to develop the TRITON documentation.

**Prerequisites**:

* **Python:** Version 3
* **Doxygen:** Required for TRITON API generation
* **make:** Used to run commands for generating and cleaning documentation

**Setup Environment**:

We recommend creating a Python virtual environment for TRITON documentation
development:

.. code-block:: bash

    python -m venv .venv_triton
    source .venv_triton/bin/activate
    pip install sphinx breathe exhale sphinx_rtd_theme sphinx_design

    # Install Doxygen (example for Linux)
    sudo apt install doxygen

**Generating Documentation**:

From the top-level directory where the `Makefile` is located, run:

.. code-block:: bash

    make clean_doc
    make doc

When the process completes, `index.html` will be generated in
`triton/doc/_build/html` directory. Open `index.html` in your browser
to view the documentation.
