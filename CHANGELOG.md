# Changelog

<!-- Assembled at release time from the per-release notes;
     edit those, not this file. -->

Release notes for every version of TRITON published to
[triton-hydro/triton](https://github.com/triton-hydro/triton), newest first.
Each section corresponds to that version's entry
on the [Releases page](https://github.com/triton-hydro/triton/releases).

The upstream development history, and the per-commit authorship
that goes with it, is at <https://code.ornl.gov/hydro/triton>.

## TRITON 2.1.1

### Changes

- Fix a bug that prevented support for multiple nodes on Miller.

---

Published from upstream tag `2.1.1` (`d18ed5b`) of https://code.ornl.gov/hydro/triton.
Development history, per-commit authorship, and the development issue
tracker stay in the upstream repository.

## TRITON 2.1.0

### Changes

- **TRITON now requires a C++20 compiler and CMake 3.20**, up from C++17 and
  CMake 3.16. Known-good compilers: gcc 13+, clang 16+, nvcc 12.4+,
  Cray CCE 16+, Intel oneAPI 2024.0+.
- The bundled Kokkos moves from a 4.5.99 development snapshot to **Kokkos 5.1.1**.
- The solver headers were reorganized. `triton.h`, `kernels.h`, `output.h`, and
  `matrix.h` were split into focused headers — `triton_init.h`,
  `triton_simulate.h`, `triton_decomp.h`, `kernels_flux.h`, `kernels_halo.h`,
  `kernels_state.h`, `kernels_wetdry.h`, `output_ascii.h`, `output_binary.h`,
  `output_geotiff.h`, `output_timeseries.h`, and `matrix_io.h`. Code that
  includes TRITON headers directly may need to include more than `triton.h`.
- New Cray build profile for Miller (`cmake/machines/miller/cray_CUDA.sh`), now
  the **default on that machine**: a build there with no compiler specified uses
  `PrgEnv-cray` and `CC` instead of the NVIDIA HPC compiler.
- New GNU build profile for the `arch` machine
  (`cmake/machines/arch/gnu_CUDA.sh`), also made that machine's default —
  `PrgEnv-gnu` on Arm Grace with a Hopper (`HOPPER90`) target.
- New `doc/cpp20_idioms.rst` documenting the C++20 patterns new TRITON code is
  expected to follow.
- Added `.clang-format` and `.clang-tidy` so formatting and static-analysis
  settings travel with the source.

---

Published from upstream tag `2.1.0` (`1790101`) of https://code.ornl.gov/hydro/triton.
Development history, per-commit authorship, and the development issue
tracker stay in the upstream repository.

## TRITON 2.0.0

### Changes

- First TRITON release published to this repository, from upstream tag `2.0.0` (`ec35bc4`).
    * Supports various CPUs and GPUs through Kokkos
    * Modernizes the build system using CMake
    * Supports ensemble simulations
    * Includes new documentation

---

Published from upstream tag `2.0.0` (`ec35bc4`) of https://code.ornl.gov/hydro/triton.
Development history, per-commit authorship, and the development issue
tracker stay in the upstream repository.
