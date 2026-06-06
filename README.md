# orderbook

A order-matching engine in C++20.

## Dependencies

On the host, you only need:

- **Docker** (with the Compose plugin) — everything else runs inside the dev container
- **Git** — clone with `--recurse-submodules` so the bundled vcpkg comes along

Inside the container (built from `docker/Dockerfile`):

- C++20 toolchain (g++, CMake, Ninja, GDB)
- clang-format, clang-tidy
- vcpkg (vendored at `extern/vcpkg`)
- GoogleTest (via vcpkg)

## Setup

Run `make init` once after cloning. It verifies the host tooling, marks the helper scripts executable, and enables the
pre-push hook:

```bash
make init
```

Re-running it is safe — every step is idempotent.

## Local development

Use `make` as the entry point — every target wraps the dockerized script so you don't have to remember the
`docker compose` invocation. Run `make` (or `make help`) for the list:

```bash
make format        # apply clang-format in-place
make format-check  # verify formatting (no changes)
make lint          # clang-tidy (strict — warnings are errors)
make build         # configure + build
make test          # build + ctest
make verify        # format-check + lint + test (same as the pre-push hook)
make shell         # open a bash shell inside the dev container
make clean         # remove the build directory
```

## Pre-push hook

`make init` points `core.hooksPath` at `.githooks/`, so `git push` runs `make verify` first — what passes locally passes
in CI. To bypass for a WIP branch, use `git push --no-verify`.
