# Contributing to TRITON

Thank you for your interest in TRITON. This repository is the public release
site, and it is the right place to **report** something: bugs, usage questions,
feature requests, and problems with a published release asset all belong in
this repository's issue tracker at
<https://github.com/triton-hydro/triton/issues>. A free GitHub account is all
you need, and no account on the development GitLab is required.

**Code changes are the exception: this repository cannot merge them.** Its
contents are generated from released upstream tags, and anything committed here
would be replaced by the next release without ever reaching the developers.
Code and documentation changes are reviewed upstream at
<https://code.ornl.gov/hydro/triton/-/merge_requests>, and if you cannot obtain
an account there, there is a route for that too.

Everything below is about where each kind of thing goes.

## Where development happens

TRITON is developed at <https://code.ornl.gov/hydro/triton>.
That repository holds the full commit history, the development issue tracker,
the merge requests, and the code review process. Releases are published from it
to <https://github.com/triton-hydro/triton> as snapshot commits, one per
version, each with a tarball that includes the Kokkos and yaml-cpp submodules.

## Where each thing goes

| What you want to do | Where to do it | What happens next |
| --- | --- | --- |
| Report a bug | [GitHub issue](https://github.com/triton-hydro/triton/issues) on this repository | Maintainers open the corresponding upstream issue when a report needs work in the development repository. |
| Request a feature | [GitHub issue](https://github.com/triton-hydro/triton/issues) on this repository | Maintainers open the corresponding upstream issue when a report needs work in the development repository. |
| Ask a usage question | Check <https://triton-ornl.readthedocs.io> first, then a [GitHub issue](https://github.com/triton-hydro/triton/issues) | Maintainers open the corresponding upstream issue when a report needs work in the development repository. |
| Report a broken release asset | [GitHub issue](https://github.com/triton-hydro/triton/issues), quoting the version and asset name | The assets are published from this repository. Maintainers open the corresponding upstream issue when a report needs work in the development repository. |
| Submit a code or documentation change | Merge request at <https://code.ornl.gov/hydro/triton/-/merge_requests> | Review happens in the development repository. Changes that ship appear here with the release that carries them. |
| Submit a code change without an account on that GitLab instance | [GitHub issue](https://github.com/triton-hydro/triton/issues) with the patch pasted into the issue inside a fenced code block, or attached as a `.txt` or `.zip` file | Maintainers open the corresponding upstream issue when a report needs work in the development repository. |
| Report a security problem | **Not a public issue** — see [SECURITY.md](.github/SECURITY.md) | That file explains how to report one privately, and what to do when no private route is open to you. |
| Collaboration, institutional and press enquiries | <https://triton.ornl.gov/contact/> | — |

Follow whatever contribution, style, and review guidance the upstream
repository provides; this page does not restate it, so that the two cannot
drift apart.

## Reporting something sensitive

If the problem is a security issue, do not open a public issue. A public issue
cannot be withdrawn: watchers are notified the moment it is created, and copies
persist after it is deleted.

[SECURITY.md](.github/SECURITY.md) explains how to report one privately, and
what to do when no private route is open to you. It is the only place the
routes are written out, so that they cannot drift.

## Pull requests opened here

Pull requests against `triton-hydro/triton` cannot be merged. This is not a
judgement about the change — every release commit on this branch is generated
from an upstream release tag, and the chain is assembled elsewhere and pushed
here. A merge made on this side is not part of that chain, so it would block
the next release rather than travel with it, *and* it would never reach anyone
working on the code.

To move a pull request upstream:

1. Keep your branch; nothing is lost.
2. Create an account on <https://code.ornl.gov/hydro/triton> if you can.
3. Fork the upstream project, push your branch there, and open a merge request
   at <https://code.ornl.gov/hydro/triton/-/merge_requests> against the
   development branch.
4. Link the merge request in your pull request, then close the pull request.

If you cannot obtain an account there, open an issue on **this** repository at
<https://github.com/triton-hydro/triton/issues> describing the change, and
include the patch: produce it with `git format-patch`, then paste it into the
issue inside a fenced code block, or attach it as a `.txt` or `.zip` file.
Maintainers open the corresponding upstream issue when a report needs work in
the development repository.

## Reporting a problem with a release itself

Problems with what this repository *publishes*, as opposed to what TRITON
*does*, are reported here — this is the repository that publishes them. Open
an issue at <https://github.com/triton-hydro/triton/issues> and include:

- the version tag, for example `2.1.1`
- the asset name, for example `triton-2.1.1.tar.gz`
- the checksum you computed and the one published in the matching `.sha256`
  file
- how you obtained the file (release asset, `git clone`, or the GitHub-generated
  source archive — note that the generated archives omit the submodule contents
  and are not supported; see [README.md](README.md))

An asset can be replaced under the tag it was published on: a re-uploaded
tarball or a corrected `.sha256` file is not a new release, and
[CHANGELOG.md](CHANGELOG.md) records the versions published here, so a
replacement of that kind does not appear there. Where the *contents* of an
asset are wrong because the upstream tag they were built from is wrong, the fix
belongs to the development repository. Maintainers open the corresponding
upstream issue when a report needs work in the development repository.

## What is in this repository

| Path | Origin |
| --- | --- |
| Source, `input/`, `cmake/`, `external/`, and the rest of `doc/` | Verbatim from the upstream release tag |
| `README.md`, `CONTRIBUTING.md`, `CHANGELOG.md`, `.github/` | Written for the public release repository |
| `LICENSE` | Upstream text with an SPDX identifier line prepended; the terms are unchanged |
| `doc/releases.rst`, `doc/index.rst`, `doc/conf.py`, `doc/installation.rst` | Added or adjusted so the published documentation describes the release you actually downloaded |

Every other path is byte-identical to the upstream tag, and each release states
in its commit message which upstream tag and commit it was published from.

## License

TRITON is distributed under the BSD 3-Clause License; see [LICENSE](LICENSE).
Contributions are made upstream and are licensed on the terms the upstream
project states.
