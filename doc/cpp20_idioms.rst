C++20 Idioms in TRITON
======================

This page summarizes the C++20 idioms TRITON adopted in the May 2026
modernization. Contributors writing new TRITON code should follow these
patterns and respect the host/device split.

Host code
---------

* Use **concepts** to constrain templates. ``template<std::floating_point T>``
  for numeric kernels and solver state. ``template<Matrix::Arithmetic T>``
  (defined in ``src/matrix.h``) for code that operates on both floating-point
  and integral types (e.g., ``Matrix::matrix<T>`` is instantiated with both
  ``double`` for solver state and ``int`` for runoff indices).
* Use ``std::span<T>`` instead of pointer + size pairs for buffer arguments.
  See ``Matrix::matrix::data_span()`` for the additive accessor pattern.
* Use ``std::format`` (header ``<format>``) for string assembly. Do not
  introduce new ``std::stringstream`` chains.
* Use ``std::ranges`` algorithms (``std::ranges::find``, ``std::ranges::sort``,
  ``std::views::drop``, etc.) instead of iterator-pair calls.
* Use designated initializers ``{.x = 1, .y = 2}`` for aggregate
  construction when the struct has named members.
* Use ``using enum E;`` inside functions that reference enumerators
  of ``enum class E`` repeatedly.
* Use ``std::string::starts_with`` and ``std::string::ends_with`` for
  prefix/suffix checks.
* Use ``[[likely]]`` and ``[[unlikely]]`` only on host-side branches that
  benefit from the hint (error paths, rare conditions).
* Use ``constinit`` for non-const globals that must be initialized at
  compile time (not applicable in current TRITON because all constants
  in ``constants.h`` are ``typedef``s and ``#define`` macros).

Device code
-----------

Anything inside ``KOKKOS_INLINE_FUNCTION``, ``KOKKOS_LAMBDA``, or a function
called from a Kokkos kernel is **device code**. Apply only:

* **Concepts on the outer wrapping template** (the constraint is applied
  at host-side declaration; the body is unaffected).
* ``using enum`` (pure compile-time).
* ``constexpr`` expansion where the compiler accepts it.

Do NOT use inside device code:

* ``std::ranges``, ``std::format``, ``std::span`` (not portable across
  nvcc/hipcc/SYCL)
* ``[[likely]]``/``[[unlikely]]`` (until verified per backend)
* Coroutines, modules

Build system
------------

* Minimum CMake: 3.20
* Minimum C++ standard: 20 (``CMAKE_CXX_STANDARD 20``)
* Minimum compilers: gcc 13+, clang 16+, nvcc 12.4+, Cray CCE 16+,
  Intel oneAPI 2024.0+
* Kokkos: vendored submodule at tag 5.1.1

ALLATOONA test exclusion
------------------------

The standard ctest gate during development excludes the ALLATOONA cases
because they take ~17 minutes each. Use::

    ./triton_ctest.sh -E ALLATOONA

This runs the 4 PARABOLOID and CIRCULAR-DAMBREAK cases (including
BIT4BIT-* raster comparisons) in ~95 seconds. Run the full suite
before merging to main.

Header organization
-------------------

After the May 2026 split, the four largest TRITON headers are decomposed
as facades:

* ``matrix.h`` → ``matrix.h`` (decl + inline ops) + ``matrix_io.h``
* ``output.h`` → ``output.h`` (decl + dispatch) + ``output_ascii.h``,
  ``output_binary.h``, ``output_geotiff.h``, ``output_timeseries.h``
* ``kernels.h`` → ``kernels.h`` (orchestrator + non-step helpers) +
  ``kernels_flux.h``, ``kernels_state.h``, ``kernels_wetdry.h``,
  ``kernels_halo.h``
* ``triton.h`` → ``triton.h`` (class decl + sub-includes) +
  ``triton_init.h``, ``triton_simulate.h``, ``triton_decomp.h``

Each umbrella re-includes its parts so existing ``#include "triton.h"``
(etc.) consumers work unchanged.

GPU spot-check (post-merge)
---------------------------

The CI on this dev box covers only the SERIAL backend via ``ctest``.
Before merging the modernization branch to ``master``, manually
verify on a GPU machine:

* Frontier (HIP): build and run paraboloid; compare output to reference
* Perlmutter-gpu (CUDA): build and run paraboloid; compare output to
  reference

(Aurora SYCL: build-only verification acceptable.)
