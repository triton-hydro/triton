.. _releases:

Releases
========

TRITON is developed at https://code.ornl.gov/hydro/triton
and released publicly at `triton-hydro/triton <https://github.com/triton-hydro/triton>`_.
This page explains what is published there, how to choose a download, how the
two repositories relate, where to report a problem, and where to send a change.

The release repository
----------------------

The GitLab repository is authoritative. It holds the development history, the
development issue tracker, the merge requests, and the review process, and it
is where the code is written.

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
``2.1.1``, not ``v2.1.1``. The tag names are identical on both sides, so a
version named in a paper, a log file, or a support request identifies the same
release in either repository.

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
   GitHub generates automatically for every tag. TRITON depends on the
   Kokkos and yaml-cpp submodules under ``external/``. GitHub's generated
   archives include the submodule directories but none of their contents,
   so ``external/kokkos`` and ``external/yaml-cpp`` arrive empty; the CMake
   configure step fails on the missing Kokkos sources. The
   ``triton-<version>.tar.gz`` asset has both submodules vendored in and
   builds without network access.

Downloading and verifying a release:

.. code-block:: bash

    curl -LO https://github.com/triton-hydro/triton/releases/download/2.1.1/triton-2.1.1.tar.gz
    curl -LO https://github.com/triton-hydro/triton/releases/download/2.1.1/triton-2.1.1.tar.gz.sha256
    sha256sum -c triton-2.1.1.tar.gz.sha256
    tar xzf triton-2.1.1.tar.gz
    cd triton-2.1.1

The checksum is published with the release. If it does not match, do not use
the file: download it again, and if it still does not match, report it -- see
`Reporting problems`_ below.

Cloning instead of downloading
------------------------------

Cloning the release repository works, provided the submodules come with it:

.. code-block:: bash

    git clone --recursive https://github.com/triton-hydro/triton.git
    cd triton
    git checkout 2.1.1

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
   * - Source, ``input/``, ``cmake/``, ``external/`` pins, and the rest of
       ``doc/``
     - Verbatim from the upstream release tag
   * - ``LICENSE``
     - Upstream text with an SPDX identifier line prepended; the terms are
       unchanged
   * - ``README.md``, ``CONTRIBUTING.md``, ``CHANGELOG.md``, ``.github/``
     - Written for the public release repository
   * - ``doc/releases.rst`` and small edits to ``doc/index.rst``,
       ``doc/conf.py``, ``doc/installation.rst``
     - Added or adjusted so the published documentation describes the release
       you actually downloaded

Every other path is byte-identical to the upstream tag, and each release
states in its commit message which upstream tag and commit it was published
from.

Reporting problems
------------------

Bugs, usage questions, feature requests, and problems with a published release
asset -- a checksum that does not match, a missing or truncated asset, a
tarball that will not extract -- are reported in the release repository's
issue tracker at https://github.com/triton-hydro/triton/issues, quoting the
version and the asset name. A free GitHub account is all you need; no account
on the development GitLab is required.

Maintainers open the corresponding upstream issue when a report needs work in
the development repository. A code fix reaches you in a later release, and
``CHANGELOG.md`` names the version. A published asset is a separate matter: an
asset can be replaced under the tag it was published on, and a re-uploaded
tarball or a corrected ``.sha256`` file is not a new release, so it carries no
CHANGELOG entry -- check the
`releases page <https://github.com/triton-hydro/triton/releases>`__ for the
current asset and its checksum.

**Security problems are different.** Do not open a public issue. The
`security policy <https://github.com/triton-hydro/triton/blob/main/.github/SECURITY.md>`_
in ``.github/SECURITY.md`` explains how to report one privately, and what to do
when no private route is open to you.

Contributing changes
--------------------

Pull requests opened against the release repository cannot be merged. Every
release commit there is generated from an upstream release tag, so a merge made
on that side would block the next release rather than travel with it, and it
would never reach the people working on the code. That is a statement about
code, not about problems: reports belong in the release repository's issue
tracker, as above.

Code and documentation changes are reviewed upstream at
https://code.ornl.gov/hydro/triton/-/merge_requests. If you cannot obtain an
account there, ``CONTRIBUTING.md`` in the release repository describes how to
send a patch through an issue there instead.
