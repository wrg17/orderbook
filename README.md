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

Run the init script once after cloning. It verifies the host tooling, marks the helper scripts executable, and enables
the pre-push hook:

```bash
./scripts/init.sh
```

Re-running it is safe — every step is idempotent.

## Local development

Anything CI does, you can do locally with the same command:

```bash
docker compose run --rm orderbook-dev bash scripts/format-check.sh   # clang-format
docker compose run --rm orderbook-dev bash scripts/lint.sh           # clang-tidy
docker compose run --rm orderbook-dev bash scripts/test.sh           # build + ctest
```

## Pre-push hook

`scripts/init.sh` points `core.hooksPath` at `.githooks/`, which makes `git push` run the three checks above first — so
what passes locally passes in CI. To bypass for a WIP branch, use `git push --no-verify`.
