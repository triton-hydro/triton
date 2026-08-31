# TRITON

**Two-dimensional Runoff Inundation Toolkit for Operational Needs**

TRITON is an open-source, high-performance software framework for simulating
two-dimensional flood inundation. It solves the shallow water equations on
structured grids and runs on both CPUs and GPUs, using
[Kokkos](https://github.com/kokkos/kokkos) for performance portability and MPI
for execution across multiple nodes.

- **Project website:** <https://triton.ornl.gov>
- **User documentation:** <https://triton-ornl.readthedocs.io>
- **Questions and bug reports:** <https://github.com/triton-hydro/triton/issues>

## About this repository

This is the **official public release repository** for TRITON. The most recent
release published here is **2.1.1**.

TRITON is developed at <https://code.ornl.gov/hydro/triton>.
That repository is authoritative: it holds the development history, the
development issue tracker, the merge requests, and the review process. This
repository is strictly downstream of it. Each release here is a single snapshot
commit taken from a released upstream tag, tagged with the same version number,
and published together with a ready-to-build tarball. Nothing is ever pushed
from here back upstream.

That is a statement about *code*. Reporting a problem is a different matter:
this repository's issue tracker is open, and [Getting help](#getting-help) says
what belongs where.

## Getting TRITON

### Download a release tarball — recommended

Every release is listed on the
[Releases page](https://github.com/triton-hydro/triton/releases). Download the asset named
**`triton-<version>.tar.gz`** — for this version, that is
`triton-2.1.1.tar.gz`.

**Do not use the "Source code (zip)" or "Source code (tar.gz)" links that GitHub
generates automatically.** TRITON depends on two git submodules, Kokkos and
yaml-cpp, under `external/`. GitHub's generated source archives contain the
submodule *directories* but none of their *contents*, so `external/kokkos` and
`external/yaml-cpp` arrive empty and the CMake configure step fails. The
`triton-<version>.tar.gz` asset has both submodules vendored in, so it
builds with no further downloads.

Every tarball is published with a matching `.sha256` file:

```bash
curl -LO https://github.com/triton-hydro/triton/releases/download/2.1.1/triton-2.1.1.tar.gz
curl -LO https://github.com/triton-hydro/triton/releases/download/2.1.1/triton-2.1.1.tar.gz.sha256
sha256sum -c triton-2.1.1.tar.gz.sha256
tar xzf triton-2.1.1.tar.gz
cd triton-2.1.1
```

### Clone this repository

Cloning works too, as long as the submodules come with it:

```bash
git clone --recursive https://github.com/triton-hydro/triton.git
cd triton
git checkout 2.1.1
```

If you have already cloned without `--recursive`:

```bash
git submodule update --init --recursive
```

A clone pulls the submodules from their own upstreams, so unlike the release
tarball it needs network access to github.com/kokkos and github.com/jbeder.

### Clone the development repository

To follow development rather than build a release:

```bash
git clone --recursive https://code.ornl.gov/hydro/triton.git
```

## Building and documentation

User documentation is published at <https://triton-ornl.readthedocs.io>, built
from the development repository. Prerequisites and the CMake build are
documented in `doc/installation.rst`. `doc/releases.rst`, which this repository
adds to its copy of `doc/`, describes how releases are published here and which
download to choose. The documentation sources under `doc/` build with Sphinx.

## Releases

- [CHANGELOG.md](CHANGELOG.md) — release notes for every version published here
- [Releases page](https://github.com/triton-hydro/triton/releases) —
  tarballs and their checksums
- Tags carry the upstream version number exactly (`2.1.1`, not `v2.1.1`)
  and are never moved or deleted once published

## Getting help

1. **Read the documentation** — <https://triton-ornl.readthedocs.io>.
   Installation, configuration, running a simulation, and the case studies.
2. **Open an issue here** — <https://github.com/triton-hydro/triton/issues>.
   Bugs, feature requests, usage questions the documentation does not answer,
   and problems with a published release asset — a checksum that does not
   match, a missing or truncated asset, a tarball that will not extract.
   Quote the version tag and the asset name. A free GitHub account is all you
   need; no account on the development GitLab is required.
3. **Non-technical matters** — <https://triton.ornl.gov/contact/>.
   Collaboration, institutional and press enquiries.

TRITON is developed at <https://code.ornl.gov/hydro/triton>, which not
everyone can get an account on. Maintainers open the corresponding upstream
issue when a report needs work in the development repository. A code fix
reaches you in a later release, and [CHANGELOG.md](CHANGELOG.md) names the
version. A published asset is a separate matter: replacing a tarball or a
`.sha256` file is not a release and carries no CHANGELOG entry, so check the
[Releases page](https://github.com/triton-hydro/triton/releases) for the
current asset and its checksum.

**Security problems go to none of the three routes above.** Do not open a
public issue.
[SECURITY.md](.github/SECURITY.md) explains how to report one privately, and
what to do when no private route is open to you.

## Contributing code

**Pull requests cannot be merged in this repository.** Every release commit
here is generated from an upstream release tag, and the chain is assembled
elsewhere and pushed here, so a merge made on this side is not part of that
chain: it would block the next release rather than travel with it, and it would
never reach the people working on the code. This is about code changes only —
to report a problem, see [Getting help](#getting-help).

Code and documentation changes are reviewed upstream at
<https://code.ornl.gov/hydro/triton/-/merge_requests>. If you cannot obtain an
account there, open an issue on this repository and include the patch. Produce
it with `git format-patch`, then paste it into the issue inside a fenced code
block, or attach it as a `.txt` or `.zip` file.

[CONTRIBUTING.md](CONTRIBUTING.md) has the full policy.

## Citing TRITON

If you use TRITON in published work, please cite:

> Morales Hernández, M., M. B. Sharif, A. J. Kalyanapu, S. K. Ghafoor,
> T. T. Dullo, S. Gangrade, S.-C. Kao, M. Norman, and K. J. Evans (2021).
> *TRITON: A Multi-GPU Open Source 2D Hydrodynamic Flood Model.*
> *Environmental Modelling & Software, 141*, 105034.
> <https://doi.org/10.1016/j.envsoft.2021.105034>

`doc/papers.rst` lists further TRITON studies and applications.

## License

TRITON is distributed under the BSD 3-Clause License. See [LICENSE](LICENSE)
for the full text, including the copyright held by UT-Battelle, LLC and
Tennessee Technological University.
