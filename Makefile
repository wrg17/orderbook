# Entry point for local development. Wraps the dockerized scripts so you don't
# have to remember the docker compose invocation. Run `make` for the list.
.DEFAULT_GOAL := help

COMPOSE_RUN := docker compose run --rm orderbook-dev bash

.PHONY: help init format format-check lint build test verify shell clean

help:
	@echo "Targets:"
	@echo "  init          One-time setup: deps check, exec bits, enable pre-push hook"
	@echo "  format        Apply clang-format in-place"
	@echo "  format-check  Verify formatting (no changes)"
	@echo "  lint          Run clang-tidy (strict: warnings are errors)"
	@echo "  build         Configure and build (CI preset)"
	@echo "  test          Build and run unit tests"
	@echo "  verify        format-check + diff-only lint + test (fast local pass; CI runs full lint)"
	@echo "  shell         Open a bash shell inside the dev container"
	@echo "  clean         Remove the build directory"

init:
	./scripts/init.sh

format:
	$(COMPOSE_RUN) scripts/format.sh

format-check:
	$(COMPOSE_RUN) scripts/format-check.sh

lint:
	$(COMPOSE_RUN) scripts/lint.sh

build:
	$(COMPOSE_RUN) scripts/build.sh

test:
	$(COMPOSE_RUN) scripts/test.sh

verify:
	./scripts/verify.sh

shell:
	docker compose run --rm orderbook-dev bash

clean:
	rm -rf build/
