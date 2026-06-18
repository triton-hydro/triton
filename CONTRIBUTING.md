# Contributing to TRITON

Thank you for your interest in TRITON. **This repository does not accept
contributions directly.** It is the public release site: its contents are
generated from released upstream tags, and anything committed here would be
replaced by the next release without ever reaching the developers.

Everything below is about where to send your change instead — and the answer is
always the development repository at <https://code.ornl.gov/hydro/triton>.

## Where development happens

TRITON is developed at <https://code.ornl.gov/hydro/triton>.
That repository holds the full commit history, the issue tracker, the merge
requests, and the code review process. Releases are published from it to
<https://github.com/triton-hydro/triton> as snapshot commits, one per version,
each with a tarball that includes the Kokkos and yaml-cpp submodules.

| What you want to do | Where to do it |
| --- | --- |
| Report a bug | <https://code.ornl.gov/hydro/triton/-/issues> |
| Request a feature | <https://code.ornl.gov/hydro/triton/-/issues> |
| Ask a usage question | <https://code.ornl.gov/hydro/triton/-/issues> |
| Submit a code or documentation change | Merge request at <https://code.ornl.gov/hydro/triton/-/merge_requests> |
| Report a broken release asset | <https://code.ornl.gov/hydro/triton/-/issues>, quoting the version and asset name |

Follow whatever contribution, style, and review guidance the upstream
repository provides; this page does not restate it, so that the two cannot
drift apart.

## Reporting something sensitive

If the problem is a security issue, do not open a public issue. Use a
confidential issue in the upstream tracker (**New issue → "This issue is
confidential"**) at <https://code.ornl.gov/hydro/triton/-/issues>,
or contact the TRITON maintainers through the institutional channel listed on
the upstream project page.

## Pull requests opened here

Pull requests against `triton-hydro/triton` cannot be merged, and maintainers will
close them with a pointer to the upstream merge request tracker. This is not a
judgement about the change — every commit on this branch is generated from an
upstream release tag, and the chain is assembled elsewhere and pushed here. A
merge made on this side is not part of that chain, so it would block the next
release rather than travel with it, *and* it would never reach anyone working on
the code.

To move a pull request upstream:

1. Keep your branch; nothing is lost.
2. Create an account on <https://code.ornl.gov/hydro/triton> if you do not have one.
3. Fork the upstream project, push your branch there, and open a merge request
   against the development branch.
4. Link the merge request in your pull request, then close the pull request.

If you cannot create an upstream account — some deployments restrict
registration — open an issue upstream describing the change and attach a patch
produced with `git format-patch`.

## Reporting a problem with a release itself

Problems with what this repository *publishes*, as opposed to what TRITON
*does*, are still reported upstream. Please include:

- the version tag, for example `2.1.0`
- the asset name, for example `triton-2.1.0.tar.gz`
- the checksum you computed and the one published in the matching `.sha256`
  file
- how you obtained the file (release asset, `git clone`, or the GitHub-generated
  source archive — note that the generated archives omit the submodule contents
  and are not supported; see [README.md](README.md))

## What is in this repository

| Path | Origin |
| --- | --- |
| Source, `doc/`, `input/`, `cmake/`, `external/` | Verbatim from the upstream release tag |
| `README.md`, `CONTRIBUTING.md`, `CHANGELOG.md`, `.github/` | Written for the public release repository |
| `LICENSE` | Upstream text with an SPDX identifier line prepended, so the license is machine-detectable |
| `doc/releases.rst`, `doc/index.rst`, `doc/conf.py`, `doc/installation.rst` | Upstream, adjusted so the published documentation describes the release you actually downloaded |

Every other path is byte-identical to the upstream tag, and each release states
in its commit message which upstream tag and commit it was published from.

## License

TRITON is distributed under the BSD 3-Clause License; see [LICENSE](LICENSE).
Contributions are made upstream and are licensed on the terms the upstream
project states.
