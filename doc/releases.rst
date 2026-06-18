.. _releases:

Releases
========

TRITON is developed at https://code.ornl.gov/hydro/triton
and released publicly at `triton-hydro/triton <https://github.com/triton-hydro/triton>`_.
This page explains what is published there, how to choose a download, and how
the two repositories relate.

The release repository
----------------------

The GitLab repository is authoritative. It holds the development history, the
issue tracker, the merge requests, and the review process, and it is where all
work happens.

The GitHub repository is the official public distribution channel. It is
strictly downstream: each release is a single snapshot commit taken from a
released upstream tag, carrying the same version number, published together
with a tarball that is ready to build. Nothing is ever pushed from GitHub back
to GitLab.

Because releases are published as snapshots, the GitHub repository contains no
development history and no per-commit authorship. Both live in GitLab, and every
published commit message records the upstream tag and commit it came from.

Version numbers
---------------

Releases are numbered ``MAJOR.MINOR.PATCH`` and the tag is the bare version --
``2.1.0``, not ``v2.1.0``. Tags are identical on both sides, so a
version named in a paper, a log file, or a support request identifies exactly
one tree.

A published tag is never moved or deleted. If a release needs correcting, the
correction is a new version.

Choosing a download
-------------------

Every release on the
`releases page <https://github.com/triton-hydro/triton/releases>`_ carries an asset named
``triton-<version>.tar.gz`` together with a
``triton-<version>.tar.gz.sha256`` checksum file. **That asset is the
supported download.**

.. warning::

   Do not use the "Source code (zip)" or "Source code (tar.gz)" links that
   GitHub generates automatically for every tag. TRITON depends on the Kokkos
   and yaml-cpp submodules under ``external/``. GitHub's generated archives
   include the submodule directories but none of their contents, so
   ``external/kokkos`` and ``external/yaml-cpp`` arrive empty and the CMake
   configure step fails. The ``triton-<version>.tar.gz`` asset has
   both submodules vendored in and builds without network access.

Downloading and verifying a release:

.. code-block:: bash

    curl -LO https://github.com/triton-hydro/triton/releases/download/2.1.0/triton-2.1.0.tar.gz
    curl -LO https://github.com/triton-hydro/triton/releases/download/2.1.0/triton-2.1.0.tar.gz.sha256
    sha256sum -c triton-2.1.0.tar.gz.sha256
    tar xzf triton-2.1.0.tar.gz
    cd triton-2.1.0

The checksum is published with the release and is re-checked against the
uploaded asset after every publication, so a mismatch means the file was
damaged or altered in transit -- download it again before using it.

Cloning instead of downloading
------------------------------

Cloning the release repository works, provided the submodules come with it:

.. code-block:: bash

    git clone --recursive https://github.com/triton-hydro/triton.git
    cd triton
    git checkout 2.1.0

If the clone was made without ``--recursive``:

.. code-block:: bash

    git submodule update --init --recursive

Unlike the release tarball, a clone fetches the submodules from their own
upstream repositories, so it needs network access to github.com.

To work on TRITON rather than build a release, clone the development
repository instead:

.. code-block:: bash

    git clone --recursive https://code.ornl.gov/hydro/triton.git

What each release contains
--------------------------

A published release is the upstream release tree with a small, documented set
of additions for the public repository:

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Path
     - Origin
   * - Source, ``doc/``, ``input/``, ``cmake/``, ``external/`` pins
     - Verbatim from the upstream release tag
   * - ``LICENSE``
     - Upstream text with an SPDX identifier line prepended so the license is
       machine-detectable; the terms are unchanged
   * - ``README.md``, ``CONTRIBUTING.md``, ``CHANGELOG.md``, ``.github/``
     - Written for the public release repository
   * - ``doc/releases.rst`` and small edits to ``doc/index.rst``,
       ``doc/conf.py``, ``doc/installation.rst``
     - Added or adjusted so the published documentation describes the release
       you downloaded

Every other path is byte-identical to the upstream tag, and each release is
verified against both the upstream tree and the published checksum after it is
pushed.

Reporting problems
------------------

Bugs, questions, and feature requests belong in the upstream issue tracker at
https://code.ornl.gov/hydro/triton/-/issues.
The release repository's own issue tracker is intentionally disabled, and pull
requests opened against it cannot be merged. Problems with a published artifact
-- a checksum that does not match, a missing asset, a tarball that will not
extract -- are reported in the same place, quoting the version and the asset
name.
