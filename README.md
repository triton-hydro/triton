# TRITON

**Two-dimensional Runoff Inundation Toolkit for Operational Needs**

TRITON is an open-source, high-performance software framework for simulating
two-dimensional flood inundation. It solves the shallow water equations on
structured grids and runs on both CPUs and GPUs, using
[Kokkos](https://github.com/kokkos/kokkos) for performance portability and MPI
for execution across multiple nodes.

## About this repository

This is the **official public release repository** for TRITON. The most recent
release published here is **2.0.0**.

TRITON is developed at <https://code.ornl.gov/hydro/triton>.
That repository is authoritative: it holds the development history, the issue
tracker, the merge requests, and the review process. This repository is strictly
downstream of it. Each release here is a single snapshot commit taken from a
released upstream tag, tagged with the same version number, and published
together with a ready-to-build tarball. Nothing is ever pushed from here back
upstream.

Two consequences are worth stating plainly.

- **Per-commit authorship lives upstream.** Because every release is published
  as one snapshot commit, the commits in this repository are attributed to the
  release identity that published them, not to the people who wrote the code.
  `git log` and `git blame` here describe the publication, not the development.
  Contributor attribution for the code itself is in the upstream history at
  <https://code.ornl.gov/hydro/triton>.
- **There is no development history here.** The `main` branch holds
  one commit per published release, in order, so comparing two releases with
  `git diff` works exactly as you would expect — but there is nothing in between
  them.

## Getting TRITON

### Download a release tarball — recommended

Every release is listed on the
[Releases page](https://github.com/triton-hydro/triton/releases). Download the asset named
**`triton-<version>.tar.gz`** — for this version, that is
`triton-2.0.0.tar.gz`.

**Do not use the "Source code (zip)" or "Source code (tar.gz)" links that GitHub
generates automatically.** TRITON depends on two git submodules, Kokkos and
yaml-cpp, under `external/`. GitHub's generated source archives contain the
submodule *directories* but none of their *contents*, so `external/kokkos` and
`external/yaml-cpp` arrive empty and the CMake configure step fails. The
`triton-<version>.tar.gz` asset has both submodules vendored in, so it
builds with no further downloads.

Every tarball is published with a matching `.sha256` file:

```bash
curl -LO https://github.com/triton-hydro/triton/releases/download/2.0.0/triton-2.0.0.tar.gz
curl -LO https://github.com/triton-hydro/triton/releases/download/2.0.0/triton-2.0.0.tar.gz.sha256
sha256sum -c triton-2.0.0.tar.gz.sha256
tar xzf triton-2.0.0.tar.gz
cd triton-2.0.0
```

### Clone this repository

Cloning works too, as long as the submodules come with it:

```bash
git clone --recursive https://github.com/triton-hydro/triton.git
cd triton
git checkout 2.0.0
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

Prerequisites and the CMake build are documented in `doc/installation.rst`;
`doc/releases.rst` describes how releases are published here and which download
to choose. The documentation sources under `doc/` build with Sphinx.

## Releases

- [CHANGELOG.md](CHANGELOG.md) — release notes for every version published here
- [Releases page](https://github.com/triton-hydro/triton/releases) —
  tarballs and their checksums
- Tags carry the upstream version number exactly (`2.0.0`, not `v2.0.0`)
  and are never moved or deleted once published

## Issues, questions, and contributions

**Issues and pull requests are not handled in this repository.** It is a
publication target; its issue tracker is intentionally disabled, and a change
merged here could never reach the development repository.

- Report a bug or request a feature:
  <https://code.ornl.gov/hydro/triton/-/issues>
- Propose a change: open a merge request at
  <https://code.ornl.gov/hydro/triton/-/merge_requests>
- Report a problem with a published *artifact* — a checksum that does not
  match, a missing or truncated asset, a tarball that will not extract — as a
  GitLab issue as well, quoting the version and the asset name

[CONTRIBUTING.md](CONTRIBUTING.md) has the full policy.

## License

TRITON is distributed under the BSD 3-Clause License. See [LICENSE](LICENSE)
for the full text, including the copyright held by UT-Battelle, LLC and
Tennessee Technological University.
