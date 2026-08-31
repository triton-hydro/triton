# Security policy

TRITON is research software published by UT-Battelle, LLC and Tennessee
Technological University under the BSD 3-Clause License; see
[LICENSE](../LICENSE). This repository is the public release site for TRITON.
A vulnerability in a published release is reported privately, by one of the
routes below.

## Never report a vulnerability in a public issue

Do not open an issue at <https://github.com/triton-hydro/triton/issues> for a
security problem, and do not describe one in a pull request, in a comment, or
anywhere else public. A public report cannot be taken back:

- **Watchers are notified the moment it is created.** People watching this
  repository can receive the full text the instant you press the button,
  before you have a chance to reconsider.
- **Copies persist after deletion.** Notification emails, search-engine caches
  and third-party mirrors keep what was posted.

## How to report a vulnerability

Three routes. Use the best one that is available to you: routes 1 and 2 accept
full technical detail, route 3 does not.

### 1. GitHub private vulnerability reporting

Open the **Security and quality** tab of this repository and choose **Report a
vulnerability**. This route needs nothing but a free GitHub account, and
nothing you write there becomes public on its own, so full detail belongs in
it. Paste code, logs and any proof-of-concept into the report inside a fenced
code block.

**If that tab offers no "Report a vulnerability" button**, this route is not
available on this repository. Go to route 2.

### 2. A confidential issue in the development repository

TRITON is developed at <https://code.ornl.gov/hydro/triton>. Its issue tracker
is not open to the public — signed out, the URL reports it as not found — so
this route is open only to people who already work in that project and can
open an issue there. If you can, open a new issue and turn on confidentiality
before you submit it. A confidential issue is not visible to the public, so
full detail belongs in it.

Turn it on before submitting rather than after. An issue created public and
made confidential afterwards has already been public, and everything above
about a public report applies to it.

If that tracker is not open to you, this route is closed. Route 3 is not, and
it requires no account anywhere.

### 3. The TRITON contact page

<https://triton.ornl.gov/contact/> is public and reachable without an account
of any kind. It is open to you when routes 1 and 2 are not.

**Use it to ask for a private channel, not to file the report.** It is a
general-purpose web form on the project website and is not a confidential
channel. Write that you have a possible security issue in
TRITON and need a private way to send the details, and stop there.

Do not put any of this into that form:

- exploit detail, or how the problem is triggered
- proof-of-concept code, input files, or crash output
- which versions are affected

Send those only over a channel that is private.

## What to include in a report

As much of the following as you have. A partial report on a private route is
worth more than a complete one in public.

- the version you tested — a release tag such as `2.1.1`, or the commit you
  built from
- how you obtained it: a release tarball, a `git clone`, or a package built
  by someone else
- the platform and the build — compiler, MPI, whether CUDA, HIP or CPU-only,
  and the Kokkos backend
- what an attacker gains: reading a file, corrupting results, executing code,
  crashing a long simulation
- the smallest input, configuration or command line that triggers it
- what you observed — the error, the stack trace, the sanitiser report
- any proof-of-concept code, pasted inside a fenced code block, on route 1 or
  route 2 only and never in the contact form
- whether the problem is already public anywhere

## Which release to test against

Releases are published on the
[Releases page](https://github.com/triton-hydro/triton/releases) of this
repository. No support window is stated for any release, so check which one is
current before reporting, and test against it if you can.

## Kokkos and yaml-cpp

TRITON builds against Kokkos, and against yaml-cpp when the ensemble build is
enabled. They are git submodules under `external/` in a clone, and are
vendored into the release tarball. A vulnerability in one of them belongs to
the project that maintains it, and each publishes its own security policy —
`external/kokkos/docs/SECURITY.md` and `external/yaml-cpp/SECURITY.md`. Report
it to that project rather than here. If you cannot tell which side of the
boundary a problem falls on, use one of the routes above and say what you are
unsure about.

## If it is not a security problem

Bugs, usage questions, feature requests and problems with a published release
asset are reported in the open, in this repository's issue tracker at
<https://github.com/triton-hydro/triton/issues>. A free GitHub account is all
you need. [CONTRIBUTING.md](../CONTRIBUTING.md) has the routing for everything
else.
